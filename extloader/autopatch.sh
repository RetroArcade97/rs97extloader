#!/bin/sh
#
# Automatically patch fw
#
pdir=/usr/share/plash
plash="$pdir/plash -F$pdir/font.ttf"

teepipes() {
  {  2>&3 | $2; } 3>&1 1>&2 | $3
}
read_cmd() {
  dd if=/dev/mmcblk1p1 bs=32k | sed 's/mmcblk0/mmcbk1/' | tee /proc/self/fd/2
}
write_cmd() {
  dd of=/dev/mmcblk1p1 bs=32k
}
status_cmd() {
  local \
	cnt=0 \
	step=32 \
	total=$(awk '$4 == "mmcblk1p1" {print $3}' /proc/partitions)
  (while [ $cnt -lt $total ]
  do
    cnt=$(expr $cnt + $step)
    dd count=${step} bs=1024 >&-
    echo "$cnt/$total"
  done
  echo "DONE..."
  ) | $plash -d0 -i -f$pdir/orange.png
}

mount -t proc proc /proc
input=$($plash -d0 -f$pdir/ask.png)
if ( echo "$input" | grep -q btn_a ) ; then
  #$plash -f$pdir/kickstart.png -m"Autopatching NOT supported!" -d0
  teepipes read_cmd write_cmd status_cmd
  reboot
fi
umount /proc

