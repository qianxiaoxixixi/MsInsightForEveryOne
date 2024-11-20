/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import {
  JupyterFrontEnd,
  JupyterFrontEndPlugin
} from '@jupyterlab/application';

import { requestAPI } from './handler';

/**
 * Initialization data for the jupyterlab_mindstudio_insight extension.
 */
const plugin: JupyterFrontEndPlugin<void> = {
  id: 'jupyterlab_mindstudio_insight:plugin',
  autoStart: true,
  activate: (app: JupyterFrontEnd) => {\
    requestAPI<any>('get_example')
      .then(data => {
      })
      .catch(reason => {
      });
  },
};

export default plugin;
