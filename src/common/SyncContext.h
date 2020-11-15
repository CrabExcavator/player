//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SYNCCONTEXT_H
#define PLAYER_SYNCCONTEXT_H

#include <mutex>
#include <condition_variable>
#include <vector>

#include "common/Error.h"
#include "misc/typeptr.h"

namespace common {

/**
 * @brief keep different thread with proper tick rate
 */
class SyncContext {
 public:
  /**
   * @brief default
   */
  SyncContext();

  /**
   * @brief setNumOfStream
   * @param player_ctx
   * @return
   */
  common::Error Init();

  /**
   * @brief setNumOfStream
   * @param [in] size number of output
   */
  void setNumOfStream(int size);

  /**
   * @brief add video stream
   * @param [in] stream
   * @return error code
   */
  common::Error addVideoStream(const demux::stream::stream_sptr &stream);

  /**
   * @brief add audio stream
   * @param [in] stream
   * @return error code
   */
  common::Error addAudioStream(const demux::stream::stream_sptr &stream);

  /**
   * @brief add stream
   * @param [in] stream
   * @return error code
   */
  common::Error addStream(const demux::stream::stream_sptr &stream);

  /**
   * @brief get video stream
   * @param [out] stream
   * @return error code
   */
  common::Error getVideoStream(demux::stream::stream_sptr &stream);

  /**
   * @brief get audio stream
   * @param [out] stream
   * @return error code
   */
  common::Error getAudioStream(demux::stream::stream_sptr &stream);

  /**
   * @brief call in output thread for sync
   */
  void wait();

  /**
   * @brief when close called, all thread stopped by wait should
  w * start
   */
  void close();

 public:
  /**
   * @brief version used to check if all output is in correct state
   */
  uint64_t version = 0;

 private:
  /**
   * @brief flag to mark close
   */
  bool _close;

  /**
   * @brief size of SyncContext
   */
  int _size;

  /**
   * @brief used in wait
   */
  int _cnt;

  /**
   * @brief streams used in video output
   */
  misc::vector_sptr<demux::stream::stream_sptr> _video_streams;

  /**
   * @brief streams used in audio output
   */
  misc::vector_sptr<demux::stream::stream_sptr> _audio_streams;

  /**
   * @brief mutex
   */
  std::mutex _mutex;

  /**
   * @brief cond
   */
  std::condition_variable _cond{};
};

}

#endif //PLAYER_SYNCCONTEXT_H
