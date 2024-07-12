/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

export interface IMessageSender {
    sendMessage: (ceq: any) => void;
    selectFolder: () => Promise<string>;
    selectFile: () => Promise<string>;
}

export const removeAndAddEventListener = (resolve: (value: (string | PromiseLike<string>)) => void): void => {
    function onMessage(event: MessageEvent): void {
        const message = event.data;
        switch (message.command) {
            case 'ascend.folderSelected':
                resolve(message.path);
                break;
            case 'ascend.folderSelectionCanceled':
                resolve('');
                break;
            default:
        }
    }
    window.removeEventListener('message', onMessage);
    window.addEventListener('message', onMessage);
};
