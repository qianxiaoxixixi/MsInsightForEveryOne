cmake_minimum_required(VERSION 3.16)
project(insight_fuzz)

set(FUZZ_HOME_DIR ${HOME_DIR}/src/test/fuzz)

message("fuzz start")

message(STATUS "[fuzz dir is] ${FUZZ_HOME_DIR}")

include_directories(${FUZZ_HOME_DIR})

include_directories(${FUZZ_HOME_DIR}/timeline)
aux_source_directory(${FUZZ_HOME_DIR}/timeline FUZZ_TIMELINE_TEST)

include_directories(${FUZZ_HOME_DIR}/compute)
aux_source_directory(${FUZZ_HOME_DIR}/compute FUZZ_COMPUTE_TEST)

include_directories(${FUZZ_HOME_DIR}/common)
aux_source_directory(${FUZZ_HOME_DIR}/common FUZZ_COMMON_TEST)
list(APPEND DIC_FUZZ_TEST_LIST
		${FUZZ_TIMELINE_TEST}
        ${FUZZ_COMPUTE_TEST}
        ${FUZZ_COMMON_TEST}
		${SRC_HOME_DIR}/utils/
        ${U_SOCKETS_SRC_LIST}
        ${DIC_UTILS_SRC}
        ${DIC_PROTOCOL_SRC}
        ${DIC_MODULES_SRC_LIST}
        ${DIC_SERVER_SRC}
        ${DIC_CHANNEL_SRC}
)

add_executable(insight_fuzz  FuzzMain.cpp ${DIC_FUZZ_TEST_LIST})
target_include_directories(insight_fuzz PRIVATE
		${HOME_DIR}/msinsight/src
		${HOME_DIR}/msinsight/src/utils
		${HOME_DIR}/msinsight/src/base
		${HOME_DIR}/msinsight/include
		${HOME_DIR}/msinsight/include/utils
		${HOME_DIR}/msinsight/include/base)
# 添加预编译头文件到目标
target_precompile_headers(insight_fuzz PRIVATE ${HOME_DIR}/msinsight/src/utils/pch.h)
# U_SOCKETS_SRC_LIST无法使用预编译头文件，故排除
set_source_files_properties(${U_SOCKETS_SRC_LIST} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)
add_compile_options(
        -g
        -fPIC
        -Wall
        -Winvalid-pch
        -fms-extensions
        -std=c++11
        -O0
        -fno-omit-frame-pointer
        -ftest-coverage
        -fprofile-arcs
        -fdump-rtl-expand
        -Wno-pmf-conversions
        -D_GLIBCXX_USE_CXX11_ABI=0
        # for fsannitize
        -fsanitize=address
        -fsanitize=undefined
        -fsanitize-coverage=trace-cmp
        -fsanitize-coverage=trace-pc
)

add_link_options(
        -fsanitize=address
        -fsanitize=undefined
        -fsanitize-coverage=trace-pc,trace-cmp
)

target_link_libraries(insight_fuzz msinsight gtest gtest_main gmock_main
		uv_a pthread dl sqlite z Secodefuzz stdc++fs)

target_link_directories(insight_fuzz PUBLIC
        ${HOME_DIR}/third_party/secodefuzz/examples/out-bin-x64/
		${HOME_DIR}/third_party/googletest/
)


target_include_directories(insight_fuzz PUBLIC
	    ${HOME_DIR}/third_party/googletest/googletest/include/gtest
        ${HOME_DIR}/third_party/secodefuzz/Secodefuzz
)

install(TARGETS insight_fuzz
        RUNTIME DESTINATION ${HOME_DIR}/build)
