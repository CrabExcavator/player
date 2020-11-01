# Player

## Overview

It's free media player that aiming at supporting Iot device or embedded in other software

## Todo

+ module support
    + abstract class in core directory
    + some impl
    + dynamic load module

+ ~~audio ouput demo~~
+ refactor
    + some logic code in driver should put in VideoOutput && AudioOutput
    + concrete operation with frame should put in filter
    + Output && Stream should both have chain
+ error code
+ video sync with audio
+ serializer
+ config
+ ao & vo reconfig
+ play list
+ REPL
+ osd
+ seek
+ network streaming
+ cache
+ p2p support?


## Bugs

+ audio mixed with next entry
    + CAUSE: Buffer(for ) is read with fixed size but bytes in Buffer is not enough
    + HOW TO FIX: at last frame we should make up audio callback buffer

## Thinking

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