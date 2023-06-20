

  ____  _            _                      _ _                  
 | __ )| | __ _  ___| | ___ __ ___   ___   | (_)_ __  _   ___  __
 |  _ \| |/ _` |/ __| |/ / '__/ _ \ / _ \  | | | '_ \| | | \ \/ /
 | |_) | | (_| | (__|   <| | | (_) | (_) | | | | | | | |_| |>  < 
 |____/|_|\__,_|\___|_|\_\_|  \___/ \___/  |_|_|_| |_|\__,_/_/\_\ rev 0.1
                                                                 

 Developed by chelson aitcheson with the help of the PSX development community

============================= What is Blackroo linux? ===========================

G'day mate and welcome to the blackroo linux read stuff file. this file is comprised of the collected data for the history of running linux on the playstation r3000 mips system from the old runnix team and the efforts of others in the community.

Blackroo linux is a opensource effort called "Runnix" 



============================= How to setup the toolchain ===========================

OK this is the dummies guide to doing this so deal with it and feel free to make changes to it.

my system ubuntu 20 lts, x86_64 hp-z400

sudo apt install flex

install binutils 2.95 by extracting it to a folder

put it in a folder and configure with 32bit linux

linux32 ./configure mips



//tc-i386.h error (gedit)
//linux


apt install gcc-mips-linux-gnu
ln -sf <path to mipsgnu directory>/

building the alpha 2

to build:
make clean

make mrproper

make menuconfig

make dep

make

Tails92 <tails92@gmail.com> on June 28th, 2008
*****************************************************************

Introduction
This HOWTO was began on November 4th, 2001 at 12:13AM by Antonio "Willy" Malara, known as BeHappy on IRC, and it wants to describe how to execute the Linux kernel 2.4.0 on the Sony Playstation (PSX), it will be able to be redistributed totally or partly according to the terms of the GPL license. I'd want to use this occasion to greet i8088, s|ckness and suppaman and more generally all the #linux-it channel of the IRCNet network.

Let's begin the opera
The PSX has a MIPS R3000A processor with about 33Mhz of clock without support for floating point operations, 2 MB of RAM for programs, 1 MB of RAM used for the framebuffer and another MB for sound (should this be fixed? afaik it's 512kb).
The architecture we're going to work on is then the mipsel one, i.e mips processors which use little-endian ordering. Now we could download SGI's kernel and work with that, but luckily a russian company, Runix (www.runix.ru) decided to make its hands dirty with dirtier work ;), then you'd better download the kernel port from their site. At this point it's the moment to setup the compilers, we still have two ways to take, either using the tarball of binaries that is supplied to us by dear Runix or patching the binaries like explained in the MIPS-HOWTO. Personally in this first moment I've decided to use Runix's set, also if I'll have to compile gcc and binutils myself soon, in the search for the odd 'flat' binary format that seems the only one compatible with PSXLinux, like we'll see later, anyway.

=============================Configuration and compilation===========================

Now that we have the system ready for cross-compilation for mipsel architectures we can extract the file `PSXLinux-kernel-2.4.x-alpha2.tar.gz' and we'll obtain the usual Linux source tree,


 let's do the usual make menuconfig and

let's proceed with our configuration by being careful to not select "PlayStation SIO console" in the menu "PlayStation Character devices" unless we have a PSX-serial -> PC-serial adapter (more below I'll offer the link with the scheme that you can use to build it yourself). 

If you don't have the serial cable, selecting the entries `RAM disk support' e ` Initial RAM disk (initrd) support', both in the `Block devices' menu, is necessary.

In `README.PSXLinux' we find some information that we readily follow:

Code maturity level options:
Prompt for development and/or incomplete code/drivers
- YES

Machine selection:
Support for Sony Playstation
- YES

Loadable module support:
Enable loadable module support
- NO

CPU selection:
CPU type
- R3000

General setup:
Kernel floating-point emulation
- YES
Kernel support for flat binaries
- YES
Kernel support for a.out binaries
- NO
Kernel support for ELF binaries
- NO
Kernel support for MISC binaries
- NO

Block devices:
PSX memory card support
- YES

File systems:
Second extended fs support
-YES

Console drivers:
PSX GPU virtual console
- YES
Virtual terminal
- YES
Support for console on virtual terminal
- YES

Kernel hacking:
Are you using a crosscompiler
-YES

At this point we can execute the `make' command - specifying bzImage or some other thing like it's pointed out at the menuconfig utility exit is not required.
If everything went fine we'll have an ELF binary called `linux'.
`README.PSXLinux' says that now we can make our PlayStation execute the binary, but the descriptions about how to do that are full of gaps: now the are two possible ways, either using the serial cable or burning a CD. It's here that the pains come, because the programs supplied by the Runix company require this famous serial cable, if you have it you can very well use the Runix program `psx-serial' that includes the PSX side program supplied as an ISO image to burn and the PC side program in binary-only format, that among other things makes the PSX execute programs that are in the ELF format, if you don't have the "magic" cable (like me) you'll have to follow a bit more obscure path that will take us to the goal.

