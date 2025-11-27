/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include "ServerLog.h"
#include "OperatorMemoryService.h"
namespace Dic::Module::Memory {
CommonStringNumberComparator OperatorMemoryService::DefaultStringNumberComparator =
    [](const std::string numStr1, const std::string numStr2) {
        return NumberUtil::StringToDouble(numStr1) < NumberUtil::StringToDouble(numStr2);
    };

CommonStringNumberComparator OperatorMemoryService::DefaultReleaseTimeComparator =
    [](const std::string numStr1, const std::string numStr2) {
        double num1 = numStr1 == "N/A" ? std::numeric_limits<double>::max() : NumberUtil::StringToDouble(numStr1);
        double num2 = numStr2 == "N/A" ? std::numeric_limits<double>::max() : NumberUtil::StringToDouble(numStr2);
        return num1 < num2;
    };

OperatorMemoryService::OperatorMemoryService(std::shared_ptr<OperatorTable> operatorTablePtr,
    std::shared_ptr<OpMemoryTable> opMemoryTablePtr)
    : operatorTable(std::move(operatorTablePtr)), opMemoryTable(std::move(opMemoryTablePtr))
{}

OperatorDomain OperatorMemoryService::ComputeAllocationTimeById(const std::string &rankId, const std::string &id)
{
    OperatorDomain target;
    std::vector<OperatorPO> textPOs;
    operatorTable->Select(OpMemoryColumn::ALLOCATION_TIME)
        .Eq(OpMemoryColumn::ID, id)
        .OrderBy(OpMemoryColumn::ALLOCATION_TIME, Timeline::TableOrder::ASC)
        .ExcuteQuery(rankId, textPOs);
    if (!std::empty(textPOs)) {
        target.allocationTime = textPOs.front().allocationTime;
        target.metaType = "TEXT";
        return target;
    }
    std::vector<OpMemoryPO> dbPOs;
    opMemoryTable->Select(OpMemoryColumn::ALLOCATION_TIME)
        .Eq(OpMemoryColumn::ID, id)
        .OrderBy(OpMemoryColumn::ALLOCATION_TIME, Timeline::TableOrder::ASC)
        .ExcuteQuery(rankId, dbPOs);
    if (!std::empty(dbPOs)) {
        target.allocationTime = dbPOs.front().allocationTime;
        target.metaType = "PYTORCH_API";
        return target;
    }
    Dic::Server::ServerLog::Warn("Failed to query operator allocation time, id is: ", id);
    return target;
}

MemoryOperatorComparator OperatorMemoryService::GetComparatorByColumn(std::string_view orderBy, bool desc)
{
    MemoryOperatorComparator comparator;
    if (AscComparatorMap.find(orderBy) == AscComparatorMap.end()) {
        comparator = AscComparatorMap[OpMemoryColumn::NAME];
    } else {
        comparator = AscComparatorMap[orderBy];
    }
    // 如果降序需要翻转
    if (desc) {
        comparator = [comparator](const MemoryOperator &op1, const MemoryOperator &op2) {
            return comparator(op2, op1);
        };
    }
    return comparator;
}
}
