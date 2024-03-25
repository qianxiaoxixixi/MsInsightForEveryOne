/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#include <gtest/gtest.h>
#include "WsSessionManager.h"
class HandlerTest : public ::testing::Test {
public:
    static void SetUpTestCase()
    {
        Dic::Server::WsChannel *ws;
        std::unique_ptr<Dic::Server::WsSession> session = std::make_unique<Dic::Server::WsSession>(ws);
        std::string token = "hhhhhhhhhhhhh";
        Dic::Server::WsSessionManager::Instance().AddSession(token, std::move(session));
    }
    static void TearDownTestCase()
    {
        std::string token = "hhhhhhhhhhhhh";
        Dic::Server::WsSessionManager::Instance().GetSession(token)->Stop();
        Dic::Server::WsSessionManager::Instance().RemoveSession(token);
    }
    static int Main(int argc, char** argv)
    {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
};
