/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { observer } from 'mobx-react';
import React, { useEffect, useState } from 'react';
import { Select } from 'antd';
import { Label, MultiSelectWithAll } from '../communicationAnalysis/Common';

export interface ConditionDataType {
    step: string | number;
    rankIds: string[];
    order: string | number;
    top: number;
}

interface optionDataType{
    key?: string;
    label: React.ReactNode;
    value: string | number ;
}

interface optionMapDataType{
    [props: string]: optionDataType[];
}

const Filter = observer((props: any) => {
    const { handleFilterChange } = props;
    // 获取可选项
    useEffect(() => {
        getOptions();
    }, []);
    const [ options, setOptions ] = useState<optionMapDataType>({});
    const getOptions = (): void => {
        const steplist = [ 'All', 0, 1, 2, 3, 4, 5 ];
        const stepOptions: optionDataType[] = steplist.map(item => ({ value: item, label: item }));
        const rankIds = [ 0, 1, 2, 3 ];
        const rankIdOptions: optionDataType[] = rankIds.map(item => ({ value: item, label: item }));
        const orderlist = [ 'Computing', 'Communication(Not Overlapped)', 'Communication(Overlapped)', 'Free' ];
        const orderOptions: optionDataType[] = orderlist.map(item => ({ value: item, label: item }));
        const toplist = [ 1, 2, 4, 8, 16, 32 ];
        const topOptions: optionDataType[] = toplist.map(item => ({ value: item, label: item }));
        setOptions({ stepOptions, topOptions, rankIdOptions, orderOptions });
    };
    // 筛选条件变化
    const [ conditions, setConditions ] = useState<ConditionDataType>(
        { step: 'All', rankIds: [], order: 'Computing', top: 4 });
    const handleChange = (prop: keyof ConditionDataType, val: string | number | string[]): void => {
        setConditions({ ...conditions, [prop]: val });
    };
    useEffect(() => {
        handleFilterChange(conditions);
    }, [conditions]);

    return (<FilterCom conditions={conditions} handleChange={handleChange} options={options} />);
});

const FilterCom = (props: any): JSX.Element => {
    const { conditions, handleChange = [], options = {} } = props;
    return (<div style={ { margin: '0 20px 10px' }}>
        <Label name="Step" />
        <Select
            value={conditions.step}
            style={{ width: 120 }}
            onChange={val => handleChange('step', val)}
            options={options.stepOptions}
        />
        <Label name="Rank ID"/>
        <MultiSelectWithAll
            value={conditions.rankIds}
            onChange={(val: any) => handleChange('rankIds', val)}
            options={options.rankIdOptions}
            style={{ width: 150 }}
            maxTagCount={1}
        />
        <Label name="Order By"/>
        <Select
            value={conditions.order}
            style={{ width: 280 }}
            onChange={val => handleChange('order', val)}
            options={options.orderOptions}
        />
        <Label name="Top"/>
        <Select
            value={conditions.top}
            style={{ width: 100 }}
            onChange={val => handleChange('top', val)}
            options={options.topOptions}
        />
    </div>);
};

export default Filter;
