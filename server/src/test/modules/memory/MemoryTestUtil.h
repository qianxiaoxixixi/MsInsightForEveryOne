/*
* Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
*/
#include "OperatorMemoryService.h"

namespace Dic::Module::Memory {
namespace OperatorMemoryTestUtil {
static bool IsOperatorsSorted(const std::vector<MemoryOperator> &operators, std::string_view orderBy, bool desc)
{
    auto comparator = OperatorMemoryService::GetComparatorByColumn(orderBy, desc);
    return std::is_sorted(operators.begin(), operators.end(), comparator);
}

static bool IsOperatorsNameAllContains(const std::vector<MemoryOperator> &operators, const std::string &subStr)
{
    return std::all_of(operators.begin(), operators.end(),
                       [subStr](const MemoryOperator &op) {
                           return StringUtil::ContainsIgnoreCase(op.name, subStr);
                       });
}
}
}
