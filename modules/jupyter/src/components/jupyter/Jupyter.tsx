/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
import React from 'react';
import { observer } from 'mobx-react';
import type { Session } from '../../entity/session';
import './Jupyter.css';
import { loading } from '../Common';
import { useTranslation } from 'react-i18next';

const index = observer((props: { session: Session }) => {
    const { session } = props;
    const { t } = useTranslation('jupyter');
    if (session.isIpynb && session.ipynbUrl !== '') {
        return <div className="jupyter-div">
            <iframe id="jupyter" className="jupyter-iframe" src={session.ipynbUrl}></iframe>
            <div className="jupyter-tip">{t('Jupyter Tip')}: {session.ipynbUrl}</div>
        </div>;
    } else {
        return loading;
    }
});

export default index;
