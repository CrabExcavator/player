cmake_minimum_required(VERSION 3.17)
project(player)

set(CMAKE_CXX_STANDARD 20)

########## init ##########
enable_testing()
find_package(GTest REQUIRED)
find_package(gflags REQUIRED)
find_package(glog REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(./src)
include_directories(./test)

set(
        COMMON_SOURCE
        src/common/Config.cpp
        src/common/DefaultConfig.cpp
        src/misc/init.cpp
        src/core/PlayerContext.cpp
        src/core/PlayerContextImpl.cpp
        src/core/PlayList.cpp
        src/core/PlayEntry.cpp
        src/video/VideoOutput.cpp
        src/video/driver/DriverFactory.cpp
        src/video/driver/Driver.cpp
        src/video/driver/DriverSDL.cpp
        src/input/InputContext.cpp
)
set(
        COMMON_LIB
        glog::glog
        gflags
        ${SDL2_LIBRARIES}
)

set(
        RELEASE_SOURCE
        ${COMMON_SOURCE}
        src/main.cpp
        src/osal/main_osx.cpp
)
set(
        RELEASE_LIB
        ${COMMON_LIB}
)

set(
        TEST_SOURCE
        ${COMMON_SOURCE}
)
set(
        TEST_LIB
        ${COMMON_LIB}
        GTest::GTest
        GTest::Main
)

set (
        TEST_SAMPLE_SOURCE
        ${TEST_SOURCE}
        test/sample.cpp
)

set(
        TEST_DRIVER_SOURCE
        ${TEST_SOURCE}
        test/vo.cpp
)

set(
        TEST_POCKET_SOURCE
        ${TEST_SOURCE}
        test/Pocket.cpp
)
########## init ##########

########## target ##########
# release
add_executable(player ${RELEASE_SOURCE})
target_link_libraries(player ${RELEASE_LIB})

# test
add_executable(test_sample ${TEST_SAMPLE_SOURCE})
target_link_libraries(test_sample ${TEST_LIB})
add_executable(test_vo ${TEST_DRIVER_SOURCE})
target_link_libraries(test_vo ${TEST_LIB})
add_executable(test_pocket ${TEST_POCKET_SOURCE})
target_link_libraries(test_pocket ${TEST_LIB})
add_test(
        test_all
        test_sample
        test_vo
        test_pocket
)
########## target ##########

########## misc ##########
enable_testing()
find_package(GTest REQUIRED)
find_package(gflags REQUIRED)
find_package(glog REQUIRED)
find_package(SDL2 REQUIRED)
########## misc ##########
