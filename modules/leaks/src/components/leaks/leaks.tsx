/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
import React from 'react';
import { observer } from 'mobx-react';
import { Session } from '../../entity/session';
import { Layout } from 'ascend-layout';
import MemoryStack from '../MemoryStack';

const index = observer((props: { session: Session; isDark: boolean }) => {
    const { session, isDark } = props;
    return (<Layout> <MemoryStack session={session} isDark={isDark} /> </Layout>);
});

export default index;
