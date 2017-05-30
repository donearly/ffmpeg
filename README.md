#!/bin/bash
make clean
export SYSROOT=$NDK/platforms/android-21/arch-arm64/
export TOOLCHAIN=$NDK/toolchains/aarch64-linux-androideabi-4.9/prebuilt/linux-x86_64
export CPU=aarch64
export PREFIX=$(pwd)/android/$CPU
export ADDI_CFLAGS="-marm"
export NDK_PREBUILTLLVM=$NDK/toolchains/llvm/prebuilt/linux-x86_64

./configure --target-os=linux \
--prefix=$PREFIX --arch=aarch64 \
--cc=$NDK_PREBUILTLLVM/bin/clang \
--disable-doc \
--enable-shared \
--disable-static \
--disable-yasm \
--disable-symver \
--enable-gpl \
--disable-ffmpeg \
--disable-ffplay \
--disable-ffprobe \
--disable-ffserver \
--disable-doc \
--disable-symver \
--cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
--enable-cross-compile \
--extra-libs=-lgcc \
--sysroot=$SYSROOT \
--extra-cflags=" --target=aarch64-linux-android -O3 -DANDROID -Dipv6mr_interface=ipv6mr_ifindex -fasm -fno-short-enums -fno-strict-aliasing -Wno-missing-prototypes" \
$ADDITIONAL_CONFIGURE_FLAG
make clean
make
make install
