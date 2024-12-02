/*
* Copyright (c) Huawei Technologies Co., Ltd. 2024-2024. All rights reserved.
*/

#ifndef PROFILER_SERVER_INTERCORELOADTEST_H
#define PROFILER_SERVER_INTERCORELOADTEST_H

#include <string>

namespace Dic::Module::Source::Test {
const std::string INTER_CORE_LOAD_ANALYSIS_OF_VECTOR_JSON = R"(
{
	"advice" : "\t1) core3 vector1 took more time than other vector cores.\n",
	"op_detail" : [{
			"core_detail" : [{
					"L2cache_hit_rate" : "75.700935",
					"cycles" : "7114",
					"subcore_id" : "0",
					"subcore_type" : "vector",
					"throughput" : "15104"
				}
			],
			"core_id" : 0
		}, {
			"core_detail" : [{
					"L2cache_hit_rate" : "76.635513",
					"cycles" : "7453",
					"subcore_id" : "0",
					"subcore_type" : "vector",
					"throughput" : "15104"
				}
			],
			"core_id" : 1
		}, {
			"core_detail" : [{
					"L2cache_hit_rate" : "77.272728",
					"cycles" : "7940",
					"subcore_id" : "0",
					"subcore_type" : "vector",
					"throughput" : "15104"
				}
			],
			"core_id" : 2
		}
	],
	"op_type" : "vector",
	"soc" : "Ascend910B4"
}
)";
const std::string INTER_CORE_LOAD_ANALYSIS_JSON = R"(
{
    "advice": "\t0) vector core0 subcore1 took more time than other core.\n",
    "op_detail": [
        {
            "core_detail": [
                {
                    "L2cache_hit_rate": "63.994083",
                    "cycles": "135938",
                    "subcore_id": "0",
                    "subcore_type": "cube",
                    "throughput": "256"
                },
                {
                    "L2cache_hit_rate": "87.804878",
                    "cycles": "76949",
                    "subcore_id": "0",
                    "subcore_type": "vector",
                    "throughput": "512"
                },
                {
                    "L2cache_hit_rate": "92.682930",
                    "cycles": "130426",
                    "subcore_id": "1",
                    "subcore_type": "vector",
                    "throughput": "128"
                }
            ],
            "core_id": 0
        }
    ],
    "op_type": "mix",
    "soc": "Ascend910B4"
}
)";

const std::string INTER_CORE_LOAD_ANALYSIS_RESPONSE_JSON =
    R"({"type":"response","id":0,"requestId":0,"result":false,"command":"source/details/interCoreLoadAnalysis",)"
    R"("moduleName":"unknown","body":{"soc":"soc","opType":"optype","advice":"advice",)"
    R"("opDetails":[{"coreId":0,"subCoreDetails":[{"subCoreName":"cube0",)";

}
#endif // PROFILER_SERVER_INTERCORELOADTEST_H
