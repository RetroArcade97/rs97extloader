#!/bin/sh
#
# This script checks the external SD for a bootable chroot
#
rm=/bin/rm
mount=/bin/mount
umount=/bin/umount
touch=/bin/touch
mknod=/bin/mknod
mkdir=/bin/mkdir
sleep=/bin/sleep
grep=/bin/grep
chroot=/usr/sbin/chroot
autopatch=/usr/share/plash/autopatch.sh
intboot=/usr/share/plash/intboot.sh

blkdev=/dev/mmcblk1p1
mntdir=/mnt


result() {
  set +x
  echo "#NEXT#" "$@"
  exit 0
}
mount_ext_sd() {
  $mount -t ext3 -o ro $blkdev $mntdir && return 0
  $mount -t vfat -o rw,utf8,noatime $blkdev $mntdir && return 0
  return 1
}
run_probe() {
  exec 2>&1
  echo CUSTOM RC '*******************************************************'
  : Probing External SD card
  if mount_ext_sd ; then
    ls -l $mntdir
    set -x
    probe_autoexec
    probe_chroot
    probe_patched_fw
    probe_unpatched_fw
    $umount $mntdir
  fi
}
probe() {
  local output="$(run_probe)"
  local res=$(echo "$output" | (
	last=""
	while read ln
	do
	  [ -w /dev/kmsg ] && echo "$ln" >/dev/kmsg
	  echo "x - $ln" 1>&2
	  last="$ln"
	done
	echo "$last"
      ))
  set -x
  set - $res
  [ x"$1" != x"#NEXT#" ] && return
  shift
  echo "$@"
}

probe_autoexec() {
  [ -x $mntdir/autoexec.sh ] && result run_autoexec
}
run_autoexec() {
  exec $mntdir/autoexec.sh "$@"
}
probe_chroot() {
  [ -x $mntdir/chroot.sh ] && result run_chroot
}
run_chroot() {
  cd $mntdir
  exec .$chroot . ./chroot.sh "$@"
}
probe_patched_fw() {
  $grep -q /dev/mmcblk1 $mntdir/etc/inittab && result run_patched_fw
}
run_patched_fw() {
  cd $mntdir
  exec .$chroot . ./sbin/init "$@"
}
probe_unpatched_fw() {
  $grep -q /dev/mmcblk1 $mntdir/etc/inittab && return
  $grep -q /dev/mmcblk0 $mntdir/etc/inittab && result run_unpatched_fw
}
run_unpatched_fw() {
  # This is not patched...
  $umount $mntdir
  $autopatch
}

#~ env=/usr/bin/env
#~ plash=/usr/bin/plash
#~ img=/usr/share/kickstart.png
#~ plash_pid=""
#~ splash_on() {
  #~ [ -x $plash ] || return
  #~ [ -f $img ] || return
  #~ [ -e /dev/fb 0 ] || return

  #~ $env SDL_NOMOUSE=1 $plash -f$img -d0 &
  #~ plash_pid="$!"
#~ }
#~ splash_off() {
  #~ [ -n "$plash_pid" ] && kill "$plash_id"
#~ }
#~ splash_on
#~ splash_off

probe_out=$(probe)
echo $probe_out >/dev/kmsg
$probe_out
$umount $mntdir

exec $intboot "$@"
