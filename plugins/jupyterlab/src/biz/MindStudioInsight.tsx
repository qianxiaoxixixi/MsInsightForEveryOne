/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import React, { useState, useEffect, useRef } from 'react';
import { MindStudioManager } from '../manager';
import * as MindStudio from '../mindstudio';

interface MindStudioInsightProps {
    setWidgetName?: (name: string) => void;
    createdModelName?: string;
    mindstudioManager: MindStudioManager;
    closeWidget: () => void;
    openMindStudio: (modelName: string) => void;
    openDoc: () => void;
    updateCurrentModel: (model: MindStudio.IModel | null) => void;
    startNew: (name: string, options?: MindStudio.IOptions) => Promise<MindStudio.IMindStudio>;
}

export const MindStudioInsightTab = (props: MindStudioInsightProps): JSX.Element => {
    const [currentMindStudio, setCurrentMindStudio] = useState<MindStudio.IModel | null>(null);
    const currentMindStudioRef = useRef(currentMindStudio);
    // currently inactive
    const [notActiveError, setNotActiveError] = useState(false);
    const [srcToken, setSrcToken] = useState('');
    const [srcVal, setSrcVal] = useState('');
    const [isTokenPageVisible, setIsTokenPageVisible] = useState(true);

    const updateCurrentMindStudio = (model: MindStudio.IModel | null): void => {
        props.updateCurrentModel(model);
        setCurrentMindStudio(model);
        currentMindStudioRef.current = model;
    };

    const refreshRunning = async (): Promise<void> => {
        await props.mindstudioManager.refreshRunning();
        const runningMindStudios = [...props.mindstudioManager.running()];

        // hint: Using runningMindStudios directly may cause setState to fail to respond
        const modelList = [];
        for (const model of runningMindStudios) {
            modelList.push(model);
        }

        if (currentMindStudioRef.current) {
            if (!modelList.find(model => model.name === currentMindStudioRef.current?.name)) {
                setNotActiveError(true);
            }
        }

        const model = props.createdModelName
            ? modelList.find(modelItem => modelItem.name === props.createdModelName)
            : null;
        if (model) {
            updateCurrentMindStudio(model);
            if (props.setWidgetName) {
                props.setWidgetName(model.name);
            }
        }
    };

    const getToken = async (): Promise<void> => {
        const savedToken = sessionStorage.getItem('mindstudio-insight-token');
        if (savedToken) {
            setSrcToken(savedToken);
            setIsTokenPageVisible(false);
        } else {
            await MindStudio.getToken();
            setIsTokenPageVisible(true);
        }
    };

    const handleTokenSubmit = async (token: string): Promise<void> => {
        const genToken = await MindStudio.getToken();
        if (genToken === token) {
            setSrcToken(token);
            sessionStorage.setItem('mindstudio-insight-token', token);
        }
        setIsTokenPageVisible(false);
        await getUrl(token);
    };

    const getUrl = async (token: string): Promise<void> => {
        const val = await MindStudio.getUrl('', token);
        setSrcVal(val);
    };

    useEffect(() => {
        if (srcToken) {
            getUrl(srcToken);
        }
    }, [srcToken]);

    useEffect(() => {
        getToken();
    }, []);

    useEffect(() => {
        refreshRunning();
    }, []);

    return (
        <div style={{ width: '100%', height: '100%' }}>
            { isTokenPageVisible ? (
                <div style={{
                    display: 'flex',
                    flexDirection: 'column',
                    alignItems: 'center',
                    height: '100vh',
                    textAlign: 'center',
                }}>
                    {/* Token 输入界面 */}
                    <h3>Please enter your token, which you can get it from the jupyterlab service startup</h3>
                    <div style={{
                        display: 'flex',
                        alignItems: 'center',
                    }}>
                        <input
                            type="text"
                            value={srcToken}
                            onChange={(e): void => setSrcToken(e.target.value)}
                            placeholder="Enter Token"
                            style={{
                                marginRight: '10px',
                                padding: '8px',
                                width: '200px',
                            }}
                        />
                        <button
                            onClick={(): Promise<void> => handleTokenSubmit(srcToken)}
                            style={{
                                padding: '8px 16px',
                                backgroundColor: '#007bff',
                                color: 'white',
                                border: 'none',
                                borderRadius: '4px',
                                cursor: 'pointer',
                            }}
                        >
                            Submit Token
                        </button>
                    </div>
                </div>
            ) : (
                <div style={{ width: '100%', height: '100%' }}>
                    { !currentMindStudio && (
                    <iframe
                        style={{ width: '100%', height: '100%' }}
                        sandbox="allow-scripts allow-forms allow-same-origin"
                        referrerPolicy="no-referrer"
                        src={srcVal}
                    />
                    ) }
                    { currentMindStudio && (
                    <div>
                        <p>
                            No instance for current directory yet, please create a new MindStudio Insight.
                        </p>
                    </div>
                    ) }
                    { notActiveError && (
                    <div>
                        <p>
                        Current Tensorboard is not active. Please select others or create a new one.
                        </p>
                    </div>
                    ) }
                </div>
            )}
        </div>
    );
};