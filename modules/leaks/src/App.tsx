/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
import { observer } from 'mobx-react';
import React, { useEffect } from 'react';
import { ThemeProvider } from '@emotion/react';
import { useRootStore } from './context/context';
import connector from './connection';
import Leaks from './components/leaks/leaks';
import { themeInstance } from './theme/theme';
import { registerEventHandlers } from './index';
import { GlobalStyles } from 'ascend-theme';
const App = observer(() => {
    const { sessionStore } = useRootStore();
    let session = sessionStore.activeSession;
    const getLanguage = (): void => {
        connector.send({
            event: 'getLanguage',
        });
    };
    useEffect(() => {
        session = sessionStore.activeSession;
        connector.send({ event: 'getParseStatus', body: {} });
        registerEventHandlers();
        getLanguage();
    }, []);
    return <ThemeProvider theme={themeInstance.getThemeType()}>
        <GlobalStyles />
        {session !== undefined ? <Leaks session={session} isDark={themeInstance.getCurrentTheme() === 'dark'}/> : <></>}
    </ThemeProvider>;
});

export default App;
