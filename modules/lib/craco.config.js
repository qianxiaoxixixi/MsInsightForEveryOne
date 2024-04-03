/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/
const {ModuleFederationPlugin} = require('webpack').container;
module.exports = {
  devServer: {
    port: 8000,
  },
  webpack: {
    configure: webpackConfig => {
      webpackConfig.plugins.push(new ModuleFederationPlugin({
        filename: 'remoteEntry.js',
        name: 'lib',
        exposes: {
          './resize': './src/resize',
          './Simple': './src/Simple',
        },
        shared: {
          react: {
            singleton: true,
            eager: true,
          },
          'react-dom': {
            singleton: true,
            eager: true,
          },
        },
      }));
      webpackConfig.output.publicPath = 'auto';
      return webpackConfig;
    },
    alias: {
      react: '@cloudsop/horizon', // 新增
      'react-dom/client': '@cloudsop/horizon', // 兼容react18的用法
      'react-dom': '@cloudsop/horizon', // 新增
      'react-is': '@cloudsop/horizon', // 新增
    },
  },
};
