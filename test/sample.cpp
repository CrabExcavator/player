//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#include <gtest/gtest.h>

#include "misc/Thread.h"
#include "misc/util.h"
#include "common/Error.h"

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
    LOG(INFO) << static_cast<int>(common::Error::END);
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

//#include "misc/Buffer.h"
//
//TEST(SAMPLE, BUFFER) {
//    for (int i = 0 ; i < 1000 ; i++) {
//        LOG(INFO) << "----------------------------------------------------------------";
//        long long total_put = 0;
//        long long total_get = 0;
//        misc::Buffer<uint8_t, 10> buffer;
//        std::array<uint8_t, 3> in{3, 2, 1};
//        std::array<uint8_t, 3> x{1, 2, 3};
//        misc::Thread thread("233");
//        thread.run([&]() {
//            for (int j = 0; j < 1000; j++) {
//              buffer.Get(x, 0, 3);
//                ASSERT_EQ(x, in);
//                total_get += 3;
//            }
//        });
//        for (int j = 0; j < 1000; j++) {
//          buffer.Put(in, 0, 3);
//            total_put += 3;
//        }
//        thread.join();
//        LOG(INFO) << total_put << " " << total_get << " " << buffer.size();
//    }
//    std::array<uint8_t, 3> in{3, 2, 1};
//    misc::Buffer<uint8_t , 100> buffer;
//    misc::Thread thread;
//    thread.run([&](){
//        for (int i = 0 ; i < 100 ; i++) {
//          buffer.Put(in, 0, 3);
//        }
//    });
//    std::this_thread::sleep_for(std::chrono::seconds(1));
//    buffer.close();
//    thread.join();
//}

TEST(SAMPLE, DEFER) {
    DEFER([](){
        std::cout << "Hello World" << std::endl;
    });
    misc::Defer a([](int x){
      LOG(INFO) << x;
    }, 1);
    std::cout << "HEHE" << std::endl;
}

#include "misc/Channel.h"

TEST(SAMPLE, CHANNEL) {
  misc::Channel<100, int> channel;
  for (int i = 0 ; i < 100 ; i++) {
    channel.BlockingPut(i);
  }
  int val;
  for (int i = 0 ; i < 100 ; i++) {
    channel.BlockingGet(val);
    ASSERT_EQ(i, val);
  }
}

#include "misc/BlockingChannel.h"
TEST(SAMPLE, BLOCKING_CHANNEL) {
  int number_of_iterator = 1000000;
  int output;

  misc::BlockingChannel<int> blocking_channel;
  misc::Thread thread;
  thread.run([&](){
    for (int i = 0 ; i < number_of_iterator ; i++) {
      blocking_channel << i;
    }
  });
  for (int i = 0 ; i < number_of_iterator ; i++) {
    blocking_channel >> output;
    ASSERT_EQ(i, output);
  }
  thread.join();
}

#include "dag/DAGNodeWrapper.h"
#include "dag/DAGContext.h"
#include "dag/DAGRoot.h"

class SampleDAGNode : public dag::IDAGNode {
 public:
  common::Error Run() override {
    LOG(INFO) << "sample dag node " << x;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return common::Error::SUCCESS;
  }

 public:
  int x = 0;
};

TEST(SAMPLE, DAGWrapper) {
  auto dag_context = std::make_shared<dag::DAGContext>();
  auto dag_root = std::make_shared<dag::DAGRoot>();
  dag_root->Init(dag_context);
  auto dag_node_wrapper = std::make_shared<dag::DAGNodeWrapper>();
  dag_node_wrapper->Init(dag_context);
  for (int i = 0 ; i < 64 ; i++) {
    auto sample_dag_node = std::make_shared<SampleDAGNode>();
    sample_dag_node->x = i;
    dag_node_wrapper->Add(sample_dag_node);
  }
  dag_root->Add(dag_node_wrapper);

  auto new_dag_node_wrapper = std::make_shared<dag::DAGNodeWrapper>();
  for (int i = 0 ; i < 64 ; i++) {
    auto sample_dag_node = std::make_shared<SampleDAGNode>();
    sample_dag_node->x = i + 100;
    new_dag_node_wrapper->Add(sample_dag_node);
  }
  new_dag_node_wrapper->Add(dag_node_wrapper);
  dag_root->Add(new_dag_node_wrapper);
  dag_root->Wait();
}

TEST(SAMPLE, LAMBDA) {
  int a = 123;
  auto f = [&a] {LOG(INFO) << a;};
  a = 321;
  f();
}
