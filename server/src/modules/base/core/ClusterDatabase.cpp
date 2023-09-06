/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#include "ClusterDatabase.h"
#include "ServerLog.h"
#include "SummaryProtocolResponse.h"

namespace Dic {
namespace Module {
namespace Timeline {
using namespace Server;
ClusterDatabase::~ClusterDatabase()
{
    SaveLastData();
    if (isInitStmt) {
        ReleaseStmt();
    }
}

bool ClusterDatabase::SetConfig()
{
    if (!isOpen) {
        ServerLog::Error("Failed to set config. Database is not open.");
        return false;
    }
    return ExecSql("PRAGMA synchronous = OFF; PRAGMA journal_mode = MEMORY;");
}

bool ClusterDatabase::CreateTable()
{
    if (!isOpen) {
        ServerLog::Error("Failed to set config. Cluster Database is not open.");
        return false;
    }
    std::string sql =
            "CREATE TABLE " + timeInfoTable +
            "(id INTEGER PRIMARY KEY AUTOINCREMENT, iteration_id VARCHAR(50),"
            " stage_id VARCHAR(200), rank_id VARCHAR(50), op_name VARCHAR(100),"
            " op_suffix VARCHAR(100), elapse_time double, synchronization_time_ratio double, "
            "synchronization_time double, transit_time double, wait_time_ratio double, "
            "wait_time double, idle_time double);"
            "CREATE TABLE " + bandwidthTable +
            " (id INTEGER PRIMARY KEY AUTOINCREMENT, iteration_id VARCHAR(50), "
            "stage_id VARCHAR(200), rank_id VARCHAR(50), op_name VARCHAR(100), "
            "op_suffix VARCHAR(100), transport_type VARCHAR(20), bandwidth_size double,"
            " bandwidth_utilization double, large_package_ratio double, size_distribution json,"
            " transit_size double, transit_time double);" +
            "CREATE TABLE " + baseInfoTable +
            " (id INTEGER PRIMARY KEY AUTOINCREMENT, file_path VARCHAR(500), ranks json,"
            " steps json, collect_start_time DATETIME, collect_duration double, data_size double);" +
            "CREATE TABLE " + stepTraceTable +
            "(id INTEGER PRIMARY KEY AUTOINCREMENT, rank_id VARCHAR(50), step_id VARCHAR(50),"
            " stage_id VARCHAR(50), compute_time double, pure_communication_time double, "
            "overlap_communication_time double, communication_time double, free_time double, "
            "stage_time double, bubble_time double, pure_communication_exclude_receive_time double);";
    return ExecSql(sql);
}

bool ClusterDatabase::InitStmt()
{
    if (isInitStmt) {
        return true;
    }
    insertTimeInfoStmt = GetTimeInfoStmtSql(cacheSize);
    insertBandwidthStmt = GetBandwidthStmtSql(cacheSize);
    isInitStmt = true;
    return true;
}

sqlite3_stmt *ClusterDatabase::GetTimeInfoStmtSql(int len)
{
    if (len == cacheSize && isInitStmt) {
        sqlite3_reset(insertBandwidthStmt);
        return insertTimeInfoStmt;
    }
    sqlite3_stmt *stmt = nullptr;
    std::string sql = "INSERT INTO " + timeInfoTable +
                      " (iteration_id, stage_id, rank_id, op_name, op_suffix, elapse_time,"
                      " synchronization_time_ratio,"
                      "synchronization_time, transit_time, wait_time_ratio, wait_time, idle_time )"
                      " VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
    for (int i = 0; i < len - 1; i++) {
        sql.append(",(?,?,?,?,?,?,?,?,?,?,?,?)");
    }
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        ServerLog::Error("Failed to prepare timeInfoTable statement. error:", sqlite3_errmsg(db));
    }
    return stmt;
}

sqlite3_stmt *ClusterDatabase::GetBandwidthStmtSql(int len)
{
    if (len == cacheSize && isInitStmt) {
        sqlite3_reset(insertBandwidthStmt);
        return insertBandwidthStmt;
    }
    sqlite3_stmt *stmt = nullptr;
    std::string sql = "INSERT INTO " + bandwidthTable +
                      " (iteration_id, stage_id, rank_id, op_name, op_suffix, transport_type,"
                      " bandwidth_size, bandwidth_utilization,large_package_ratio, size_distribution,"
                      " transit_size, transit_time) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)";
    for (int i = 0; i < len - 1; i++) {
        sql.append(",(?,?,?,?,?,?,?,?,?,?,?,?)");
    }
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        ServerLog::Error("Failed to prepare InsertBandwidth statement. error:", sqlite3_errmsg(db));
    }
    return stmt;
}

void ClusterDatabase::ReleaseStmt()
{
    if (insertTimeInfoStmt != nullptr) {
        sqlite3_finalize(insertTimeInfoStmt);
    }
    if (insertBandwidthStmt != nullptr) {
        sqlite3_finalize(insertBandwidthStmt);
    }
}

void ClusterDatabase::SaveLastData()
{
    ServerLog::Info("SaveLastData ");
    if (!timeInfoCache.empty()) {
        InsertTimeInfoList(timeInfoCache);
        timeInfoCache.clear();
    }
    if (!bandwidthCache.empty()) {
        InsertBandwidthList(bandwidthCache);
        bandwidthCache.clear();
    }
}

bool ClusterDatabase::InsertTimeInfo(Cluster::CommunicationTimeInfo &timeInfo)
{
    timeInfoCache.emplace_back(timeInfo);
    if (timeInfoCache.size() == cacheSize) {
        InsertTimeInfoList(timeInfoCache);
        timeInfoCache.clear();
    }
    return true;
}

bool ClusterDatabase::InsertTimeInfoList(std::vector<Cluster::CommunicationTimeInfo> &timeInfoList)
{
    if (timeInfoList.empty()) {
        return false;
    }
    sqlite3_stmt *stmt = GetTimeInfoStmtSql(timeInfoList.size());
    if (stmt == nullptr) {
        ServerLog::Error("Failed to get timeInfo stmt.");
        return false;
    }
    int idx = bindStartIndex;
    for (const auto &timeInfo: timeInfoList) {
        sqlite3_bind_text(stmt, idx++, timeInfo.iterationId.c_str(), timeInfo.iterationId.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, timeInfo.stageId.c_str(), timeInfo.stageId.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, timeInfo.rankId.c_str(), timeInfo.rankId.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, timeInfo.opName.c_str(), timeInfo.opName.length(), SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, timeInfo.opSuffix.c_str(), timeInfo.opSuffix.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, idx++, timeInfo.elapseTime);
        sqlite3_bind_double(stmt, idx++, timeInfo.synchronizationTimeRatio);
        sqlite3_bind_double(stmt, idx++, timeInfo.synchronizationTime);
        sqlite3_bind_double(stmt, idx++, timeInfo.transitTime);
        sqlite3_bind_double(stmt, idx++, timeInfo.waitTimeRatio);
        sqlite3_bind_double(stmt, idx++, timeInfo.waitTime);
        sqlite3_bind_double(stmt, idx++, timeInfo.idleTime);
    }
    auto result = sqlite3_step(stmt);
    if (timeInfoList.size() != cacheSize) {
        sqlite3_finalize(stmt);
    }
    if (result != SQLITE_DONE) {
        ServerLog::Error("Insert timeInfo data fail. ", sqlite3_errmsg(db));
        return false;
    }
    return true;
}

bool ClusterDatabase::InsertBandwidth(Cluster::CommunicationBandWidth &bandWidth)
{
    bandwidthCache.emplace_back(bandWidth);
    if (bandwidthCache.size() == cacheSize) {
        InsertBandwidthList(bandwidthCache);
        bandwidthCache.clear();
    }
    return true;
}

bool ClusterDatabase::InsertBandwidthList(std::vector<Cluster::CommunicationBandWidth> &bandWidthList)
{
    if (bandWidthList.empty()) {
        return false;
    }
    sqlite3_stmt *stmt = GetBandwidthStmtSql(bandWidthList.size());
    if (stmt == nullptr) {
        ServerLog::Error("Failed to get timeInfo stmt.");
        return false;
    }
    int idx = bindStartIndex;
    for (const auto &bandwidth: bandWidthList) {
        sqlite3_bind_text(stmt, idx++, bandwidth.iterationId.c_str(), bandwidth.iterationId.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, bandwidth.stageId.c_str(), bandwidth.stageId.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, bandwidth.rankId.c_str(), bandwidth.rankId.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, bandwidth.opName.c_str(), bandwidth.opName.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, bandwidth.opSuffix.c_str(), bandwidth.opSuffix.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, idx++, bandwidth.transportType.c_str(), bandwidth.transportType.length(),
                          SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, idx++, bandwidth.bandwidthSize);
        sqlite3_bind_double(stmt, idx++, bandwidth.bandwidthUtilization);
        sqlite3_bind_double(stmt, idx++, bandwidth.largePackageRatio);
        sqlite3_bind_text(stmt, idx++, bandwidth.sizeDistribution.c_str(),
                          bandwidth.sizeDistribution.length(), SQLITE_TRANSIENT);
        sqlite3_bind_double(stmt, idx++, bandwidth.transitSize);
        sqlite3_bind_double(stmt, idx++, bandwidth.transitTime);
    }
    auto result = sqlite3_step(stmt);
    if (bandWidthList.size() != cacheSize) {
        sqlite3_finalize(stmt);
    }
    if (result != SQLITE_DONE) {
        ServerLog::Error("Insert bandwidth data fail. ", sqlite3_errmsg(db));
        return false;
    }
    return true;
}

bool ClusterDatabase::InsertStepStatisticsInfo(Cluster::StepStatistic &stepStatistic)
{
    if (stepStmt == nullptr) {
        std::string sql = "INSERT INTO " + stepTraceTable +
                          "(rank_id, step_id, stage_id, compute_time,pure_communication_time,"
                          " overlap_communication_time,communication_time, free_time, stage_time,"
                          " bubble_time,pure_communication_exclude_receive_time) "
                          " VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stepStmt, nullptr) != SQLITE_OK) {
            ServerLog::Error("Failed to prepare stepTraceTable statement. error:", sqlite3_errmsg(db));
        }
        if (stepStmt == nullptr) {
            ServerLog::Error("Failed to get stepTraceTable stmt.");
            return false;
        }
    } else {
        sqlite3_reset(stepStmt);
    }
    int idx = bindStartIndex;
    sqlite3_bind_text(stepStmt, idx++, stepStatistic.rankId.c_str(), stepStatistic.rankId.length(),
                      SQLITE_STATIC);
    sqlite3_bind_text(stepStmt, idx++, stepStatistic.stepId.c_str(), stepStatistic.stepId.length(),
                      SQLITE_STATIC);
    sqlite3_bind_text(stepStmt, idx++, stepStatistic.stageId.c_str(), stepStatistic.stageId.length(),
                      SQLITE_STATIC);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.computingTime);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.pureCommunicationTime);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.overlapCommunicationTime);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.communicationTime);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.freeTime);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.stageTime);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.bubbleTime);
    sqlite3_bind_double(stepStmt, idx++, stepStatistic.pureCommunicationExcludeReceiveTime);
    auto result = sqlite3_step(stepStmt);
    if (result != SQLITE_DONE) {
        ServerLog::Error("Insert bandwidth data fail. ", sqlite3_errmsg(db));
        return false;
    }
    return true;
}

