/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_OPERATORPROTOCOLRESPONSE_H
#define PROFILER_SERVER_OPERATORPROTOCOLRESPONSE_H

#include <vector>
#include "ProtocolDefs.h"
#include "ProtocolMessage.h"

namespace Dic::Protocol {

    // 饼图中按算子类型耗时统计和按计算单元分类耗时统计
    struct OperatorDurationRes {
        std::string name;
        int64_t duration;
    };

    // 按Operator Type、Input Shape展示时算子统计信息
    struct OperatorStatisticInfoRes {
        std::string opType;
        std::string inputShape;
        std::string accCore;
        uint64_t totalTime;
        uint64_t count;
        uint64_t avgTime;
        uint64_t maxTime;
        uint64_t minTime;
    };

    // 按Operator展示算子详细信息，和See More响应
    struct OperatorDetailInfoRes {
        std::string stepId;
        std::string name;
        std::string type;
        std::string accCore;
        uint64_t startTime;
        uint64_t duration;
        uint64_t waitTime;
        uint64_t blockDim;
        std::string inputShape;
        std::string inputType;
        std::string inputFormat;
        std::string outputShape;
        std::string outputType;
        std::string outputFormat;
    };

    // 获取按算子类型统计耗时信息以绘制饼图的响应
    struct OperatorCategoryInfoResponse : public Response {
        OperatorCategoryInfoResponse() : Response(REQ_RES_OPERATOR_CATEGORY_INFO) {}
        std::vector<OperatorDurationRes> datas;
    };

    // 获取按计算单元统计耗时信息以绘制饼图的响应
    struct OperatorComputeUnitInfoResponse : public Response {
        OperatorComputeUnitInfoResponse() : Response(REQ_RES_OPERATOR_COMPUTE_UNIT_INFO) {}
        std::vector<OperatorDurationRes> datas;
    };

    // 获取按算子类型统计耗时信息以填充表格的响应
    struct OperatorStatisticInfoResponse : public Response {
        OperatorStatisticInfoResponse() : Response(REQ_RES_OPERATOR_STATISTIC_INFO) {};
        uint64_t total;
        std::vector<OperatorStatisticInfoRes> datas;
    };

    // 获取按算子详情信息以填充表格的响应
    struct OperatorDetailInfoResponse : public Response {
        OperatorDetailInfoResponse() : Response(REQ_RES_OPERATOR_DETAIL_INFO) {};
        uint64_t total;
        std::string level; // l0, l1, l2
        std::vector<OperatorDetailInfoRes> datas;
    };

    // 获取See More时算子详情信息的响应
    struct OperatorMoreInfoResponse : public Response {
        OperatorMoreInfoResponse() : Response(REQ_RES_OPERATOR_MORE_INFO) {};
        uint64_t total;
        std::string level; // l0, l1, l2
        std::vector<OperatorDetailInfoRes> datas;
    };
}

#endif // PROFILER_SERVER_OPERATORPROTOCOLRESPONSE_H
