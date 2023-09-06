/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_CLUSTER_DATABASE_H
#define PROFILER_SERVER_CLUSTER_DATABASE_H


#include "Database.h"
#include "ClusterDef.h"
#include "Protocol.h"
#include "SummaryProtocolResponse.h"
#include "SummaryProtocolRequest.h"

namespace Dic {
namespace Module {
namespace Timeline {
class ClusterDatabase : public Database {
public:
    ClusterDatabase() = default;
    ~ClusterDatabase() override;

    bool SetConfig();
    bool CreateTable();
    bool InitStmt();
    void ReleaseStmt();
    bool InsertTimeInfo(Cluster::CommunicationTimeInfo &timeInfo);
    bool InsertTimeInfoList(std::vector<Cluster::CommunicationTimeInfo> &timeInfoList);
    bool InsertBandwidth(Cluster::CommunicationBandWidth &bandWidth);
    bool InsertBandwidthList(std::vector<Cluster::CommunicationBandWidth> &bandWidthList);
    bool InsertStepStatisticsInfo(Cluster::StepStatistic &stepStatistic);
    bool InsertClusterBaseInfo(Cluster::ClusterBaseInfo &clusterBaseInfo);
    bool QuerySummaryData(const Protocol::SummaryTopRankParams &requestParams,
                          Protocol::SummaryTopRankResBody &responseBody);
    void SaveLastData();
private:
    const std::string timeInfoTable = "communication_time_info";
    const std::string bandwidthTable = "communication_bandwidth_info";
    const std::string stepTraceTable = "step_statistic_info";
    const std::string baseInfoTable = "cluster_base_info";
    sqlite3_stmt *insertTimeInfoStmt = nullptr;
    sqlite3_stmt *insertBandwidthStmt = nullptr;
    sqlite3_stmt *stepStmt = nullptr;
    bool isInitStmt = false;
    const int cacheSize = 100;
    std::vector<Cluster::CommunicationTimeInfo> timeInfoCache;
    std::vector<Cluster::CommunicationBandWidth> bandwidthCache;
    sqlite3_stmt *GetTimeInfoStmtSql(int len);
    sqlite3_stmt *GetBandwidthStmtSql(int len);

    static void BuildCondition(const Protocol::SummaryTopRankParams &requestParams,
                               sqlite3_stmt *stmt, int index, std::string &stepCondition,
                               std::string &rankCondition);
};
} // end of namespace Timeline
} // end of namespace Module
} // end of namespace Dic

#endif // PROFILER_SERVER_CLUSTER_DATABASE_H