bool ClusterDatabase::InsertClusterBaseInfo(Cluster::ClusterBaseInfo &clusterBaseInfo)
{
    sqlite3_stmt *stmt;
    std::string sql = "INSERT INTO " + baseInfoTable +
                      "(file_path, ranks, steps, collect_start_time,collect_duration,data_size)"
                      " VALUES (?, (select json_group_array(rank_id) from "
                      "(select DISTINCT rank_id from step_statistic_info where rank_id !='')), "
                      "(select json_group_array(step_id) from"
                      " (select DISTINCT step_id from step_statistic_info where rank_id !='')) ,"
                      " ?, ?, ?)";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        ServerLog::Error("Failed to prepare baseInfoTable statement. error:", sqlite3_errmsg(db));
    }
    if (stmt == nullptr) {
        ServerLog::Error("Failed to get baseInfoTable stmt.");
        return false;
    }
    int idx = bindStartIndex;
    sqlite3_bind_text(stmt, idx++, clusterBaseInfo.filePath.c_str(), clusterBaseInfo.filePath.length(),
                      SQLITE_TRANSIENT);
    sqlite3_bind_double(stmt, idx++, clusterBaseInfo.collectStartTime);
    sqlite3_bind_double(stmt, idx++, clusterBaseInfo.collectDuration);
    sqlite3_bind_double(stmt, idx++, clusterBaseInfo.dataSize);
    auto result = sqlite3_step(stmt);
    if (result != SQLITE_DONE) {
        ServerLog::Error("Insert baseInfoTable data fail. ", sqlite3_errmsg(db));
        return false;
    }
    return true;
}

