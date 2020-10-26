# Player

## Overview

It's free media player that aiming at supporting Iot device or embedded in other software

## Todo

+ module support
    + abstract class in core directory
    + some impl
    + dynamic load module

+ ~~audio ouput demo~~
+ dynamic load module
+ video sync with audio
+ serializer
+ config
+ ao & vo reconfig
+ REPL
+ osd
+ seek
+ network streaming
+ cache
+ p2p support?

## Thinking

+ how to support different demuxer

## Reference

[[VLC] module loading](https://wiki.videolan.org/Documentation:VLC_Modules_Loading/)

[[VLC] hack guide](https://wiki.videolan.org/Hacker_Guide/)

[[VLC] how to write module](https://wiki.videolan.org/Hacker_Guide/)

[[MPV] Display synchronization](https://github.com/mpv-player/mpv/wiki/Display-synchronization)

[Optimizing CRC32](https://merrymage.com/lab/crc32/)