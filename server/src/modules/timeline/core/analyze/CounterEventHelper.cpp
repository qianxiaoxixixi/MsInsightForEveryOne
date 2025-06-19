/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 */

#include "CounterEventHelper.h"
namespace Dic::Module::Timeline {
using namespace Dic::Protocol;
const std::map<std::string, std::string> CounterEventHelper::displayNameToValueName = {
    {"AI Core Freq", "freq"},
    {"Read", "read"},
    {"Write", "write"},
    {"L2 Buffer Bw Level", "l2BufferBwLevel"},
    {"Mata Bw Level", "mataBwLevel"}
};
void CounterEventHelper::RegisterHostMap()
{
    hostCounterEventMap.insert({PROCESS_TYPE::CPU_USAGE,
        {"CPU Usage", PROCESS_TYPE_ES.at(PROCESS_TYPE::CPU_USAGE), "usage", "CPU {cpuId}", "usage(%)"}});
    hostCounterEventMap.insert({PROCESS_TYPE::HOST_DISK_USAGE,
        {"Disk Usage", PROCESS_TYPE_ES.at(PROCESS_TYPE::HOST_DISK_USAGE), "usage", "Disk Usage", "usage(%)"}});
    hostCounterEventMap.insert({PROCESS_TYPE::HOST_NETWORK_USAGE,
        {"Network Usage", PROCESS_TYPE_ES.at(PROCESS_TYPE::HOST_NETWORK_USAGE), "usage", "Network Usage", "usage(%)"}});
    hostCounterEventMap.insert({PROCESS_TYPE::HOST_MEM_USAGE,
        {"Memory Usage", PROCESS_TYPE_ES.at(PROCESS_TYPE::HOST_MEM_USAGE), "usage", "Memory Usage", "usage(%)"}});
}

void CounterEventHelper::RegisterDeviceMap()
{
    RegisterDeviceAICoreFreqMap();
    RegisterDeviceAccPMUMap();
    RegisterDeviceDDRMap();
    RegisterDeviceStarsSocMap();
    RegisterDeviceNICMap();
    RegisterDevicePCIeMap();
    RegisterDeviceHCCSMap();
}

void CounterEventHelper::RegisterDeviceAICoreFreqMap()
{
    deviceCounterEventMap.insert({PROCESS_TYPE::AI_CORE,
        {"AI Core Freq", "AICORE_FREQ", "freq", "AI Core Freq", "Mhz"}});
}

void CounterEventHelper::RegisterDeviceAccPMUMap()
{
    deviceCounterEventMap.insert({PROCESS_TYPE::ACC_PMU,
        {"ACC_PMU", "ACC_PMU", "readBwLevel", "Accelerator {accId}/readBwLevel", "Level"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::ACC_PMU,
        {"ACC_PMU", "ACC_PMU", "writeBwLevel", "Accelerator {accId}/writeBwLevel", "Level"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::ACC_PMU,
        {"ACC_PMU", "ACC_PMU", "readOstLevel", "Accelerator {accId}/readOstLevel", "Level"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::ACC_PMU,
        {"ACC_PMU", "ACC_PMU", "writeOstLevel", "Accelerator {accId}/writeOstLevel", "Level"}});
}

void CounterEventHelper::RegisterDeviceDDRMap()
{
    deviceCounterEventMap.insert({PROCESS_TYPE::DDR,
        {"DDR", "DDR", "read", "Read", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::DDR,
        {"DDR", "DDR", "write", "Write", "Bandwidth(B/s)"}});
}

void CounterEventHelper::RegisterDeviceStarsSocMap()
{
    deviceCounterEventMap.insert({PROCESS_TYPE::STARS_SOC,
        {"Stars Soc", "SOC_BANDWIDTH_LEVEL", "l2BufferBwLevel", "L2 Buffer Bw Level", "Level"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::STARS_SOC,
        {"Stars Soc", "SOC_BANDWIDTH_LEVEL", "mataBwLevel", "Mata Bw Level", "Level"}});
}

// NIC数据db格式的来源是NETDEV_STATS表，而不是NIC表
void CounterEventHelper::RegisterDeviceNICMap()
{
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "roceTxPkt", "NIC/roceTxPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "roceRxPkt", "NIC/roceRxPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "roceTxErrPkt", "NIC/roceTxErrPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "roceRxErrPkt", "NIC/roceRxErrPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "roceTxCnpPkt", "NIC/roceTxCnpPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "roceRxCnpPkt", "NIC/roceRxCnpPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "roceNewPktRty", "NIC/roceNewPktRty", "Rty"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "nicTxByte", "NIC/nicTxByte", "Byte"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "nicTxBandwidth", "NIC/nicTxBandwidth", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "nicRxByte", "NIC/nicRxByte", "Byte"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "nicRxBandwidth", "NIC/nicRxBandwidth", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macTxPfcPkt", "NIC/macTxPfcPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macRxPfcPkt", "NIC/macRxPfcPkt", "Pkt"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macTxByte", "NIC/macTxByte", "Byte"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macTxBandwidth", "NIC/macTxBandwidth", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macRxByte", "NIC/macRxByte", "Byte"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macRxBandwidth", "NIC/macRxBandwidth", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macTxBadByte", "NIC/macTxBadByte", "Byte"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::NIC,
        {"NIC", "NETDEV_STATS", "macRxBadByte", "NIC/macRxBadByte", "Byte"}});
}

void CounterEventHelper::RegisterDevicePCIeMap()
{
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txPostMin", "PCIE/txPostMin", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txPostMax", "PCIE/txPostMax", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txPostAvg", "PCIE/txPostAvg", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxPostMin", "PCIE/rxPostMin", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxPostMax", "PCIE/rxPostMax", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxPostAvg", "PCIE/rxPostAvg", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txNonpostMin", "PCIE/txNonpostMin", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txNonpostMax", "PCIE/txNonpostMax", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txNonpostAvg", "PCIE/txNonpostAvg", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxNonpostMin", "PCIE/rxNonpostMin", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxNonpostMax", "PCIE/rxNonpostMax", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxNonpostAvg", "PCIE/rxNonpostAvg", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txCplMin", "PCIE/txCplMin", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txCplMax", "PCIE/txCplMax", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txCplAvg", "PCIE/txCplAvg", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxCplMin", "PCIE/rxCplMin", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxCplMax", "PCIE/rxCplMax", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "rxCplAvg", "PCIE/rxCplAvg", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txNonpostLatencyMin", "PCIE/txNonpostLatencyMin", "Time(ns)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txNonpostLatencyMax", "PCIE/txNonpostLatencyMax", "Time(ns)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::PCIE,
        {"PCIE", "PCIE", "txNonpostLatencyAvg", "PCIE/txNonpostLatencyAvg", "Time(ns)"}});
}

void CounterEventHelper::RegisterDeviceHCCSMap()
{
    deviceCounterEventMap.insert({PROCESS_TYPE::HCCS,
        {"HCCS", "HCCS", "txThroughput", "HCCS/txThroughput", "Bandwidth(B/s)"}});
    deviceCounterEventMap.insert({PROCESS_TYPE::HCCS,
        {"HCCS", "HCCS", "rxThroughput", "HCCS/rxThroughput", "Bandwidth(B/s)"}});
}

std::string CounterEventHelper::GenerateHostMetadataSQL(PROCESS_TYPE type)
{
    CounterEventConfig config = hostCounterEventMap.at(type);
    std::string sql = "SELECT DISTINCT ";
    std::string substitutedFormat = SubstituteThreadNameFormat(config.threadNameFormat);
    sql += substitutedFormat;
    sql += " AS name, '" + config.type + "' AS types FROM " + config.tableName + ";";
    return sql;
}

std::string CounterEventHelper::GenerateHostCounterSQL(Dic::Module::Timeline::PROCESS_TYPE type)
{
    CounterEventConfig config = hostCounterEventMap.at(type);
    std::string sql = "SELECT timestampNs - ? AS startTime, '{\"" + config.type + "\":' || " + config.valueName +
        " || '}' AS args FROM " + config.tableName + " WHERE ";
    std::string substitutedFormat = SubstituteThreadNameFormat(config.threadNameFormat);
    sql += substitutedFormat;
    sql += " = ? AND startTime >= ? AND startTime <= ? ORDER BY startTime ASC;";
    return sql;
}

std::string CounterEventHelper::GenerateDeviceMetadataSQL(Dic::Module::Timeline::PROCESS_TYPE type)
{
    std::string sql;
    for (auto [beg, end] = deviceCounterEventMap.equal_range(type); beg != end; ++beg) {
        CounterEventConfig config = beg->second;
        if (beg != deviceCounterEventMap.lower_bound(type)) {
            sql += " UNION ALL ";
        }
        sql += "SELECT DISTINCT ";
        std::string substitutedFormat = SubstituteThreadNameFormat(config.threadNameFormat);
        sql += substitutedFormat;
        sql += " AS name, '" + config.type + "' AS types FROM " + config.tableName + " WHERE deviceId = ?";
    }
    sql += ";";
    return sql;
}

std::string CounterEventHelper::GenerateDeviceCounterSQL(Dic::Module::Timeline::PROCESS_TYPE type,
    const std::string &threadId)
{
    std::string expectedDisplayName;
    size_t index = threadId.find_last_of('/');
    if (index == std::string::npos) {
        expectedDisplayName = threadId;
    } else {
        expectedDisplayName = threadId.substr(index + 1);
    }
    std::string expectedValueName;
    if (displayNameToValueName.find(expectedDisplayName) == displayNameToValueName.end()) {
        expectedValueName = expectedDisplayName;
    } else {
        expectedValueName = displayNameToValueName.at(expectedDisplayName);
    }
    auto beg = deviceCounterEventMap.lower_bound(type);
    auto end = deviceCounterEventMap.upper_bound(type);
    for (; beg != end; ++beg) {
        if (beg->second.valueName == expectedValueName) {
            break;
        }
    }
    if (beg == end) {
        return "";
    }

    CounterEventConfig config = beg->second;
    std::string sql = "SELECT timestampNs - ? AS startTime, '{\"" + config.type + "\":' || " + config.valueName +
        " || '}' AS args FROM " + config.tableName + " WHERE ";
    std::string substitutedFormat = SubstituteThreadNameFormat(config.threadNameFormat);
    sql += substitutedFormat;
    sql += " = ? AND startTime >= ? AND startTime <= ? AND deviceId = ? ORDER BY startTime ASC;";
    return sql;
}

std::string CounterEventHelper::SubstituteThreadNameFormat(const std::string &format)
{
    std::string substitutedFormat = "'";
    size_t index = 0;
    while (format.find("{", index) != std::string::npos) {
        size_t nextIndex = format.find("{", index);
        substitutedFormat += format.substr(index, nextIndex - index) + "' || ";
        size_t nextBackBraceIndex = format.find("}", index);
        substitutedFormat += format.substr(nextIndex + 1, nextBackBraceIndex - nextIndex - 1) + " || '";
        index = nextBackBraceIndex + 1;
    }
    substitutedFormat += format.substr(index, format.size() - index) +"'";
    return substitutedFormat;
}
}