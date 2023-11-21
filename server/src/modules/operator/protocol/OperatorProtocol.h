/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 */

#ifndef PROFILER_SERVER_OPERATORPROTOCOL_H
#define PROFILER_SERVER_OPERATORPROTOCOL_H

#include "ProtocolUtil.h"

namespace Dic::Protocol {
    class OperatorProtocol : public ProtocolUtil {
    public:
        OperatorProtocol() = default;
        ~OperatorProtocol() override = default;

    private:
        void RegisterJsonToRequestFuncs() override;
        void RegisterResponseToJsonFuncs() override;
        void RegisterEventToJsonFuncs() override;

        static std::unique_ptr<Request> ToOperatorCategoryInfoRequest(const json_t &json, std::string &error);
        static std::unique_ptr<Request> ToOperatorComputeUnitInfoRequest(const json_t &json, std::string &error);
        static std::unique_ptr<Request> ToOperatorStatisticInfoRequest(const json_t &json, std::string &error);
        static std::unique_ptr<Request> ToOperatorDetailInfoRequest(const json_t &json, std::string &error);
        static std::unique_ptr<Request> ToOperatorMoreInfoRequest(const json_t &json, std::string &error);

        static std::optional<json_t> ToOperatorCategoryInfoResponse(const Response &response);
        static std::optional<json_t> ToOperatorComputeUnitInfoResponse(const Response &response);
        static std::optional<json_t> ToOperatorStatisticInfoResponse(const Response &response);
        static std::optional<json_t> ToOperatorDetailInfoResponse(const Response &response);
        static std::optional<json_t> ToOperatorMoreInfoResponse(const Response &response);

        static std::optional<json_t> ToOperatorParseStatusEvent(const Event &event);
    };
}

#endif // PROFILER_SERVER_OPERATORPROTOCOL_H