bool ClusterDatabase::QuerySummaryData(const Protocol::SummaryTopRankParams &requestParams,
                                       Protocol::SummaryTopRankResBody &responseBody)
{
    sqlite3_stmt *stmt = nullptr;
    int index = bindStartIndex;
    std::string stepCondition;
    std::string rankCondition;
    BuildCondition(requestParams, stmt, index, stepCondition, rankCondition);
    std::string sql = "SELECT rank_id as rankId, sum(ROUND(compute_time,2)) as computingTime,"
                      "sum(ROUND(pure_communication_time,2)) as communicationNotOverLappedTime,"
                      "sum(ROUND(overlap_communication_time,2)) as communicationOverLappedTime,"
                      "sum(ROUND(free_time,2)) as freeTime FROM " + stepTraceTable +
                      "WHERE rank_id !='' " + stepCondition + rankCondition
                      + "group by rank_id order by " + requestParams.orderBy + " desc";
    int result = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        ServerLog::Error("QueryThreads. Failed to prepare sql.", sqlite3_errmsg(db));
        return false;
    }
    std::vector<Protocol::SummaryDto> summaryDtoList;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int col = resultStartIndex;
        Protocol::SummaryDto summaryDto;
        summaryDto.rankId = sqlite3_column_string(stmt, col++);
        summaryDto.computingTime = sqlite3_column_double(stmt, col++);
        summaryDto.communicationNotOverLappedTime = sqlite3_column_double(stmt, col++);
        summaryDto.communicationOverLappedTime = sqlite3_column_double(stmt, col++);
        summaryDto.freeTime = sqlite3_column_double(stmt, col++);
        summaryDtoList.emplace_back(summaryDto);
    }
    responseBody.summaryList  = summaryDtoList;
    sqlite3_stmt *stmt2 = nullptr;
    std::string baseInfoSql =
            "select file_path as filePath,ranks,steps,data_size as dataSize from" + baseInfoTable;
    int result2 = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt2, nullptr);
    if (result2 != SQLITE_OK) {
        ServerLog::Error("Query base info Failed to prepare sql.", sqlite3_errmsg(db));
        return false;
    }
    while (sqlite3_step(stmt2) == SQLITE_ROW) {
        int col = resultStartIndex;
        responseBody.filePath = sqlite3_column_string(stmt, col++);
        json_t json = json_t::parse(sqlite3_column_string(stmt, col++));
        responseBody.rankList = json.get<std::vector<std::string>>();
        json = json_t::parse(sqlite3_column_string(stmt, col++));
        responseBody.stepList = json.get<std::vector<std::string>>();
        responseBody.dataSize = sqlite3_column_double(stmt, col++);
        responseBody.stepNum = responseBody.stepList.size();
        responseBody.rankCount = responseBody.rankList.size();
    }
    return true;
}

