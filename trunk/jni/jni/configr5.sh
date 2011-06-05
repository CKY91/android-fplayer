#!/bin/bash

PREBUILT=/home/administrator/android/android-ndk-r5/toolchains/arm-eabi-4.4.0/prebuilt/linux-x86
PLATFORM=/home/administrator/android/android-ndk-r5/platforms/android-8/arch-arm

./configure --target-os=linux \
    --arch=arm \
    --enable-version3 \
    --enable-gpl \
    --enable-nonfree \
    --disable-stripping \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffserver \
    --disable-ffprobe \
    --disable-encoders \
    --disable-muxers \
    --disable-devices \
    --disable-protocols \
    --enable-protocol=file \
    --enable-avfilter \
    --disable-network \
    --disable-mpegaudio-hp \
    --disable-avdevice \
    --enable-cross-compile \
    --cc=$PREBUILT/bin/arm-eabi-gcc \
    --cross-prefix=$PREBUILT/bin/arm-eabi- \
    --nm=$PREBUILT/bin/arm-eabi-nm \
    --extra-cflags="-fPIC -DANDROID" \
    --disable-asm \
    --enable-neon \
    --enable-armv5te \
    --extra-ldflags="-Wl,-T,$PREBUILT/arm-eabi/lib/ldscripts/armelf.x -Wl,-rpath-link=$PLATFORM/usr/lib -L$PLATFORM/usr/lib -nostdlib $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtbegin.o $PREBUILT/lib/gcc/arm-eabi/4.4.0/crtend.o -lc -lm -ldl"
