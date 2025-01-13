/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import React, { useEffect } from 'react';
import { observer } from 'mobx-react-lite';
import { runInAction } from 'mobx';
import { SearchBox, FlexDiv } from '../utils/styleUtils';
import { MemoryHeaderStrategy } from '../utils/strategyUtils';
import { Session } from '../entity/session';
import { MemorySession, GroupBy } from '../entity/memorySession';
import { Label, useHit } from './Common';
import { Select } from 'ascend-components';
import { GroupRankIdsByHost } from 'ascend-utils';
import { useTranslation } from 'react-i18next';

const groupByOptions = (isCompare: boolean): Array<{ label: string; value: string }> => {
    const options = [
        { label: 'Overall', value: 'Overall' },
        { label: 'Stream', value: 'Stream' },
        { label: 'Component', value: 'Component' },
    ];
    if (isCompare) {
        options.splice(1, 1);
    }
    return options;
};

const MemoryHeader = observer(({ strategy, session, memorySession }:
{strategy: MemoryHeaderStrategy; session: Session; memorySession: MemorySession}) => {
    const isCompare: boolean = session.compareRank.isCompare;
    const { t } = useTranslation('memory');
    const hit = useHit();
    const groupByCondition = groupByOptions(isCompare).map(item => ({
        ...item,
        label: t(`searchCriteria.${item.label}`),
    }));

    const onHostChanged = (value: string): void => {
        runInAction(() => {
            memorySession.hostCondition = { ...memorySession.hostCondition, value };
        });
    };

    const onRankIdChanged = (value: string): void => {
        runInAction(() => {
            memorySession.rankIdCondition = { ...memorySession.rankIdCondition, value };
        });
    };

    const onGroupByChanged = (value: string): void => {
        runInAction(() => {
            memorySession.groupId = value;
        });
    };

    useEffect(() => {
        const { hosts, ranks } = GroupRankIdsByHost(session.memoryRankIds);
        runInAction(() => {
            memorySession.hostCondition = { options: hosts, value: hosts[0] ?? '', ranks };
        });
    }, [session.memoryRankIds.join('')]);

    useEffect(() => {
        // 只对RankId为数字做排序，不能转为数字的字符串则不排序
        const rankIdOptions: string[] = JSON.parse(JSON.stringify(memorySession.hostCondition.ranks?.get(memorySession.hostCondition.value) ?? []))
            .sort((a: any, b: any) => Number(a) - Number(b));
        if (rankIdOptions.length === 0) {
            runInAction(() => {
                memorySession.rankIdCondition = { options: [], value: '' };
            });
            return;
        }
        const rankIdValue = (memorySession.rankIdCondition.value === undefined || memorySession.rankIdCondition.value === '')
            ? rankIdOptions[0]
            : memorySession.rankIdCondition.value;
        runInAction(() => {
            memorySession.rankIdCondition = { options: rankIdOptions, value: rankIdValue };
        });
    }, [memorySession.hostCondition.options, memorySession.hostCondition.value, memorySession.hostCondition.ranks]);

    useEffect(() => {
        runInAction(() => {
            memorySession.rankIdCondition = { options: memorySession.rankIdCondition.options, value: memorySession.rankIdCondition.options[0] };
        });
    }, [session.isClusterMemoryCompletedSwitch]);

    useEffect(() => {
        if (session.compareRank.rankId === memorySession.rankIdCondition.value) {
            return;
        }
        if (session.memoryRankIds.includes(session.compareRank.rankId)) {
            onRankIdChanged(session.compareRank.rankId);
        }
    }, [session.compareRank.rankId, session.memoryRankIds.join('')]);

    useEffect(() => {
        if (isCompare && memorySession.groupId === GroupBy.STREAM) {
            runInAction(() => {
                memorySession.groupId = GroupBy.DEFAULT;
            });
        }
    }, [isCompare]);
    const renderFields = (): JSX.Element[] => {
        const fields = [
            {
                key: 'host',
                element:
                    <FlexDiv>
                        <Label name={t('searchCriteria.Host')} />
                        <Select
                            id={'select-host'}
                            value={memorySession.hostCondition.value}
                            size="middle"
                            onChange={onHostChanged}
                            disabled={isCompare}
                            options={memorySession.hostCondition.options.map((host: string) => {
                                return { value: host, label: host };
                            })}
                        />
                    </FlexDiv>,
            },
            {
                key: 'rankId',
                element:
                    <FlexDiv>
                        <Label name={t('searchCriteria.RankId')} />
                        <Select
                            id={'select-rankId'}
                            value={memorySession.rankIdCondition.value}
                            size="middle"
                            onChange={onRankIdChanged}
                            disabled={isCompare}
                            options={memorySession.rankIdCondition.options.map((rankId: string) => {
                                return {
                                    value: rankId,
                                    label: rankId.replace(`${memorySession.hostCondition.value} `, ''),
                                };
                            })}
                        />
                    </FlexDiv>,
            },
            {
                key: 'groupId',
                element:
                    <FlexDiv>
                        <Label name={<span>{t('searchCriteria.GroupBy')}{hit}</span>} />
                        <Select
                            id={'select-groupId'}
                            value={memorySession.groupId}
                            style={{ width: 180 }}
                            onChange={onGroupByChanged}
                            options={groupByCondition}
                        />
                    </FlexDiv>,
            },
        ];
        return fields
            .filter((field) => strategy.shouldDisplay(field.key))
            .map((field) => field.element);
    };

    return (
        <div className="mb-30">
            <SearchBox>{renderFields()}</SearchBox>
        </div>
    );
});

export default MemoryHeader;
