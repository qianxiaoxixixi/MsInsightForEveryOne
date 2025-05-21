/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2025. All rights reserved.
 */

#ifndef PROFILER_SERVER_LEAKSMEMORYSERVICE_H
#define PROFILER_SERVER_LEAKSMEMORYSERVICE_H

#include <utility>

#include "MemoryDef.h"
#include "LeaksMemoryDatabase.h"
#include "MemoryProtocolRespose.h"

namespace Dic {
namespace Module {
namespace Memory {
struct BlockEventAttr {
    std::string addr;
    int64_t size;
    std::string owner;
};
class LeaksMemoryService {
public:
    static void ParseEventsToBlockAndAllocations(const std::vector<MemoryEvent> &events,
                                                 const std::shared_ptr<FullDb::LeaksMemoryDatabase> &db);
    static void BuildBlockEventAttrFromEvent(const MemoryEvent &event, BlockEventAttr &eventAttr);
    static bool ParseMemoryLeaksDumpEvents(const std::string &fileId);
    static void ParserEnd(const std::string &rankId, bool result);
    static void ParseCallBack(const std::string &fileId, bool result, const std::string &msg);

private:
    static bool SingleDeviceEventParse(const std::shared_ptr<FullDb::LeaksMemoryDatabase> &db, const MemoryEvent &event,
                                       std::map<std::string, const MemoryEvent *> &allocMap,
                                       const BlockEventAttr &eventExtendAttr);
};
}  // Memory
}  // Module
}  // Dic

#endif  // PROFILER_SERVER_LEAKSMEMORYSERVICE_H
