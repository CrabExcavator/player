cmake_minimum_required(VERSION 3.17)
project(player)

set(CMAKE_CXX_STANDARD 20)

########## init ##########
enable_testing()
find_package(GTest REQUIRED)
find_package(gflags REQUIRED)
find_package(glog REQUIRED)
find_package(SDL2 REQUIRED)
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
        src/common/Slots.cpp
        src/common/GlobalInstanceManager.cpp
        src/dag/DAGContext.cpp
        src/dag/DAGNodeWrapper.cpp
        src/dag/DAGRoot.cpp
        src/player/PlayerContext.cpp
        src/player/PlayList.cpp
        src/player/PlayEntry.cpp
        src/output/video/VideoOutput.cpp
        src/output/video/driver/DriverFactory.cpp
        src/output/video/driver/DriverSDL.cpp
        src/output/video/ImageFormat.cpp
        src/output/audio/AudioOutput.cpp
        src/output/audio/driver/DriverFactory.cpp
        src/output/audio/driver/DriverPortAudio.cpp
        src/output/audio/driver/DriverSDL.cpp
        src/output/audio/SampleFormat.cpp
        src/output/audio/ChannelLayout.cpp
        src/input/InputContext.cpp
        src/input/handler/EventHandlerBase.cpp
        src/input/handler/Universal.cpp
        src/demux/DemuxContext.cpp
        src/demux/stream/FFStream.cpp
        src/demux/demuxer/FFDemuxer.cpp
        src/demux/demuxer/DemuxerFactory.cpp
        src/demux/frame/FFFrame.cpp
        src/demux/frame/ResampledFrame.cpp
        src/tool/resample/FFResample.cpp
        src/tool/sdl/SDLManager.cpp
)
set(
        COMMON_LIB
        glog::glog
        gflags
        SDL2::SDL2
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
)
set(
        RELEASE_LIB
        ${COMMON_LIB}
)

set(
        TEST_SOURCE
        test/sample.cpp
        test/sdl.cpp
)
set(
        TEST_LIB
        GTest::GTest
        GTest::Main
)
########## init ##########

########## target ##########
# release
add_library(libplayer ${RELEASE_SOURCE})
set_target_properties(libplayer PROPERTIES LINK_FLAGS "-Wall -Wextra -Werror")
target_link_libraries(libplayer ${RELEASE_LIB})

add_executable(player src/main.cpp)
target_link_libraries(player libplayer)

# test
add_executable(unit_test ${TEST_SOURCE})
target_link_libraries(unit_test  ${TEST_LIB} libplayer)
########## target ##########
