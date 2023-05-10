# mips-binutils 2.9.5
### Build options for mips target
```sh
./configure --target=mips-linux --prefix=/opt/cross --disable-gprof --disable-gdb --disable-werror --host=i386-pc-linux --build=i386-pc-linux

make CFLAGS="-static -fno-stack-protector -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0"
```

### Only needed files

- **ar** - `binutils/ar`
- **as** - `gas/as-new`
- **ld** - `ld/ld-new`
- **strip** - `binutils/strip-new`
- **objcopy** - `binutils/objcopy`