How to prepare an ISO with our kernel
Well, we only have a kernel compiled in ELF format. The grey PlayStation, the one we have in the living room to play, has its own executable format called PS-EXE; some time ago Sony released a black-coloured version of the PSX, called `Net Yaroze', sold as an amateur development kit. This package included the Net Yaroze PlayStation, the "mystic" serial cable, and the GNU compilers (indeed, GNU) and the development libraries; this PSX used the ECOFF executable format, like some SGI machines, but because the Net Yaroze project was closed number and it was required to sign a contract that disallowed the distribution of one's own programs to anyone except for Net Yaroze members to enter it, some demo coder groups developed tools to help the one who wanted to take the feat of programming the PSX, and among those tools their a program called `eco2exe' which converts the Yaroze executables, that is ECOFF binaries executable by the Grey PSX, in PS-EXE.
What is the relation between this and our goal that is seeming always farther? Well, going for the `PSXLinux/' I found some little programs that are going to be very useful for us in the future, for example in `PSXLinux/arch/mipsnommu/boot' there are the `elf2ecoff.c' and `addinitrd.c' files that we'll need later.
Let's compile the two programs with:
gcc -o elf2ecoff elf2ecoff.c

gcc -o addinitrd addinitrd.c

and let's convert the `linux' file that was obtained with the compilation in ECOFF format with:
elf2ecoff linux linux.ecoff

At this point to have a correctly working Linux system, a root filesystem is needed. In Runix's project the root fs should be inserted in the PSX memory card, and it provides the 'psx-mcard' program that also requires the serial cable and is distributed as PSX ISO and PC binaries. Extracting the 'psx-mcard.0.8.2.tar.gz' file we find 'mem_card.example', a small but working ext2 filesystem that can be used as root filesystem or as an hint to create a new one. Now we can use the 'addinitrd' program to 'fuse' the root filesystem with the kernel image, because at the the current state of Runix we can't access the cd reader of the PSX, by using the command:
addinitrd linux.ecoff mem_card.example linux.image.ecoff

Now we have a kernel image with an initrd in ecoff format. Now we are forced to move on the Windows platform, real or emulated, to prepare the final ISO that can be burned. By having the linux.image.ecoff file available on Windows we can give the command:
eco2exe linux.image.ecoff

to transform the binary in PS-EXE format, and we rename it in a more canonical 8.3 'linux.exe' to avoid nuisances both on Windows and on the PSX.

At this point we can test the result of our efforts with a PSX emulator, to verify the correctness of the operations.
Now, to burn the final cdrom we have to do a last step, because Sony's cd reader can't read executable files that aren't multiplies of 2048 bytes, and for this we use another tool, exefixup:
C:> exefixup linux.exe

[...]
[it asks us if we want to save the padded file as padded.exe]? yes

C:> move padded.exe psx.exe

Now we have the psx.exe filled of zeros (nop in mips assembly) until being a multiply of 2048 bytes. With our favourite burning program we create an ISO XA (Mode 2) image that contains the psx.exe file, nothing else is needed because the PSX executes the psx.exe file automatically if it's present in the CD root. After getting the ISO image we have to do the last step before being able to burn it, the licensing, that is making the PSX execute the CD, so we use the windows program bootedit2 that makes us able other than to license the image also to change the string displayed at the disk boot, and eventually the image displayed at the top of the screen, too. After this last step we can burn the ISO and run to see what we have just done ;).

Conclusions
We have a kernel that the Sony PlayStation can execute and nothing else, because the Runix company's root fs redirects the console on the serial port (at the next update of this HOWTO I'll describe how to redirect the console to the PSX GPU).
But I think we've done an important step, because an operating system without kernel is nothing :).

TODO:
inserting the links that for the moment I don't have with me
trying to compile gcc with support for flat binaries
finding an utility to be ran by Linux for the PlayStation

Antonio "Willy" Malara
(BeHappy_ on IRC)
(finished on November 4th, 2001 at 3:15AM)
