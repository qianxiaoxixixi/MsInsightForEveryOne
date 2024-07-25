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
        Dic::Server::WsSessionManager::Instance().AddSession(std::move(session));
    }
    static void TearDownTestCase()
    {
        Dic::Server::WsSessionManager::Instance().GetSession()->Stop();
        Dic::Server::WsSessionManager::Instance().RemoveSession();
    }
    static int Main(int argc, char** argv)
    {
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();
    }
};
