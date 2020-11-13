//
// Created by CrabExcavator on 2020/11/1.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_DRIVERPORTAUDIO_H
#define PLAYER_DRIVERPORTAUDIO_H

#include <portaudio.h>
#include <folly/MPMCQueue.h>

#include "AudioDriver.h"
#include "misc/Buffer.h"

namespace audio::driver {

/**
 * @brief the PortAudio impl of audio driver
 */
class DriverPortAudio : public AudioDriver {
 public:
  /**
   * @brief default construct
   */
  DriverPortAudio() = default;

  /**
   * @brief default deConstruct
   */
  ~DriverPortAudio() override;

  /**
   * @brief bind PortAudio playback callback & terminate callback
   * @param [in] ao audio output
   * @return error code
   */
  common::Error init(ao_sptr ao) override;

  /**
   * @brief put a frame in buffer if possible
   * @param [in] ao audio output
   * @return error code
   */
  common::Error play(ao_sptr ao) override;

  /**
   * @brief stop audio playback
   * @param [in] ao audio output
   * @return error code
   */
  common::Error stop(ao_sptr ao) override;

  /**
   * @brief do nothing for now
   * @param [in] ao audio output
   * @return error code
   */
  common::Error reConfig(ao_sptr ao) override;

  /**
   * @brief [todo] return list of devices
   * @param [in] ao audio output
   * @param [out] list of devices
   * @return error code
   */
  common::Error getDevices(ao_sptr ao, misc::vector_sptr<std::string> &devices) override;

 private:
  /**
   * @brief PortAudio terminate callback wrapper
   * @param [in] userData pointer to *this
   */
  static void paStreamFinished(void *userData);

  /**
   * @brief PortAudio terminate callback
   */
  void paStreamFinishedMethod();

  /**
   * @brief PortAudio playback callback wrapper
   * @param [in] inputBuffer
   * @param [out] outputBuffer
   * @param [out] framesPerBuffer
   * @param [in] timeInfo
   * @param [out] statusFlags
   * @param [in] userData
   * @return
   */
  static int paCallback(const void *inputBuffer, void *outputBuffer,
                        unsigned long framesPerBuffer,
                        const PaStreamCallbackTimeInfo *timeInfo,
                        PaStreamCallbackFlags statusFlags,
                        void *userData);

  /**
   * @brief PortAudio playback callback
   * @param [in] inputBuffer data from input device like microphone
   * @param [out] outputBuffer data to playback
   * @param [out] framesPerBuffer number of frame in inputBuffer of outputBuffer
   * @param [in] timeInfo timing info when callback is called
   * @param [out] statusFlags Flag bit constants for the statusFlags to PaStreamCallback
   * @return
   */
  int paCallbackMethod(const void *inputBuffer, void *outputBuffer,
                       unsigned long samplesPerBuffer,
                       const PaStreamCallbackTimeInfo *timeInfo,
                       PaStreamCallbackFlags statusFlags);

 private:
  /**
   * @brief PaStream Handle
   */
  PaStream *_stream = nullptr;

  /**
   * @brief buffer to pass bytes to PortAudio playback callback
   */
  misc::Buffer<uint8_t, 65536> _buffer;
};

}

#endif //PLAYER_DRIVERPORTAUDIO_H
