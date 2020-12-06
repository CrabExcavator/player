# Player

## Overview

It's free media player that aiming at supporting Iot device or embedded in other software

## Todo

+ ~~audio ouput demo~~
+ ~~doc support(use doxygen)~~
+ ~~error code~~
+ ~~event handler~~
+ ~~dispatch Stream to Output~~
+ ~~resample~~
+ support packed audio format
+ thread pool for decode & blit & resample
+ video sync with audio
+ serializer
+ config
+ object pool
+ REPL
+ osd
+ ao & vo ReConfig
+ seek
+ network streaming
+ cache
+ p2p support?

## Bugs

+ audio mixed with next entry
    + CAUSE: Buffer(for ) is read with fixed size but bytes in Buffer is not enough
    + HOW TO FIX: at last frame we should make up audio callback buffer

+ resample need flush
    + [refer](https://blog.csdn.net/qq_34305316/article/details/106459849)
  
+ sdl audio driver
    + there is not sound if callback stuck for a period

## TEST

### Unit Test

+ correction of function
+ correction of context

### Integration Test

+ multiple driver
+ multiple hardware

## Source Code Overview

the source files are organized by different usage in different folder and designed
by traditional OOP that the src code that impl one feature should not cross 
too many files, the dst code can though. Anyway, we should consider where to put
the src code and to fix it in.

### The Folder

+ audio: audio output and audio driver
+ common: some high level data structure like Config that is used by other module
+ core: what make player different from demuxer
+ demux: split stream from container
+ exception: define err code
+ input: define input code and the
+ misc: some low level help util and function
+ osal: operating system abstraction layer
+ video: video output and video driver

### Name

you can guess what it is aimed for when you see name in src code first time

#### class

+ xxxContext: it mean that it can be accessed by many other class, it must always
be in the right state 

## EXPR

+ how to support different demuxer
    + WHY: because we should support a lot of format in the future
    + HOW: the first sight is to use default ffmpeg demuxer by default but allow user to override their own. 
    user should define their own codec though.

## Reference

[[VLC] module loading](https://wiki.videolan.org/Documentation:VLC_Modules_Loading/)

[[VLC] hack guide](https://wiki.videolan.org/Hacker_Guide/)

[[VLC] how to write module](https://wiki.videolan.org/Hacker_Guide/)

[[MPV] Display synchronization](https://github.com/mpv-player/mpv/wiki/Display-synchronization)

[Optimizing CRC32](https://merrymage.com/lab/crc32/)