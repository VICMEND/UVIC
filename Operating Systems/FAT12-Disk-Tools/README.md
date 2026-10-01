# FAT12 Disk Tools

**Language:** C

**Build environment:** Linux with `gcc` and `make`. The tools use POSIX-style paths and were written for a Linux lab. Build from the `a3` directory, which has the makefile and the shared helper.

Read a FAT12 disk image (`.IMA`), the file system used by MS-DOS, and provide four tools: print disk information, list files and directories, copy a file out of the image, and copy a file into the image.

## Breakdown

The sources in `a3/` are the ones to build. Copies of the `.c` files also sit in the parent folder.

- `a3/diskinfo.c` — prints the OS name, volume label, total size, free size, file count, number of FAT copies, and sectors per FAT
- `a3/disklist.c` — lists the files and directories in the image
- `a3/diskget.c` — copies one file out of the image into the current directory
- `a3/diskput.c` — checks free space, that the destination path exists, and that the name is free, then starts writing the new directory entry. The write path was left unfinished.
- `a3/helper.c`, `a3/helper.h` — shared FAT12 reads (boot sector, directory entries, cluster chain)
- `a3/Makefile` — builds all four programs

## Usage

```bash
cd a3
make
./diskinfo disk.IMA
./disklist disk.IMA
./diskget disk.IMA FILENAME.TXT
./diskput disk.IMA /SUBDIR/file.txt
```

`disk.IMA` is any FAT12 image in the same directory. `diskget` copies `FILENAME.TXT` from the root of the image into the current Linux directory, and it stops if that name is already there. `diskput` expects a path inside the image and a file of the same name in the current directory.

```bash
make clean
```
