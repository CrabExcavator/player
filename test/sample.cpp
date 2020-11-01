//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <gtest/gtest.h>

#include "misc/Thread.h"
#include "misc/util.h"
#include "common/error.h"

TEST(SAMPLE, THREAD) {
    misc::Thread t1(std::string("hehe"));
    misc::Thread t2(std::move(t1));
    t2.run([](){
        LOG(INFO) << "Hello Thread";
    });
    t2.join();
}

TEST(SAMPLE, TO_STRING) {
    LOG(INFO) << misc::to_string(" a b c ", 233, "h asdadasd asdaposdkmas");
}

#include <chrono>

TEST(SAMPLE, CHRONO) {
    LOG(INFO) << static_cast<int>(common::error::eof);
    double raw = 2.23;
    auto seconds = std::chrono::duration<double>(raw);
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(seconds);
    LOG(INFO) << nanoseconds.count();
}

#include <portaudio.h>

typedef struct
{
    float left_phase;
    float right_phase;
} paTestData;
/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
*/
static int patestCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData )
{
    /* Cast data passed through stream to our structure. */
    paTestData *data = (paTestData*)userData;
    float *out = (float*)outputBuffer;
    unsigned int i;
    (void) inputBuffer; /* Prevent unused variable warning. */

    for( i=0; i<framesPerBuffer; i++ )
    {
        *out++ = data->left_phase;  /* left */
        *out++ = data->right_phase;  /* right */
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
        data->left_phase += 0.01f;
        /* When signal reaches top, drop back down. */
        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        /* higher pitch so we can distinguish left and right. */
        data->right_phase += 0.03f;
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
    }
    return 0;
}

static paTestData data;

TEST(SAMPLE, AUDIO) {
    auto err = Pa_Initialize();
    ASSERT_EQ(err, paNoError);
    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, 44100, 256, patestCallback, &data);
    ASSERT_EQ(err, paNoError);
    err = Pa_StopStream(stream);
    ASSERT_EQ(err, paNoError);
    err = Pa_CloseStream(stream);
    ASSERT_EQ(err, paNoError);
    err = Pa_Terminate();
    ASSERT_EQ(err, paNoError);
}

#include "misc/Buffer.h"

TEST(SAMPLE, BUFFER) {
    for (int i = 0 ; i < 1000 ; i++) {
        LOG(INFO) << "----------------------------------------------------------------";
        long long total_put = 0;
        long long total_get = 0;
        misc::Buffer<uint8_t, 10> buffer;
        std::array<uint8_t, 3> in{3, 2, 1};
        std::array<uint8_t, 3> x{1, 2, 3};
        misc::Thread thread("233");
        thread.run([&]() {
            for (int j = 0; j < 1000; j++) {
                buffer.get(x, 0, 3);
                ASSERT_EQ(x, in);
                total_get += 3;
            }
        });
        for (int j = 0; j < 1000; j++) {
            buffer.put(in, 0, 3);
            total_put += 3;
        }
        thread.join();
        LOG(INFO) << total_put << " " << total_get << " " << buffer.size();
    }
    std::array<uint8_t, 3> in{3, 2, 1};
    misc::Buffer<uint8_t , 100> buffer;
    misc::Thread thread;
    thread.run([&](){
        for (int i = 0 ; i < 100 ; i++) {
            buffer.put(in, 0, 3);
        }
    });
    std::this_thread::sleep_for(std::chrono::seconds(1));
    buffer.close();
    thread.join();
}

TEST(SAMPLE, DEFER) {
    DEFER([](){
        std::cout << "Hello World" << std::endl;
    });
    std::cout << "HEHE" << std::endl;
}