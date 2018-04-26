#!/bin/sh
#
# Continue internal boot
#
mount=/bin/mount
umount=/bin/umount
busybox=/bin/busybox

pdir=/usr/share/plash
plash=$pdir/plash

mntdir=/mnt
intdev=/dev/mmcblk0p4

cd /
$umount $mntdir

if $mount -t vfat -o rw,utf8,noatime $intdev $mntdir ; then
  if [ -f $mntdir/autoexec.sh ] ; then
    exec $mntdir/autoexec.sh "$@"
  fi
  $umount $mntdir
fi

exec $busybox init "$@"

#~ $plash -F$pdir/font.ttf -f$pdir/kickstart.png -D1500 -m'Booting internal FW...'

