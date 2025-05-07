/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
import React, { forwardRef } from 'react';
import { Tree, type TreeProps } from 'antd';
import type RcTree from 'rc-tree';
import styled from '@emotion/styled'; import type { BasicDataNode } from 'rc-tree';
import type { DataNode } from 'rc-tree/lib/interface';

const InternalMITree = forwardRef(function MITree<T extends BasicDataNode | DataNode = DataNode>(props: React.PropsWithChildren<TreeProps<T>> & {
    ref?: React.Ref<RcTree>;
}) {
    return (
        <Tree {...props} />
    );
});

const StyledMITree = styled(InternalMITree)`
    color: ${(props): string => props.theme.textColorPrimary};
    background: ${(props): string => props.theme.bgColorDark};
    font-size: 14px;
`;

// 强制类型断言，使 StyledMITree 支持泛型 T
type GenericTreeComponent = <T extends BasicDataNode | DataNode>(
    props: React.PropsWithChildren<TreeProps<T>> & { ref?: React.Ref<RcTree> }
) => JSX.Element;

export const MITree = StyledMITree as GenericTreeComponent;
