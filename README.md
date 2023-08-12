# Server

## 构建说明

### 构建工具

1.cmake
https://clouddragon.huawei.com/cloudbuild/project/g05eddbe2f77d400ba43b805e8bca793a/buildToolManage/buildToolDetails?versionId=0591A8KY
2.mingw-w64（windows平台）
https://clouddragon.huawei.com/cloudbuild/project/g05eddbe2f77d400ba43b805e8bca793a/buildToolManage/buildToolDetails?versionId=0591A9HS
3.python3
https://clouddragon.huawei.com/cloudbuild/project/g05eddbe2f77d400ba43b805e8bca793a/buildToolManage/buildToolDetails?versionId=0591A9ND
4.ninja（linux平台）
https://clouddragon.huawei.com/cloudbuild/project/g05eddbe2f77d400ba43b805e8bca793a/buildToolManage/buildToolDetails?versionId=0591A6QK
5.xcode（mac平台）
https://clouddragon.huawei.com/cloudbuild/project/g05eddbe2f77d400ba43b805e8bca793a/buildToolManage/buildToolDetails?versionId=0591A7GJ



### 开源代码

群空间下载开源代码， 解压覆盖到thirdparty目录

https://onebox.huawei.com/p/7606b4ccd02cec637bc5f518c156a184



### 构建

```
cd build
python build.py build --release
```

只构建中心server

~~~
python build.py build --release --project_type=CentricServer
~~~

只构建边缘server

~~~
python build.py build --release --project_type=EdgeServer
~~~

