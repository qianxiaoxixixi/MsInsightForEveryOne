cmake_minimum_required(VERSION 3.16)

set(SRC_HOME_DIR ${HOME_DIR}/src)

# usockets definitions
add_compile_definitions("LIBUS_NO_SSL")
add_compile_definitions("UWS_NO_ZLIB")
add_compile_definitions("LIBUS_USE_LIBUV")

# src defs
include_directories(${SRC_HOME_DIR}/defs)

# src utils
include_directories(${SRC_HOME_DIR}/utils)
aux_source_directory(${SRC_HOME_DIR}/utils DIC_UTILS_SRC)

# src channel
include_directories(${SRC_HOME_DIR}/channel)
aux_source_directory(${SRC_HOME_DIR}/channel DIC_CHANNEL_SRC)

# src server
include_directories(${SRC_HOME_DIR}/server)
aux_source_directory(${SRC_HOME_DIR}/server DIC_SERVER_SRC)

# src protocol
include_directories(${SRC_HOME_DIR}/protocol)
aux_source_directory(${SRC_HOME_DIR}/protocol DIC_PROTOCOL_SRC)

# src module
include_directories(${SRC_HOME_DIR}/module)
include_directories(${SRC_HOME_DIR}/module/base)
include_directories(${SRC_HOME_DIR}/module/global)
include_directories(${SRC_HOME_DIR}/module/global/handler)
include_directories(${SRC_HOME_DIR}/module/timeline)
include_directories(${SRC_HOME_DIR}/module/timeline/core)
include_directories(${SRC_HOME_DIR}/module/timeline/core/database)
include_directories(${SRC_HOME_DIR}/module/timeline/core/parser)
include_directories(${SRC_HOME_DIR}/module/timeline/handler)
include_directories(${SRC_HOME_DIR}/module/timeline/protocol)

# module for server
aux_source_directory(${SRC_HOME_DIR}/module DIC_MODULE_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/base DIC_MODULE_BASE_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/global DIC_MODULE_GLOBAL_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/global/handler DIC_MODULE_GLOBAL_HANDLER_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/timeline  DIC_MODULE_TIMELINE_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/timeline/core DIC_MODULE_TIMELINE_CORE_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/timeline/core/database DIC_MODULE_TIMELINE_CORE_DATABASE_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/timeline/core/parser DIC_MODULE_TIMELINE_CORE_PARSER_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/timeline/handler DIC_MODULE_TIMELINE_HANDLER_SRC)
aux_source_directory(${SRC_HOME_DIR}/module/timeline/protocol DIC_MODULE_TIMELINE_PROTOCOL_SRC)

list(APPEND DIC_MODULE_SRC_LIST
        ${DIC_MODULE_SRC}
        ${DIC_MODULE_BASE_SRC}
        ${DIC_MODULE_GLOBAL_SRC}
        ${DIC_MODULE_GLOBAL_HANDLER_SRC}
        ${DIC_MODULE_TIMELINE_SRC}
        ${DIC_MODULE_TIMELINE_CORE_SRC}
        ${DIC_MODULE_TIMELINE_CORE_DATABASE_SRC}
        ${DIC_MODULE_TIMELINE_CORE_PARSER_SRC}
        ${DIC_MODULE_TIMELINE_HANDLER_SRC}
        ${DIC_MODULE_TIMELINE_PROTOCOL_SRC})

add_subdirectory(entry)