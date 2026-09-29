V_number:V00905409
Section: A02
Name: Victor Mendes
Assignmnet 3
-------------------------------------------------------------------------------
Compile and run instructions.
-------------------------------------------------------------------------------

To compile the code issue "make" command from the directory where diskget.c, diskinfo.c, 
disklist.c, diskinput.c, helper.c, helper.h and the Makefile are located.

After compiling the program it can be run by issuing the following commands:


./diskinfo "FILE.IMA" from the directory where diskinfo is located (The "./" is necessary and FILE.IMA is any IMA file in the directory where diskinfo is located)

./disklist "FILE.IMA" from the directory where disklist is located (The "./" is necessary and FILE.IMA is any IMA file in the directory where disklist is located)

./diskget "FILE.IMA" "file" from the directory where diskget is located (The "./" is necessary, FILE.IMA is any IMA file in the directory where diskget is located and "file" is a file in the root directory of the "FILE.IMA")

./diskput "FILE.IMA" "/path/file" from the directory where diskput is located (The "./" is necessary, FILE.IMA is any IMA file in the directory where diskput is located, "/path/file" is a subdirectory path in FILE.IMA and file is any file in the current linux directory) 

-------------------------------------------------------------------------------
Behaviour
-------------------------------------------------------------------------------

./diskinfo - diskinfo lists the OS Name, disk label, total disk size, total free size, the number of files in the disk, the number of FAT copies and the number of sectors per FAT.

./disklist - disklist lists all the files and directories in the IMA file.

./diskget - diskget creates a copy of a file in the .IMA in the current Linux directory (if the file in the .IMA has the same name as a file in the Linux directory then an error is generated).

./diskput - places a file from the current Linux directory into the specified subdirectory in the .IMA file. generates an error if the directory path does not exist, if there is not enough space in the disk and if the file is not found. (NOT FULLY IMPLEMENTED).

