/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import React, { useEffect, useState } from 'react';
import { Select } from 'ascend-components';
import MemoryBarChart from './MemoryBarChart';
import { Label } from './Common';
import { useTranslation } from 'react-i18next';
import { GetLeaksGraphData } from '../utils/RequestUtils';
import { message } from 'antd';
import type { BlockData, AllocationData } from '../utils/RequestUtils';
import { observer } from 'mobx-react';

const MemoryStack = observer(({ session, isDark }: { session: any; isDark: boolean }): React.ReactElement => {
    const { t } = useTranslation('leaks');
    const [options, setOptions] = useState([]);
    const [deviceId, setDeviceId] = useState('');
    const [blockData, setBlockData] = useState<BlockData>();
    const [allocationData, setAllocationData] = useState<AllocationData>();
    const init = async (): Promise<void> => {
        try {
            const blockDatas = await GetLeaksGraphData({ deviceId, graph: 'blocks' });
            const allocationDatas = await GetLeaksGraphData({ deviceId, graph: 'allocations' });
            setBlockData(blockDatas as BlockData);
            setAllocationData(allocationDatas as AllocationData);
        } catch (error: any) {
            message.error(error.message);
        }
    };
    useEffect(() => {
        if (deviceId !== '') {
            init();
        }
    }, [deviceId]);
    useEffect(() => {
        const opts = session.deviceIds.map((id: string) => ({ label: id, value: id }));
        if (opts.length) {
            setOptions(opts);
            setDeviceId(opts[0].value);
        }
    }, [session.deviceIds]);
    return (
        <div>
            <div style={{ marginLeft: 24, marginTop: 24 }}>
                <Label name={t('DeviceID')} />
                <Select
                    id={'select-host'}
                    value={deviceId}
                    size="middle"
                    onChange={(value): void => { setDeviceId(value); }}
                    options={options}
                />
            </div>
            <MemoryBarChart blockData={blockData} allocationData={allocationData} isDark={isDark} />
        </div>
    );
});

export default MemoryStack;
