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
        SDL2::SDL2
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
        test/sample.cpp
        test/vo.cpp
        test/Pocket.cpp
        test/PlayList.cpp
)
set(
        TEST_LIB
        ${COMMON_LIB}
        GTest::GTest
        GTest::Main
)

########## init ##########

########## target ##########
# release
add_executable(player ${RELEASE_SOURCE})
target_link_libraries(player ${RELEASE_LIB})

# test
add_executable(unit_test ${TEST_SOURCE})
target_link_libraries(unit_test ${TEST_LIB})
########## target ##########

########## misc ##########
enable_testing()
find_package(GTest REQUIRED)
find_package(gflags REQUIRED)
find_package(glog REQUIRED)
find_package(SDL2 REQUIRED)
########## misc ##########
