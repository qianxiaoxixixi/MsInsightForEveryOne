/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
 */
#ifndef DETAILSMEMORYPARSERTEST_H
#define DETAILSMEMORYPARSERTEST_H

#include <string>
#include <gtest/gtest.h>

class DetailsMemoryParserTest : public ::testing::Test {
protected:
    const std::string VALID_DETAILS_BASE_INFO_JSON = R"JSON(
{
	"advice" : ["The current frequency is lower than the rated frequency"],
	"block_dim" : "1",
	"cur_freq" : "800",
	"device_id" : "0",
	"duration" : 205.5041046142578,
	"mix_block_detail" : {
		"head_name" : ["Block ID", "Cube0 Duration (μs)", "Vector0 Duration (μs)", "Vector1 Duration (μs)"],
        "row" : [{
                "value" : ["0", "201.845001", "113.531250", "198.641251"]
            }
        ],
        "size" : [2, 4]
    },
    "mix_block_dim" : "2",
    "name" : "MatmulCustom_1396c02f7d7795e4c5cf7dddc5abe6da_1_mix_aic",
    "op_type" : "mix",
    "pid" : "4117480",
    "rated_freq" : "1650",
    "soc" : "Ascend910B4"
    }
)JSON";
};

#endif // DETAILSMEMORYPARSERTEST_H
