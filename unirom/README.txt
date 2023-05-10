nops.exe, ELFSharp.dll, nops
Serial tool to send stuff to the PSX, see https://github.com/JonathanDotCel/NOTPSXSerial

schematic.png
One of many possible serial cables you could build.
See https://unirom.github.io/serial_psx_cable/

unirom_standalone.rom
Every cheat cart is big enough to hold this rom.

unirom_withcaetla.rom
If your cart is 256kb or higher, then you can run Unirom and Caetla side by side.

unirom_datelv2_withcaetla.rom
For Datel V2 carts (uni will tell you if your cart is V2), this .rom adjusts to the slightly different memory layout. (Not required for V1 or V3)

unirom_b.exe
This is the main unirom .exe when loaded from the bootdisc.
It does not attempt to unlock the CD drive since it has already booted from CD.

unirom_r.exe
This is the main unirom .exe when loaded from rom.
This .exe will attempt to unlock the CD drive for you, and so is used in projects like OpenBIOS and FreePSXBoot.

danhans_pong.psx
Dan made it, you can send it to the psx via serial using nops. Neat.

exe2memcard
run homebrew from the memcard - see README in the exe2memcard dir


