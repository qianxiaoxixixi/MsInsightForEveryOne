/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2024. All rights reserved.
 */
import styled from '@emotion/styled';
import { Button, Checkbox, Input, Tooltip } from 'ascend-components';
import { observer } from 'mobx-react';
import React, { type ChangeEvent, useRef, useState } from 'react';
import { LinkIcon } from 'ascend-icon';
import type { Session } from '../entity/session';
import { CustomButton } from './base/StyledButton';
import { useTranslation } from 'react-i18next';
import { StyledEmpty } from './base/StyledEmpty';
import { action, runInAction } from 'mobx';
import type { InsightUnit, LinkLines } from '../entity/insight';
import { CardUnit, ThreadUnit } from '../insight/units/AscendUnit';
import { customDebounce } from '../utils/customDebounce';
import { getTimeOffset } from '../insight/units/utils';
import { CardMetaData, type HostMetaData, ProcessMetaData, ThreadMetaData } from '../entity/data';
import i18n from 'ascend-i18n';
import { Spin } from 'antd';

const FilterContainer = styled.div`
    display: flex;
    flex-direction: column;
    padding: 10px;
`;

const FilterList = styled.div`
    min-height: 100px;
    max-height: 200px;
    overflow-y: scroll;
`;

const FilterButtonLine = styled.div`
    display: flex;
    justify-content: space-evenly;
    align-items: center;
    margin-top: 14px;
`;

interface FilterItemProps {
    category: string;
    checkedCategories: string[];
    setCheckedCategories: React.Dispatch<React.SetStateAction<string[]>>;
}
const FilterItem: React.FC<FilterItemProps> = observer(({ category, checkedCategories, setCheckedCategories }) => {
    const isChecked = checkedCategories.includes(category);
    return (
        <p style={{ marginBottom: 0 }}>
            <Checkbox
                checked={isChecked}
                onChange={(): void => {
                    setCheckedCategories(prev => isChecked ? prev.filter(cat => cat !== category) : prev.concat(category));
                }}>
                {category}
            </Checkbox>
        </p>
    );
});

const getCardUnits = (units: InsightUnit[]): InsightUnit[] => {
    return units.flatMap(unit => {
        const res: InsightUnit[] = [];
        if (unit instanceof CardUnit) {
            res.push(unit);
        };
        if (unit.children) {
            res.push(...getCardUnits(unit.children));
        }
        return res;
    });
};

const getHostChildUnitCardId = (units: InsightUnit[], viewedCardIdSet: Set<string>): string[] => {
    return units.flatMap(unit => {
        const res: string[] = [];
        if (unit instanceof ThreadUnit) {
            const { cardId } = unit.metadata as { cardId: string };
            res.push(cardId);
        }
        if (unit.isExpanded && unit.children) {
            res.push(...getHostChildUnitCardId(unit.children, viewedCardIdSet));
        }
        return res;
    });
};

export interface DataBlock {
    pid: string;
    tid: string;
    timestamp: number;
    depth: number;
    height?: number;
    rankId?: string;
};
export interface FlowEvent {
    category: string;
    from: DataBlock;
    to: DataBlock;
    cardId: string;
};
interface CategoryEvents {
    flowDetailList: Array<{
        category: string;
        from: DataBlock;
        to: DataBlock;
    }>;
};

type FetchLinkLines = (session: Session) => Promise<CategoryEvents['flowDetailList']>;
type UseFetchLinkLines = Map<string, FetchLinkLines>;

const getLockRangeMetaList = (session: Session, cardId: string | undefined): any => {
    return session.lockUnit.map(selectUnit => {
        const { threadId, processId, metaType } = selectUnit?.metadata as ThreadMetaData ?? {};
        return {
            tid: threadId,
            pid: processId,
            metaType,
            rankId: cardId,
        };
    });
};

