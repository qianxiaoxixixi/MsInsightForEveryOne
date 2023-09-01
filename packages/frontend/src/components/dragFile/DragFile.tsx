/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */
import React, { useEffect } from 'react';
import { FolderFilled } from '@ant-design/icons';
import { notification, Progress } from 'antd';
export const DragFileInit = (id: string): void => {
    const dropZone = document.getElementById(id);
    if (dropZone) {
        dropZone.addEventListener('dragenter', function (e: any) {
            e.preventDefault();
            e.stopPropagation();
        }, false);

        dropZone.addEventListener('dragover', function (e: any) {
            e.preventDefault();
            e.stopPropagation();
            e.target.classList.add('fileshover');
        }, false);

        dropZone.addEventListener('dragleave', function (e: any) {
            e.preventDefault();
            e.stopPropagation();
            e.target.classList.remove('fileshover');
        }, false);
        // 处理拖拽文件
        dropZone.addEventListener('drop', function (e: any) {
            e.preventDefault();
            e.stopPropagation();
            e.target.classList.remove('fileshover');
            handleFile(e.dataTransfer.items);
        }, false);
    }
};

async function handleFile(items: DataTransferItemList): Promise<void> {
    const allFiles = await getFilelist(items);
    allFiles.forEach((fileinfo: UploadFileDataType) => {
        readFile(fileinfo);
    });
}
interface UploadFileDataType{
    file: any;// 二进制流
    name: string;// 文件名
    path: string;// 文件路径
    progress?: number;// 上传进度
    status?: 'normal' | 'exception' | 'active' | 'success' | undefined;// 上传状态
}

function readFile(fileInfo: any): void {
    const { file, name, path } = fileInfo;
    const reader = new FileReader();
    // 二进制流
    reader.readAsArrayBuffer(file);
    reader.onload = event => {
        const buffer: any = event.target?.result;
        if (buffer !== null && buffer !== undefined) {
            notification.success({
                message: 'Start Reading:',
                description: '【File】' + fileInfo.path,
                duration: null,
            });
            window.request('towingImport/action',
                { isBinary: true, buffer, params: { name, path } }).then(res => {
                notification.success({
                    message: 'Read File Succeed:',
                    description: '【File】' + fileInfo.path,
                    duration: null,
                });
            }).catch(error => {
                notification.error({
                    message: 'Read File Failed:',
                    description: <div>
                        <div>{error.message}</div>
                        <div>{'【File】' + fileInfo.path}</div></div>,
                    duration: null,
                });
            });
        }
    };
}

async function getFilelist(items: any): Promise<any> {
    return new Promise((resolve, reject) => {
        const list = [];
        for (const item of items) {
            if (item.kind === 'file') {
                const entry = item.webkitGetAsEntry();
                list.push(getFileFromEntryRecusively(entry));
            }
        }
        Promise.all(list).then(values => {
            resolve(values.flat(1));
        });
    });
}

async function getFileFromEntryRecusively(entry: any): Promise<any> {
    return new Promise((resolve, reject) => {
        if (entry.isFile === true) {
            entry.file((file: any) => {
                const fileInfo = { file, path: entry.fullPath.slice(1), name: file.name };
                resolve(fileInfo);
            }, (e: any) => {
                console.log(e);
            });
        } else {
            const reader = entry.createReader();
            reader.readEntries(
                (entries: any) => {
                    entries.forEach((entry: any) => getFileFromEntryRecusively(entry));
                    Promise.all(entries.map((entry: any) => getFileFromEntryRecusively(entry))).then(values => {
                        resolve(values.flat(1));
                    });
                },
                (e: any) => {
                    console.log(e);
                },
            );
        }
    });
}

const DragFile: React.FC = () => {
    const id = 'drag';
    useEffect(() => {
        DragFileInit(id);
    }, []);

    const openSelect = async(): Promise<void> => {};
    return (
        <div className={'drag-zone'} onClick={() => openSelect()} id={id}>
            <FolderFilled className={'drag-zone-icon'}/>
            <div>点击或拖拽文件到此</div>
        </div>
    );
};

export const FileUploadList: React.FC<{fileList: UploadFileDataType[]}> = ({ fileList }) => {
    return (
        <div >
            {
                fileList.map((file: UploadFileDataType, index: number) => (
                    <div key={index}>
                        <span>{file.name}</span><Progress percent={file.progress} status={file.status}/>
                    </div>
                ))
            }
        </div>
    );
};

export default DragFile;
