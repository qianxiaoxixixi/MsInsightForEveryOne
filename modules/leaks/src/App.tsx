/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
import { observer } from 'mobx-react';
import React, { useEffect } from 'react';
import { ThemeProvider } from '@emotion/react';
import { useRootStore } from './context/context';
import Leaks from './components/leaks/leaks';
import { themeInstance } from './theme/theme';
import { registerEventHandlers, getInitStatus } from './index';
import { GlobalStyles } from 'ascend-theme';
const App = observer(() => {
    const { sessionStore } = useRootStore();
    let session = sessionStore.activeSession;
    useEffect(() => {
        session = sessionStore.activeSession;
        registerEventHandlers();
        getInitStatus();
    }, []);
    return <ThemeProvider theme={themeInstance.getThemeType()}>
        <GlobalStyles />
        {session !== undefined ? <Leaks session={session} /> : <></>}
    </ThemeProvider>;
});

export default App;