void ClusterDatabase::BuildCondition(const Protocol::SummaryTopRankParams &requestParams,
                                     sqlite3_stmt *stmt, int index, std::string &stepCondition,
                                     std::string &rankCondition)
{
    if (!requestParams.stepIdList.empty()) {
        stepCondition = "and step_id in (?";
        sqlite3_bind_text(stmt, index++, requestParams.stepIdList[0].c_str(),
                          requestParams.stepIdList[0].length(), SQLITE_TRANSIENT);
        for (int i = 1; i < requestParams.stepIdList.size(); i++) {
            sqlite3_bind_text(stmt, index++, requestParams.stepIdList[i].c_str(),
                              requestParams.stepIdList[i].length(),
                              SQLITE_TRANSIENT);
            stepCondition.append(",?");
        }
        stepCondition.append(") ");
    }
    if (!requestParams.rankIdList.empty()) {
        rankCondition = " and rank_id in (?";
        sqlite3_bind_text(stmt, index++, requestParams.rankIdList[0].c_str(),
                          requestParams.rankIdList[0].length(), SQLITE_TRANSIENT);
        for (int i = 1; i < requestParams.rankIdList[0].size(); i++) {
            sqlite3_bind_text(stmt, index++, requestParams.rankIdList[i].c_str(),
                              requestParams.rankIdList[i].length(), SQLITE_TRANSIENT);
            rankCondition.append(",?");
        }
        rankCondition.append(") ");
    }
}
} // end of namespace Timeline
} // end of namespace Module
} // end of namespace Dic