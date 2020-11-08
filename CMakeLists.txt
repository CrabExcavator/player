cmake_minimum_required(VERSION 3.17)
project(player)

set(CMAKE_CXX_STANDARD 20)

########## setNumOfStream ##########
enable_testing()
find_package(GTest REQUIRED)
find_package(gflags REQUIRED)
find_package(glog REQUIRED)
find_package(SDL2 REQUIRED)
find_package(folly REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(avutil REQUIRED libavutil)
pkg_search_module(avcodec REQUIRED libavcodec)
pkg_search_module(avformat REQUIRED libavformat)
pkg_search_module(swscale REQUIRED libswscale)
pkg_search_module(avfilter REQUIRED libavfilter)
pkg_search_module(swresample REQUIRED libswresample)
pkg_search_module(portaudio REQUIRED portaudiocpp)

include_directories(./src)
include_directories(./test)
include_directories(${avcodec_INCLUDE_DIRS})
link_directories(${avcodec_LIBRARY_DIRS})
include_directories(${avutil_INCLUDE_DIRS})
link_directories(${avutil_LIBRARY_DIRS})
include_directories(${avformat_INCLUDE_DIRS})
link_directories(${avformat_LIBRARY_DIRS})
include_directories(${swscale_INCLUDE_DIRS})
link_directories(${swscale_LIBRARY_DIRS})
include_directories(${avfilter_INCLUDE_DIRS})
link_directories(${avfilter_LIBRARY_DIRS})
include_directories(${swresample_INCLUDE_DIRS})
link_directories(${swresample_LIBRARY_DIRS})
include_directories(${portaudio_INCLUDE_DIRS})
link_directories(${portaudio_LIBRARY_DIRS})

set(
        COMMON_SOURCE
        src/misc/Buffer.h
        src/common/Config.cpp
        src/common/DefaultConfig.cpp
        src/core/SyncContext.cpp
        src/core/PlayerContext.cpp
        src/core/PlayList.cpp
        src/core/PlayEntry.cpp
        src/video/VideoOutput.cpp
        src/video/driver/DriverFactory.cpp
        src/video/driver/DriverSDL.cpp
        src/audio/AudioOutput.cpp
        src/audio/driver/DriverFactory.cpp
        src/audio/driver/DriverPortAudio.cpp
        src/input/InputContext.cpp
        src/input/handler/EventHandlerBase.cpp
        src/input/handler/Universal.cpp
        src/demux/Frame.cpp
        src/demux/DemuxContext.cpp
        src/demux/filter/FrameFilterBase.cpp
        src/demux/filter/Blit.cpp
        src/demux/filter/FillVideo.cpp
        src/demux/filter/FillAudio.cpp
        src/demux/stream/StreamFactory.cpp
        src/demux/stream/Stream.cpp
        src/demux/stream/VideoStream.cpp
        src/demux/stream/AudioStream.cpp
        src/demux/demuxer/AVDemuxer.cpp
        src/demux/demuxer/DemuxerFactory.cpp
)
set(
        COMMON_LIB
        glog::glog
        gflags
        SDL2::SDL2
        ${FOLLY_LIBRARIES}
        ${avutil_LIBRARIES}
        ${avcodec_LIBRARIES}
        ${avformat_LIBRARIES}
        ${swscale_LIBRARIES}
        ${avfilter_LIBRARIES}
        ${swresample_LIBRARIES}
        ${portaudio_LIBRARIES}
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
        test/Pocket.cpp
        test/PlayList.cpp
        test/Chain.cpp
)
set(
        TEST_LIB
        ${COMMON_LIB}
        GTest::GTest
        GTest::Main
)
########## setNumOfStream ##########

########## target ##########
# release
add_executable(player ${RELEASE_SOURCE})
target_link_libraries(player ${RELEASE_LIB})

# test
add_executable(unit_test ${TEST_SOURCE})
target_link_libraries(unit_test ${TEST_LIB})
########## target ##########
