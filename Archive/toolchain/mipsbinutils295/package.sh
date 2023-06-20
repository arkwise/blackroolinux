#!/bin/sh -e

./configure --target=mips-linux --prefix=/opt/cross --disable-gprof --disable-gdb --disable-werror --host=i386-pc-linux --build=i386-pc-linux

make CFLAGS="-static -fno-stack-protector -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0"

mkdir pkg
cd pkg
cp -v ../binutils/ar ar
cp -v ../gas/as-new as
cp -v ../ld/ld-new ld
cp -v ../binutils/strip-new strip
cp -v ../binutils/objcopy objcopy
strip -v ar as ld strip objcopy

tar -czf ../mips-binutils-2.9.5.tar.gz ar as ld strip objcopy
cd ..
