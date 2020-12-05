//
// Created by CrabExcavator on 2020/12/2.
// Copyright (c) 2020 Studio F.L.A. All rights reserved.
//

#ifndef PLAYER_SRC_TOOL_SDL_SDLMANAGER_H_
#define PLAYER_SRC_TOOL_SDL_SDLMANAGER_H_

#include <memory>
#include <string>
#include <functional>
#include <SDL2/SDL.h>

#include "misc/GlobalInstance.h"
#include "common/Error.h"

namespace tool::sdl {

using window_uptr = std::unique_ptr<SDL_Window, std::function<void(SDL_Window*)>>;

using renderer_uptr = std::unique_ptr<SDL_Renderer, std::function<void(SDL_Renderer *)>>;

using texture_uptr = std::unique_ptr<SDL_Texture, std::function<void(SDL_Texture *)>>;

using audio_stream_uptr = std::unique_ptr<SDL_AudioStream, std::function<void(SDL_AudioStream *)>>;

/// @attention it's not a CTRP pattern
class SDLManager : public misc::GlobalInstance<SDLManager> {
 public:
  static std::unique_ptr<SDLManager> &GetInstance();

  common::Error Init();

  common::Error Destroy() const;

  common::Error CreateWindow(const std::string &window_name,
                                    int window_width,
                                    int window_height,
                                    window_uptr &window) const;

  common::Error CreateRenderer(const window_uptr &window, renderer_uptr &renderer) const;

  common::Error RenderClear(const renderer_uptr &renderer) const;

  common::Error SetRenderDrawColor(const renderer_uptr &renderer,
                                   uint8_t r,
                                   uint8_t g,
                                   uint8_t b,
                                   uint8_t a) const;

  common::Error CreateTexture(const renderer_uptr &renderer,
                              SDL_PixelFormatEnum texture_fmt,
                              int image_pitch,
                              int image_height,
                              texture_uptr &texture) const;

  common::Error SetTextureBlendMode(const texture_uptr &texture, SDL_BlendMode blend_mode) const;

  common::Error LockTexture(const texture_uptr &texture,
                            const SDL_Rect *rect,
                            void *&pixels,
                            int &pitch) const;

  common::Error UnlockTexture(const texture_uptr &texture) const;

  common::Error RenderCopy(const renderer_uptr &renderer, const texture_uptr &texture) const;

  common::Error RenderPresent(const renderer_uptr &renderer) const;

  common::Error WaitEventTimeout(int timeout_ms, SDL_Event &event) const;

  common::Error CreateAudioStream(SDL_AudioFormat src_format,
                                  Uint8 src_channels,
                                  int src_rate,
                                  SDL_AudioFormat dst_format,
                                  Uint8 dst_channels,
                                  int dst_rate,
                                  audio_stream_uptr &audio_stream) const;

  common::Error AudioStreamPut(const audio_stream_uptr &audio_stream,
                               uint8_t *data,
                               int size) const;

  common::Error AudioStreamAvailable(const audio_stream_uptr &audio_stream, int &avail_bytes) const;

  common::Error AudioStreamGet(const audio_stream_uptr &audio_stream,
                               uint8_t *&data,
                               int size,
                               int &gotten) const;

  common::Error AudioStreamFlush(const audio_stream_uptr &audio_stream) const;

  common::Error AudioStreamClear(const audio_stream_uptr &audio_stream) const;

  common::Error OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) const;

  common::Error PauseAudio(int pause_on) const;
 private:
  SDLManager();

 private:
  bool inited_;
};

}

#endif //PLAYER_SRC_TOOL_SDL_SDLMANAGER_H_
