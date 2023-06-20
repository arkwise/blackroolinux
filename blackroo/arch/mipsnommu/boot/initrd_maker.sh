 # ____  _            _                      _ _                  
 #| __ )| | __ _  ___| | ___ __ ___   ___   | (_)_ __  _   ___  __
 #|  _ \| |/ _` |/ __| |/ / '__/ _ \ / _ \  | | | '_ \| | | \ \/ /
 #| |_) | | (_| | (__|   <| | | (_) | (_) | | | | | | | |_| |>  < 
 #|____/|_|\__,_|\___|_|\_\_|  \___/ \___/  |_|_|_| |_|\__,_/_/\_\ rev 0.11

 #the initrd of life


                                                                 

#!/bin/bash

# Housekeeping...
rm ‑f /tmp/ramdisk.img
rm ‑f /tmp/ramdisk.img.gz

# Ramdisk Constants
RDSIZE=128
BLKSIZE=8192

# Create an empty ramdisk image
dd if=/dev/zero of=/tmp/ramdisk.img bs=$BLKSIZE count=$RDSIZE

# Make it an ext2 mountable file system
/sbin/mke2fs ‑F ‑m 0 ‑b $BLKSIZE /tmp/ramdisk.img $RDSIZE

# Mount it so that we can populate
mount /tmp/ramdisk.img /mnt/initrd ‑t ext2 ‑o loop=/dev/loop0

# Populate the filesystem (subdirectories)
mkdir /mnt/initrd/bin
mkdir /mnt/initrd/sys
mkdir /mnt/initrd/dev
mkdir /mnt/initrd/proc

# Grab busybox and create the symbolic links
pushd /mnt/initrd/bin
cp /usr/local/src/busybox‑1.1.1/busybox .
ln ‑s busybox ash
ln ‑s busybox mount
ln ‑s busybox echo
ln ‑s busybox ls
ln ‑s busybox cat
ln ‑s busybox ps
ln ‑s busybox dmesg
ln ‑s busybox sysctl
popd

# Grab the necessary dev files
cp ‑a /dev/console /mnt/initrd/dev
cp ‑a /dev/ramdisk /mnt/initrd/dev
cp ‑a /dev/ram0 /mnt/initrd/dev
cp ‑a /dev/null /mnt/initrd/dev
cp ‑a /dev/tty1 /mnt/initrd/dev
cp ‑a /dev/tty2 /mnt/initrd/dev

# Equate sbin with bin
pushd /mnt/initrd
ln ‑s bin sbin
popd

# Create the init file
cat >> /mnt/initrd/linuxrc << EOF
#!/bin/ash
echo
echo "Simple initrd is active"
echo
mount ‑t proc /proc /proc
mount ‑t sysfs none /sys
/bin/ash ‑‑login
EOF

chmod +x /mnt/initrd/linuxrc

# Finish up...
umount /mnt/initrd
gzip ‑9 /tmp/ramdisk.img
cp /tmp/ramdisk.img.gz /boot/ramdisk.img.gz