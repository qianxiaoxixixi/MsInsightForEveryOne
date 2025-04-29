/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */
#include "ExpertHotspotManager.h"
#include "ExpertHotspotParser.h"
#include "DataBaseManager.h"
#include "ClusterDef.h"
#include "CollectionUtil.h"

namespace Dic {
namespace Module {
namespace Summary {
bool ExpertHotspotManager::InitExpertHotspotData(const std::string &filePath, const std::string &version,
                                                 std::string &errorMsg)
{
    // 参数校验，ConvertToRealPath方法中会调用CheckDirValid方法对文件进行校验
    std::string realFilePath = filePath;
    if (!FileUtil::ConvertToRealPath(errorMsg, realFilePath)) {
        return false;
    }
    // 查找文件列表
    auto hotspotFiles = FileUtil::FindAllFilesByRegex(realFilePath, std::regex(expertHotspotFileReg));
    if (hotspotFiles.empty()) {
        errorMsg = "No parsable files found";
        return false;
    }
    // 获取db
    auto database = Timeline::DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    if (database == nullptr) {
        errorMsg = "Cluster database is not exist.";
        return false;
    }
    // 清空老数据
    if (!database->DeleteExpertHotspot("", version)) {
        errorMsg = "Failed to clear old expert hotspot data, version:" + version;
        return false;
    }
    ExpertHotspotParser parser(database);
    for (const auto &item: hotspotFiles) {
        // 文件解析，单个文件解析失败不影响最终结果
        if (!parser.Parse(item, version)) {
            ServerLog::Warn("Fail to parser file:", item);
        }
    }
    database->SaveExpertHotspot();
    auto modelInfoList = parser.GetModelInfoList();
    if (!modelInfoList.empty()) {
        SaveModelInfo(modelInfoList.begin()->second, database);
    }
    return true;
}

bool ExpertHotspotManager::SaveModelInfo(const ModelInfo &modelInfo, std::shared_ptr<VirtualClusterDatabase> &db)
{
    std::map<std::string, std::string> modelInfoMap;
    // 只有不为空时才执行插入操作
    if (!modelInfo.denseLayerList.empty()) {
        modelInfoMap[KEY_DENSE_LAYER_LIST] = StringUtil::join(modelInfo.denseLayerList, ",");
    }
    if (modelInfo.moeLayer != 0) {
        modelInfoMap[KEY_MOE_LAYER] = std::to_string(modelInfo.moeLayer);
    }
    if (modelInfo.rankNumber != 0) {
        modelInfoMap[KEY_RANK_NUMBER] = std::to_string(modelInfo.rankNumber);
    }
    if (modelInfo.expertNumber != 0) {
        modelInfoMap[KEY_EXPERT_NUMBER] = std::to_string(modelInfo.expertNumber);
    }
    return db->InsertDuplicateUpdateBaseInfo(modelInfoMap);
}

ModelInfo ExpertHotspotManager::GetModelInfo(std::shared_ptr<VirtualClusterDatabase> &db)
{
    std::vector<std::string> keys = {KEY_DENSE_LAYER_LIST, KEY_MOE_LAYER, KEY_RANK_NUMBER, KEY_EXPERT_NUMBER};
    std::map<std::string, std::string> modelInfoMap = db->QueryBaseInfoByKeys(keys);
    std::string defaultZeroStr = "0";
    ModelInfo modelInfo;
    std::string denseLayerListStr = CollectionUtil::FindValueByKey(modelInfoMap, KEY_DENSE_LAYER_LIST,
                                                                   CollectionUtil::EMPTY_STRING);
    for (const auto &item: StringUtil::Split(denseLayerListStr, ",")) {
        modelInfo.denseLayerList.push_back(StringUtil::StringToInt(item));
    }
    modelInfo.expertNumber = StringUtil::StringToInt(
        CollectionUtil::FindValueByKey(modelInfoMap, KEY_EXPERT_NUMBER, defaultZeroStr));
    modelInfo.rankNumber = StringUtil::StringToInt(
        CollectionUtil::FindValueByKey(modelInfoMap, KEY_RANK_NUMBER, defaultZeroStr));
    modelInfo.moeLayer = StringUtil::StringToInt(
        CollectionUtil::FindValueByKey(modelInfoMap, KEY_MOE_LAYER, defaultZeroStr));
    return modelInfo;
}

bool ExpertHotspotManager::FillExpertIdInfo(std::vector<ExpertHotspotStruct> &hotspotInfos,
    const ModelInfo &modelInfo, const std::vector<ExpertDeploymentStruct> &deployment)
{
    if (deployment.empty()) {
        if (modelInfo.rankNumber == 0 || modelInfo.expertNumber % modelInfo.rankNumber != 0) {
            ServerLog::Error("Invalid model info: Rank number cannot be zero, "
                             "and expert number should be divisible by rank number.");
            return false;
        }
        int expertNumberPerRank = modelInfo.expertNumber / modelInfo.rankNumber;
        for (auto &item: hotspotInfos) {
            item.expertId = item.rankId * expertNumberPerRank + item.localExpertId;
            item.expertIndex = item.rankId * expertNumberPerRank + item.localExpertId;
        }
        return true;
    }
    // todo-yqs:配置文件和热点数据匹配逻辑待补充
    return true;
}

std::vector<ExpertHotspotStruct> ExpertHotspotManager::QueryExpertHotspotData(const std::string &modelStage,
                                                                              const std::string &version)
{
    auto database = Timeline::DataBaseManager::Instance().GetClusterDatabase(COMPARE);
    if (database == nullptr) {
        return {};
    }
    auto hotspotRes = database->QueryExpertHotspotData(modelStage, version);
    std::sort(hotspotRes.begin(), hotspotRes.end());
    auto modelInfo = GetModelInfo(database);
    if (!FillExpertIdInfo(hotspotRes, modelInfo, {})) {
        return {};
    }
    return hotspotRes;
}
}
}
}