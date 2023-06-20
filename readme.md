

  ____  _            _                      _ _                  
 | __ )| | __ _  ___| | ___ __ ___   ___   | (_)_ __  _   ___  __
 |  _ \| |/ _` |/ __| |/ / '__/ _ \ / _ \  | | | '_ \| | | \ \/ /
 | |_) | | (_| | (__|   <| | | (_) | (_) | | | | | | | |_| |>  < 
 |____/|_|\__,_|\___|_|\_\_|  \___/ \___/  |_|_|_| |_|\__,_/_/\_\ rev 0.2
                                                                 
# 1 Intro
# 2 Toolchain setup
# 3 Building the alpha 3
# 4 Kernel Conversion
# 5 Initrd, RootFS & Ramdisk

 Developed by chelson aitcheson with the help of the PSX & Linux development community

============================= What is Blackroo linux? ===========================


#################
# 1.0 intro #####
#################


G'day mate and welcome to the blackroo linux read stuff file. this file is comprised of 
the collected data for the history of running linux on the playstation r3000 mips system 
from the old runix team and the efforts of others in the community.

Blackroo linux is based on a opensource effort called "Runix" that is long since dead since around 
2007ish. unfortunately they didint leave much to work with and even tho the kernel builds 
the root fs is still illusive as it required running the sysytem from a RAID of playstation 
memory cards via the multiport boomarangs originally.

Blackroo linux is an attempt to make this run from the upgraded EDO RAM (4mb and 8mb upgrades). 
the system can be loaded from a ramdisk.

now it will also be possible to use memory cards in a RAID configuration but this will not be the direction
of the blackroo distro.

uclinux is the base of the kernel developed originally for the PSP machines. while they also did use the 
2.6 kernel for the time being we wil use 2.4

not much info was left in a single place to make any of this happen but a long read of different
clues, docs, scripts, code and random google searches to put this all together as best as we could.

 



============================= How to setup the toolchain ===========================

OK this is the dummies guide to doing this so deal with it and feel free to make changes to it.


##################################
# 2.0 Development TOOLCHAIN ######
##################################


the following file is the original toolchain for RUNIX

 gzip -cd mipsel-linux-cross-i586.tar.gz | tar xf -

while this toolchain is specifically for (RUNIX) mipsel-linux-gcc ity is missing the mips-linux-xxx version of it
so you need to download the newer version for BLACKROO which is blackroo_mips_i586v1.tar.gz.

gzip -cd blackroo_mips_i586v1.tar.gz | tar xf -

The build tools MUST be extracted from root into the / directory and it will extract 
into /usr and /usr/local. 

while you can just export your paths to where ever you will run into nothing but dramas so 
its just easier this way.


###########################
# 2.1 BLACKROO LINUX ######
###########################

next lets unpack the damn kernel and its tools. i like to just keep it in my username folder or you could just have
it in your github folder. either way makesure you set the paths up.

gzip -cd blackroo_alpha3.tar.gz | tar xf -

Create the two symbolic links in /usr/local/include/ to blackroo/include folders asm & linux

ld -sf /home/username/blackroo/include/asm asm


ld -sf /home/username/blackroo/include/linux linux


#######################
# 2.2 Conclusion ######
#######################


this should be everything you need to get your system up and ready to build blackroo linux. if there 
are errors here just update them or add to this first step.


##################################
# 3.0  building the alpha 3 ######
##################################


to build:

the toolchain is already compiled and available from https://github.com/arkwise/blackroolinux

edit the loadr.sh file to set your $PATH (this is for where ever you have extracted the kernel)

now you should be ready to start compiling the kernel

Make mrproper

Make clean

Make menuconfig (use the configure file that is included) (is the asm links are not correct this will fail and ask you to install ncurses library)

the 'config' file is available in the main blackroo folder.

make dep

make

if it doesnt build check your settings or reach out to the community there could be a million reasons its not working its a pain 
in the butt unless you are an experienced linux nerd


##################################################################
# 4.0 Converting the Linux kernel into an ECOFF yaroze file ######
##################################################################


ok so lets move the newly created kernel to blackroo/arch/mipsnommu/boot

we need to then build elf2ecoff

gcc -o elf2ecoff elf2ecoff.c

After this we can then run " elf2ecoff linuxkernelname linux.ecoff


#################################################
# 4.1 Adding an initrd to the KERNEL ECOFF ######
#################################################


lets compile the add initrd

gcc -o addinitrd addinitrd.c



############################################
# 4.2 converting to PS EXEUTABLE file ######
############################################


You need to get a copy of exe fixup used EXEFIXUP.EXE to make the PS-EXE 
a multiple of 2048 bytes, and booted the kernel image (PS-EXE) from PSIO, but anything would work 
(CD-ROM, PSXSERIAL, PSEXE.COM or CATFLAP, etc).

So, I can get linux and linux.ecoff but cant seem to get a working ps1.exe
After using elf2ecoff (w/o -a), I've tried
ECO2EXE (w/o) (without it doesnt create PS-X EXE header and exefixup wont work)
mkboot (from arch\mipsnommu\boot) (w/o) (don't know what this does, but didnt help)
Then EXEFIXUP


Any ideas?


################################################
# 5.0 Initrd + Rootfs + raid memory cards ######
################################################ 


So this is the basic idea on building a new ROOTFS. There is a new shell script file in the boot folder to start generating one.

dd if=/dev/zero bs=8192 count=16 of=memcard.mcd

mkfs -t fat -i 8192 memcard.img

techinically you get a fat12 drive i think but this might be a start as im pretty sure the ext2 format needs a bigger size that will work over raid


1) make sure loopback block devices are configured into the kernel
 2) create an empty file system of the appropriate size, e.g.
    # dd if=/dev/zero of=initrd bs=300k count=1
    # mke2fs -F -m0 initrd
