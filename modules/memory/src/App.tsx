/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import { ThemeProvider } from '@emotion/react';
import styled from '@emotion/styled';
import { observer } from 'mobx-react';
import React, { useEffect, useState, useRef } from 'react';
import { useRootStore } from './context/context';
import { themeInstance } from './theme/theme';
import MemoryAnalysis from './pages/MemoryAnalysis';
import connector from './connection';
import { getSearchParams } from './utils/localUrl';
import i18n from './i18n';

const ImgWithFallback = ({
    className = '',
}): JSX.Element => {
    const PictureContainer = styled.picture`
      img {
        height: 48px;
        width: 48px;
      }
    `;
    return (
        <PictureContainer>
            <div className={className}></div>
        </PictureContainer>
    );
};

const StatePopover = observer(() => {
    const Mask = styled.div`
      position: absolute;
      display: flex;
      z-index: 4;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      width: 100%;
      height: 100%;
      justify-content: center;
      align-items: center;
    `;
    const Info = styled.div`
      border-radius: 4px;
      position: relative;
      display: flex;
      align-items: center;
      width: 16rem;
      height: 3rem;
      font-size: 1.12rem;
      .img {
        margin-right: 10px;
      }
    `;
    return <Mask>
        <Info className={'info'}><ImgWithFallback className={'loading'}/>waiting...</Info>
    </Mask>;
});

export const App = observer(() => {
    const { insightStore, sessionStore } = useRootStore();
    const [ themeDark, setThemeDark ] = useState<boolean>(true);
    const hasListenerRef = useRef<boolean>(false);
    let session = sessionStore.activeSession;
    const lang = getSearchParams('language');
    useEffect(() => {
        insightStore.loadTemplates().then(() => {
            session = sessionStore.activeSession;
        });
        i18n.changeLanguage(lang === 'zh' ? 'zh' : 'en');
        themeInstance.setCurrentTheme('dark');
        window.setTheme(true);
    }, []);

    !hasListenerRef.current && connector.addListener('setTheme', (e) => {
        hasListenerRef.current = true;
        setThemeDark(e.data.body.isDark);
    });

    return (
        <ThemeProvider theme={themeInstance.getThemeType()}>
            {session !== undefined ? <MemoryAnalysis session={session} isDark={themeDark} /> : <StatePopover/>}
        </ThemeProvider>
    );
});
