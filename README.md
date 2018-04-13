# rs97extloader

Coolboy RS-97 External SD loader.

This project implements a modification to the standard firmware for
the [Coolboy RS-97][rs97] handheld.  After this is installed,
you can boot to a new image through the external SD card slot.  If
no sdcard is recognized, it will continue with the built-in internal
sdcard firmware.

NOTE, the external SD image usually needs to be patched so that
it will be able to boot from the external SD slot.

## Installation

1. Download the distributed `zip` archive and unpack it to a sdcard.
2. Insert the sdcard into the external slot of your handheld.
3. **OPTIONAL**: Locate the `backup.dge` in your sdcard using the
   built-in "File browser" app and `Open` it.  This will make a
   backup of the original `rootfs` to an archive in your external
   sdcard.  You need about **150M** of free space on your sdcard.
   The backup can take quite a few minutes to complete.
4. Locate the `install.dge` in your sdcard using the built-in
   "File browser" app and `Open` it.  This will install the `extloader`
   script.
5. Reboot your handheld.  For your convenience, a `reboot.dge` is
   included.
6. You can download pre-patched images from [rs97extloader][extloader]
   downloads page for testing.

## Uninstalling

It is only possible to uninstall if you created a backup using
`backup.dge`.  To un-install, use `restore.dge` to restore your
original firmware.

**NOTE**: Only the first partition of the firmware is backed-up.
The installer only makes changes to the first partition.  As it is,
the backup/restore scripts are only useful with the
[rs97extloader][extloader] scripts.

## Usage

Simply insert a bootable sdcard into your device, and power-on.

## Creating bootable sdcards

A bootable sdcard can be one of the following:

- A patched firmware image : This is an image like the stock image
  or the one used by [OpenDingux][cfw] that has been patched to boot
  from the external sd slot.  A script is provided to do this.
- A chroot filesystem : A partition formatted as `ext3` that
  contains an executable file named `chroot.sh` in its root.
  The [extloader][extloader] will perform a `chroot` to that file
  system before passing control to this script.
- A autoexec filesystem : A partition formatted as `ext3` or `vfat`
  containing an execuable file named `autoexec.sh`.  The `autoexec.sh`
  file can then set-up the run-time environment as needed.  (i.e.
  set-up additional block devices or load a new kernel through `kexec`).

## Converting existing firmware images

**NOTE**: At the moment, this is only possible on Linux.

To convert an image is quite simple:

1. If your image is in `.7z` format, you must first extract it to `.img`.
2. Use the command:
   `sed -i.backup 's/mmcblk0/mmcblk1/' $input_file`

## Changelog

- 0.90 : First public release

## TODO

- Create a OpenDingux remix
- Boot a new kernel through kexec
- Test if the installer will work on top of OpenDingux
- Micro image option

## References

* Original [splash][splash]
* About the [Coolboy RS-97][rs97]
* [OpenDingux][cfw]

[rs97]: http://rs97.wikia.com/wiki/Main_Page
[splash]: https://github.com/steward-fu/gh_retrogame_emulator/splash
[extloader]: https://github.com/HyperTechnology5/rs97extloader
[cfw]: https://jutleys.wixsite.com/retrogamers97-90


