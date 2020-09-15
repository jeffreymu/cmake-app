project(work_test)

message (" project_source_dir:${PROJECT_SOURCE_DIR}")
#头文件
include_directories(${PROJECT_SOURCE_DIR}/../include ${PROJECT_SOURCE_DIR}/../../lib/ext/googletest/include)

aux_source_directory(. DIR_HELLO_SRCS)
add_library(work_test ${DIR_HELLO_SRCS})