const useFetchLinkLines = (displayCategories: string[], viewedCardIdSet: Set<string>): UseFetchLinkLines => React.useMemo(() => new Map(
    displayCategories.map(category => [
        category,
        customDebounce(async (session: Session): Promise<CategoryEvents['flowDetailList']> => {
            const { domainStart, domainEnd } = session.domainRange;
            const { domain: { timePerPx } } = session;
            let res: CategoryEvents['flowDetailList'] = [];
            for (const unit of getCardUnits(session.units)) {
                if (!unit.isExpanded) {
                    continue;
                }
                const metadata = unit.metadata as ProcessMetaData;
                let { dataSource, cardId, dbPath } = metadata;
                if (cardId?.endsWith('Host')) {
                    const hostUnits: InsightUnit[] = [];
                    hostUnits.push(unit);
                    const hostThreadCardIds = getHostChildUnitCardId(hostUnits, viewedCardIdSet);
                    cardId = hostThreadCardIds[0];
                }
                // 如果不在可视范围内就不查询
                if (cardId !== undefined && !viewedCardIdSet.has(cardId)) {
                    continue;
                }
                const timestampOffset = getTimeOffset(session, metadata);
                const start = Math.floor(domainStart + timestampOffset);
                const end = Math.ceil(domainEnd + timestampOffset);
                const host = (unit.parent?.metadata as HostMetaData)?.host ?? '';
                const metadataList = getLockRangeMetaList(session, cardId);
                const lockStartTime = session.lockRange === undefined ? 0 : Math.floor(session.lockRange[0] + timestampOffset);
                const lockEndTime = session.lockRange === undefined ? 0 : Math.ceil(session.lockRange[1] + timestampOffset);
                const params =
                    {
                        rankId: cardId,
                        dbPath,
                        startTime: start,
                        endTime: end,
                        category,
                        timePerPx,
                        isSimulation: session.isSimulation,
                        host,
                        metadataList,
                        lockStartTime,
                        lockEndTime,
                    };
                res = res.concat((await window.request(dataSource,
                    { command: 'flow/categoryEvents', params }) as CategoryEvents).flowDetailList
                    .map(data => ({ ...data, cardId })));
            };
            return res;
        }),
    ]),
), [displayCategories, viewedCardIdSet]);

const useGetCategories = (session: Session, isSuspend: boolean): {categories: string[]; loading: boolean} => {
    const [categories, setCategories] = React.useState<string[]>([]);
    const [loading, setLoading] = React.useState<boolean>(false);
    const cardIdsRef = React.useRef<string>();
    React.useEffect(() => {
        if (!isSuspend) {
            return;
        }
        const cardUnits = getCardUnits(session.units);
        const cardUnitsParsed = cardUnits.filter(cardUnit => cardUnit.phase === 'download');
        const parsedCardIdsString = cardUnitsParsed.map((unit) => (unit.metadata as CardMetaData).cardId).toString();

        if (parsedCardIdsString === cardIdsRef.current) { return; }
        cardIdsRef.current = parsedCardIdsString;
        const fetchList: Array<Promise<{ category: string[] }>> = [];
        for (const unit of cardUnitsParsed) {
            const { dataSource, cardId, dbPath } = unit.metadata as CardMetaData;
            fetchList.push(window.request(dataSource, { command: 'flow/categoryList', params: { rankId: cardId, dbPath } }));
        }
        setLoading(true);
        Promise.all(fetchList).then((results) => {
            const curCategories = new Set<string>();
            results.forEach(({ category }) => {
                category.forEach((cat) => {
                    curCategories.add(cat);
                });
            });
            setCategories([...curCategories]);
            runInAction(() => {
                curCategories.forEach((category) => {
                    session.linkLines[category] = undefined;
                });
            });
        }).finally(() => {
            setLoading(false);
        });
    }, [isSuspend]);

    React.useEffect(() => {
        cardIdsRef.current = '';
        setCategories([]);
    }, [session.doReset]);

    return { categories, loading };
};

