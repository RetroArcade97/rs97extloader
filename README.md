# rs97extloader

Coolboy RS-97 External SD loader.

This project implements a modification to the standard firmware for
the [Coolboy RS-97][rs97] handheld.  After this is installed,
you can boot to a new image through the external sdcard slot.  If
no sdcard is recognized, it will continue with the built-in internal
sdcard firmware.

This lets you try a new firmware for your device without having
to open it.

NOTE, the external SD image usually needs to be patched so that
it will be able to boot from the external SD slot.

## Installation

1. Download the distributed `rs97extloader-X.XX.zip` archive from the
   [rs97extloader][releases] download page.
2. Unpack the archive to a `FAT32` sdcard.  If you plan to backup your
   `rootfs` make sure that you have at least **200MB** of available
   space.
3. Insert the sdcard into the external slot of your handheld.
4. **OPTIONAL BACKUP**:  
   Locate the `backup.dge` in your sdcard using the
   built-in "File browser" app and `Open` it.  This will make a
   backup of the original `rootfs` to an archive in your external
   sdcard.  
   The backup can take quite a few minutes to complete.
4. Locate the `install.dge` in your sdcard using the built-in
   "File browser" app and `Open` it.  This will install the `extloader`
   script.
5. Reboot your handheld.  For your convenience, a `reboot.dge` is
   included.

Now, you can continue using your device as normal.  However, the next
time you turn-on your device with a "bootable" sdcard in the external
slot, it will boot that instead.
   
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

## Bootable sdcards

A bootable sdcard can be one of the following:

- An opendingux image that boots in mmcblk1. You can hack your own
  or download `opendingux.vX.XX_fw_mmcblk1.7z` from the
  [rs97extloader][releases] downloads page.  You must write this
  image to an empty sdcard using something like Win32Imager.  You
  may need to resize your partition with something like Minitool
  Partition Wizard.
- An stock image that boots in mmcblk1. You can hack your own
  or download `retrogame_OGFW_XXXXXXX_mmcblk1.7z` from the
  [rs97extloader][releases] downloads page.  You must write this
  image to an empty sdcard using something like Win32Imager.  You
  may need to resize your partition with something like Minitool
  Partition Wizard.
- An sdcard that has in its first primary partition an ext3
  filesystem with a rootfs and a executable `chroot.sh`.
  The [extloader][extloader] will perform a `chroot` to that file
  system before passing control to this script.
- An sdcard that has in its first primary partition with a FAT32
  fileystem with a file named `autoexec.sh`, or an sdcard that
  has in its primary partition with a ext3 filesystem with an
  executable file named `autoexec.sh`.
  In this case, the `autoexec.sh` can set-up the run-time environment
  as needed.  (i.e. set-up additional block devices or load a new
  kernel through `kexec`).  
  For your convenience an `opendingux` converted to `autoexec.sh`
  image: `opendingux_v1.3_autoexec.zip` is provided at
  [rs97extloader][releases] downloads page.  To use simply unzip
  the contents of the archive into an *EMPTY* FAT32 formatted
  sdcard.

## Converting existing firmware images

**NOTE**: At the moment, this is only possible on Linux.

To convert an image is quite simple:

1. If your image is in `.7z` format, you must first extract it to `.img`.
2. Use the command:
   `sed -i.backup 's/mmcblk0/mmcblk1/' $input_file`

Alternatively you can use the script `rs97cnv` from the [github][rs97]
repository.

Usage:

- `./rs97cnv patch $input_img $output_img`
- This will quickly convert firmware image from `mmcblk0` to `mmcblk1`.
- `./rs97cnv remix_cfw $input_img $output_zip`
- This will extract a firmware image into a `autoexec.sh` image.

## The `extloader` script

This script will replace the root filesystem `/sbin/init` command, so
it runs early in the boot process.  The source for this script can
be found in `files/extloader.sh` or on-line using [github][extloader_src].

## Known Issues and Limitations

- Because everything is running from the external sdcard, it can't
  be removed while the system is running.
- `extloader` will make it look to the firmware that the external sdcard
  is the internal sdcard.  So external sdcard will look empty, while
  all your content would be visible from the "internal" sdcard.

## FAQ

- Q) Do I need a special image for different hardware types?
- A) No, the hardware specific bits are always loaded from
     the internal sdcard.  
     Normally LCD display drivers are loaded into the kernel by
     u-boot before `extloader` gets invoked. So they will get
     the drives you are already using.  The exception is that
     if you have an `autoexec.sh` script that uses `kexec` to
     load a different kernel.  If you are able to do that, then
     you wouldn't be asking this question.

## Changelog

- 0.91 : Update
  - Added addition device nodes (for implementing splash screens)\
  - Fixed installation with opendingux
- 0.90 : First public release

## TODO

- Boot a new kernel through kexec
- Test if the installer will work on top of OpenDingux
- Internal micro image option for 2GB sdcards
- Splash screen
- Detect if it is a firmware image that boots into mmcblk0 and
  patch it automatically.

## References

* Original [splash][splash]
* About the [Coolboy RS-97][rs97]
* [OpenDingux][cfw]

[rs97]: http://rs97.wikia.com/wiki/Main_Page
[splash]: https://github.com/steward-fu/gh_retrogame_emulator/splash
[extloader]: https://github.com/HyperTechnology5/rs97extloader
[cfw]: https://jutleys.wixsite.com/retrogamers97-90
[releases]: https://github.com/HyperTechnology5/rs97extloader/releases/latest
[extloader_src]: https://github.com/HyperTechnology5/rs97extloader/blob/master/extloader/extloader.sh




