/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 * Description: Protocol Response declaration
 */

#ifndef PROFILER_SERVER_TIMELINETESTUTIL_CPP
#define PROFILER_SERVER_TIMELINETESTUTIL_CPP

const uint64_t PAGE_SIZE = 10;
const uint64_t CUR_PAGE_1 = 1;
const uint64_t CUR_PAGE_2 = 2;
const uint64_t EVENTS_VIEW_COLUMN_SIZE = 5;
const uint64_t EVENTS_VIEW_COLUMN_NAME = 0;
const uint64_t EVENTS_VIEW_COLUMN_START = 1;
const uint64_t EVENTS_VIEW_COLUMN_DURATION = 2;
const uint64_t EVENTS_VIEW_COLUMN_4 = 3;
const uint64_t EVENTS_VIEW_COLUMN_5 = 4;

inline void CheckEventsViewBaseColumns(Dic::Protocol::EventsViewBody &body)
{
    EXPECT_EQ(body.columnList.size(), EVENTS_VIEW_COLUMN_SIZE);
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_NAME).name, "Name");
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_START).name, "Start");
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_DURATION).name, "Duration(ns)");
}

inline void CheckEventsViewColumns4Api(Dic::Protocol::EventsViewBody &body)
{
    CheckEventsViewBaseColumns(body);
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_4).name, "TID");
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_5).name, "PID");
}

inline void CheckEventsViewColumns4Hardware(Dic::Protocol::EventsViewBody &body)
{
    CheckEventsViewBaseColumns(body);
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_4).name, "Stream Name");
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_5).name, "Rank ID");
}

inline void CheckEventsViewColumns4Group(Dic::Protocol::EventsViewBody &body)
{
    CheckEventsViewBaseColumns(body);
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_4).name, "Group Name");
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_5).name, "Rank ID");
}

inline void CheckEventsViewColumns4Overlap(Dic::Protocol::EventsViewBody &body)
{
    CheckEventsViewBaseColumns(body);
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_4).name, "Analysis Type");
    EXPECT_EQ(body.columnList.at(EVENTS_VIEW_COLUMN_5).name, "Rank ID");
}

#endif // PROFILER_SERVER_TIMELINETESTUTIL_CPP