const updateSessionLineData = (checkedCategories: string[], fetchLinkLinesMap: Map<string, FetchLinkLines>, session: Session): any => {
    return async () => {
        const newLines: LinkLines = {};
        for (const category of checkedCategories) {
            const datas = await fetchLinkLinesMap.get(category)?.(session);
            if (datas === undefined) {
                return;
            }
            newLines[category] = datas;
        }
        Object.values(session.singleLinkLine)
            .forEach(datas => {
                datas?.forEach((data) => {
                    const { category } = data as unknown as FlowEvent;
                    if (!checkedCategories.includes(category)) {
                        newLines[category] = session.singleLinkLine[category];
                    }
                });
            });
        runInAction(() => {
            session.linkLines = newLines;
            session.renderTrigger = !session.renderTrigger;
            session.linkLineCategories = checkedCategories;
        });
    };
};

const LinkLineFilterBody = observer(({ session, isSuspend }: { session: Session; isSuspend: boolean }): JSX.Element => {
    let { categories: displayCategories, loading } = useGetCategories(session, isSuspend);
    const [checkedCategories, setCheckedCategories] = React.useState<string[]>([]);
    const [inputValue, setInput] = React.useState<string>();
    const fetchLinkLinesMap = useFetchLinkLines(checkedCategories, session.viewedCardIdSet);
    const isEmptyData = displayCategories.length === 0;
    const updateLinkLines = React.useCallback(updateSessionLineData(checkedCategories, fetchLinkLinesMap, session),
        [checkedCategories, session.viewedCardIdSet]);
    const onInputChange = action((e: ChangeEvent<HTMLInputElement>): void => {
        const inputContent = e.target.value;
        const trimmedValue = inputContent.trim();
        setInput(trimmedValue);
    });
    if (inputValue !== undefined && inputValue.length > 0) {
        displayCategories = displayCategories.filter(str => str.toLowerCase().includes(inputValue.toLowerCase()));
    }
    const dependencyParam = [session.domainRange.domainStart,
        session.domainRange.domainEnd,
        checkedCategories,
        session?.unitsConfig.offsetConfig.timestampOffset,
        session.viewedCardIdSet];
    const updateLineData = (): void => {
        updateLinkLines();
    };
    React.useEffect(updateLineData, dependencyParam);
    React.useEffect(() => {
        setCheckedCategories([]);
    }, [session.doReset]);
    return (
        <Spin spinning={loading} delay={400}>
            <FilterContainer>
                <Input size="middle" allowClear onChange={onInputChange} style={{ width: '100%', marginBottom: 10 }}></Input>
                <FilterList>
                    {isEmptyData
                        ? <StyledEmpty />
                        : displayCategories.map((category, index) => <FilterItem
                            key={index}
                            category={category}
                            checkedCategories={checkedCategories}
                            setCheckedCategories={setCheckedCategories}
                        />)}
                </FilterList>
                {!isEmptyData && <FilterButtonLine>
                    <Button type={'primary'} onClick={(): void => setCheckedCategories([...displayCategories])}>
                        {i18n.t('timeline:All')}
                    </Button>
                    <Button onClick={((): void => setCheckedCategories([]))}>
                        {i18n.t('timeline:None')}
                    </Button>
                </FilterButtonLine>}
            </FilterContainer>
        </Spin>
    );
});

export const FilterLinkLine = observer(({ session }: { session: Session}): JSX.Element | null => {
    const { t } = useTranslation();
    const [customButtonProps, updateCustomButtonProps] = useState({
        isEmphasize: false,
        isSuspend: false,
    });
    // tooltip显隐控制悬浮效果
    const onTooltipVisibleChange = (visible: boolean): void => {
        updateCustomButtonProps({ ...customButtonProps, isSuspend: visible });
    };
    const ref = useRef<HTMLButtonElement>(null);
    return (
        <Tooltip overlayStyle={{ maxWidth: 1000 }}
            title={<LinkLineFilterBody session={session} isSuspend={customButtonProps.isSuspend} />}
            trigger="click"
            onOpenChange={onTooltipVisibleChange}
            overlayInnerStyle={{ borderRadius: 2 }}
            overlayClassName={'insight-category-search-overlay'} align={{ offset: [-8, 3] }}>
            <CustomButton data-testid={'tool-flow'} tooltip={t('tooltip:linker')} icon={LinkIcon as any} { ...customButtonProps } ref={ref}/>
        </Tooltip>
    );
});
