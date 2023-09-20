/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import React from 'react';

export const Label = (props: {name: string;style?: object }): JSX.Element => {
    return <span style={{ margin: '0 10px', ...(props.style ?? {}) }}>{props.name ? props.name + ' :' : ''} </span>;
};
