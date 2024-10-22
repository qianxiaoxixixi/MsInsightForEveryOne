/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */

#include <gtest/gtest.h>
#include <WsSessionManager.h>
#include "OperatorRequestHandler.h"
#include "QueryOpCategoryInfoHandler.h"
#include "QueryOpComputeUnitHandler.h"
#include "QueryOpStatisticInfoHandler.h"
#include "QueryOpDetailInfoHandler.h"
#include "QueryOpMoreInfoHandler.h"
#include "WsSessionImpl.h"

using namespace Dic::Server;
class OperatorRequestHandlerTest : public ::testing::Test {
public:
    static void SetUpTestSuite()
    {
        Dic::Server::WsChannel *ws;
        std::unique_ptr<WsSessionImpl> session = std::make_unique<WsSessionImpl>(ws);
        Dic::Server::WsSessionManager::Instance().AddSession(std::move(session));
    }
    static void TearDownTestSuite()
    {
        auto session = Dic::Server::WsSessionManager::Instance().GetSession();
        if (session != nullptr) {
            session->SetStatus(Dic::Server::WsSession::Status::CLOSED);
            session->WaitForExit();
            Dic::Server::WsSessionManager::Instance().RemoveSession();
        }
    }
};

TEST_F(OperatorRequestHandlerTest, QueryOpCategoryInfoHandlerNormalTest)
{
    Dic::Module::Operator::QueryOpCategoryInfoHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::OperatorCategoryInfoRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.group = "Operator";
    requestPtr.get()->params.topK = -1;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(OperatorRequestHandlerTest, QueryOpComputeUnitHandlerNormalTest)
{
    Dic::Module::Operator::QueryOpComputeUnitHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::OperatorComputeUnitInfoRequest>();
    requestPtr.get()->params.rankId = "0";
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(OperatorRequestHandlerTest, QueryOpStatisticInfoHandlerNormalTest)
{
    Dic::Module::Operator::QueryOpStatisticInfoHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::OperatorStatisticInfoRequest>();
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(OperatorRequestHandlerTest, QueryOpStatisticInfoHandlerCmplTest)
{
    Dic::Module::Operator::QueryOpStatisticInfoHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::OperatorStatisticInfoRequest>();
    requestPtr.get()->params.rankId = "0";
    requestPtr.get()->params.group = "Operator Type";
    requestPtr.get()->params.topK = -1;
    requestPtr.get()->params.isCompare = true;
    // 10 表示分页最小是10条
    requestPtr.get()->params.pageSize = 10;
    requestPtr.get()->params.current = 1;
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(OperatorRequestHandlerTest, QueryOpDetailInfoHandlerNormalTest)
{
    Dic::Module::Operator::QueryOpDetailInfoHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::OperatorDetailInfoRequest>();
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}

TEST_F(OperatorRequestHandlerTest, QueryOpMoreInfoHandlerNormalTest)
{
    Dic::Module::Operator::QueryOpMoreInfoHandler handler;
    auto requestPtr = std::make_unique<Dic::Protocol::OperatorMoreInfoRequest>();
    ASSERT_NO_THROW(handler.HandleRequest(std::move(requestPtr)));
}