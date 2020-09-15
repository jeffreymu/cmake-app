cmake_minimum_required(VERSION 3.12)
project(me)
add_custom_target(cmake-build-debug)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "CMAKE_CXX_COMPILER_ID=${CMAKE_CXX_COMPILER_ID}")

# 编译google test，会在当前目录生成libtest.a静态库
add_subdirectory(lib/ext/googletest)

#头文件
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/include ${PROJECT_SOURCE_DIR}lib/ext/googletest/include /usr/local/include)

link_directories(${PROJECT_SOURCE_DIR}/lib ${PROJECT_SOURCE_DIR}/lib/ext/googletest /usr/local/opt/curl/lib/ /usr/local/lib/ )

#编译器相关设置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/output/bin")
set(LIBRARIES pthread)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -g -Wall")     # 调试信息,开启所有警告

#Boost
set(BOOST_ROOT "D:/boost_1_68_0")
#set(Boost_INCLUDEDIR, ${BOOST_ROOT})
set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib)
set(BOOST_MIN_VERSION "1.68.0")
set (Boost_NO_BOOST_CMAKE ON)

set(Boost_COMPILER -mgw81)#编译器后缀，将会在引用的库文件名字后加-mgw81
set(Boost_USE_MULTITHREADED ON)#启用多线程，将会在引用的库文件名字后加-mt
set(Boost_USE_STATIC_RUNTIME ON)#静态链接到C++运行库的库，将会在引用的库文件名字后加-s
set(Boost_USE_DEBUG_RUNTIME ON)#引入调试库，将会在引用的库文件名字后加-d，与Boost_USE_STATIC_RUNTIME参数合并为-sd
set(Boost_ARCHITECTURE -x64)#引用x64库，将会在引用的库文件名字后加-x64
set(Boost_USE_STATIC_LIBS ON)#引入静态链接库，对应.a后缀

#开启以下三个参数，以便查错
set(Boost_DEBUG ON)
set(Boost_DETAILED_FAILURE_MSG ON)
set(Boost_REALPATH ON)

FIND_PACKAGE(Boost ${BOOST_MIN_VERSION} COMPONENTS REQUIRED)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
if (NOT Boost_FOUND)
    message(FATAL_ERROR "Fatal error: Boost (version >= ${BOOST_MIN_VERSION}) required.")
else()
    message(STATUS "Setting up BOOST")
    message(STATUS " Includes - ${Boost_INCLUDE_DIRS}")
    message(STATUS " Library  - ${BOOST_LIBRARY_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${BOOST_LIBRARY_DIRS})
endif (NOT Boost_FOUND)

if (UNIX AND APPLE)
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -lboost_locale-mt -lboost_system-mt -lboost_filesystem-mt")
elseif(UNIX AND NOT APPLE)
    set(CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -lboost_locale -lboost_system -lboost_filesystem")
endif()

#源码目录
FILE(GLOB_RECURSE SOURCEFILES ${PROJECT_SOURCE_DIR}/src/include/*.cpp)
FILE(GLOB_RECURSE TEST_SOURCEFILES ${PROJECT_SOURCE_DIR}/src/test/*.cpp)

#add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/src/main/main.cpp ${SOURCEFILES})
add_executable(work_relation ${PROJECT_SOURCE_DIR}/src/main/relation.cpp ${SOURCEFILES})
add_executable(work_test ${TEST_SOURCEFILES} ${SOURCEFILES})

#target_link_libraries(${PROJECT_NAME} gtest curl ${BOOST_LIBRARYDIR})
target_link_libraries(work_relation gtest curl ${BOOST_LIBRARYDIR})
target_link_libraries(work_test gtest curl ${BOOST_LIBRARYDIR})

if (UNIX AND APPLE)
    execute_process(
            COMMAND bash -c "cp -R ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_SOURCE_DIR}/output/bin/"
            COMMAND bash -c "cp -R ${CMAKE_CURRENT_SOURCE_DIR}/conf ${CMAKE_CURRENT_SOURCE_DIR}/output/bin/"
    )
endif()
