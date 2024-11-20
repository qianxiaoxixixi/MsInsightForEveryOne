/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

import { URLExt } from '@jupyterlab/coreutils';
import { ServerConnection } from '@jupyterlab/services';

/**
 * Call the API extension
 *
 * @param endPoint API REST end point for the extension.
 * @param init Initial values for the request.
 * @returns The response body interpreted as JSON.
 */
export async function requestAPI<T>(
  endPoint = '',
  init: RequestInit = {}
): Promise<T> {
  // Make request to Jupyter API
  const settings = ServerConnection.makeSettings();
  // Set request url
  const requestUrl = URLExt.join(
    settings.baseUrl,
    'jupyterlab-mindstudio-insight', // API Namespace
    endPoint
  );

  // 获取server connection并校验
  let response: Response;
  try {
    response = await ServerConnection.makeRequest(requestUrl, init, settings);
  } catch (error) {
    throw new ServerConnection.NetworkError(error);
  }
  
  // 获取后端返回json数据进行处理
  let data: any = await response.text();
  if (data.length > 0) {
    try {
      data = JSON.parse(data);
    } catch (error) {
      data = {};
    }
  }

  // 抛出后端返回异常
  if (!response.ok) {
    throw new ServerConnection.ResponseError(response, data.message || data);
  }

  return data;
}
