#include "helper.h"
#include <stdbool.h>


/*
WAS UNABLE TO COMPLETE IN TIME
 */

#define MAX 256

/* function: searchFAT
 * ----------------------
 * Returns the first empty slot in the FAT
 * ----------------------
 * Parameters: p: memory config of IMA file
 * returns: empty slot in the FAT
 */
int searchFAT(char *p){
    for (int i = 2; i < 4096; i++){
        if(getFatEntry(p,i) == 0x00){
            return i;
        }
    }
    return -1;
}

/* function: setUpDirectory
 * ----------------------
 * (INCOMPLETE) sets up the first empty directory entry with file information (time created, first logical cluster, file name and extension)
 * untested only file name extension and file size coded into function
 * missing logical cluster and time information
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             startPoint: start point of the cluster
 *             fName: name of the file to be copied into disk
 *             fileSize: size of file
 * returns: empty slot in the FAT
 */
void setUpDirectory(char*p,int startPoint,char *fName,uint32_t fileSize){
    int reps = 16;

    if (startPoint == ROOTSECTOR*SECTOR_SIZE){
        reps = 224;
    }

    char name[8] = "        ";  // Initialize with spaces
    char ext[3] = "   ";        // Initialize with spaces
    char *dot = strchr(fName, '.');
    if (dot) {
        int nameLength = dot - fName;
        nameLength = nameLength > 8 ? 8 : nameLength;
        strncpy(name, fName, nameLength);

        int extLength = strlen(dot + 1);
        extLength = extLength > 3 ? 3 : extLength;
        strncpy(ext, dot + 1, extLength);
    } else {
        int nameLength = strlen(fName);
        nameLength = nameLength > 8 ? 8 : nameLength;
        strncpy(name, fName, nameLength);
    }

    int next = startPoint / SECTOR_SIZE - LOGICALOFFSET;

    for(int i = 0; i < reps ;i++) {
        char *entry = p + startPoint + i * 32;

        if (entry[0] == 0x00 || entry[0] == 0xE5) {
            memcpy(entry, name, 8);
            memcpy(entry + 8, ext, 3);

            entry[28] = fileSize & 0xFF;
            entry[29] = (fileSize >> 8) & 0xFF;
            entry[30] = (fileSize >> 16) & 0xFF;
            entry[31] = (fileSize >> 24) & 0xFF;

//            int  firstSector= searchFAT(p);
            }
        }


    if (next == 0) return;

    int n = getFatEntry(p,next);// get the value in the FAT table

    if (n == 0xFFF){  // if the value is 0xFFF then it is the final sector.
        exit(0);
    } else {
        setUpDirectory(p,n,fName,fileSize);
    }
    exit(0);
}

/* function: findDir
 * ----------------------
 * find the directory where the file is supposed to go and checks if there is already a file with that name in it
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             next: next logical cluster
 *             startPoint: start point of the cluster
 *             components[]: array containing eahc subdirectory and the file name
 *             numToks: number of entries in components[]
 *             compIndex: current index of components represents which subdirectory to look for
 *             duplicate: boolean that get set if a file with the same name is present in the directory
 * returns: The location of the final directory in the path or -1 if not found.
 */
int findDir(char*p, int next, int sectorSize, int startPoint,char *components[], int numToks, int compIndex,bool *duplicate){

    int foundPos = -1;

    if(compIndex == (numToks-1)){
        foundPos = startPoint * SECTOR_SIZE;
    }
    for(int i = 0; i < sectorSize/32 ;i++) {
        char *entry = p + startPoint * SECTOR_SIZE + i * 32; // 19th sector start and increments by 32 each loop.

        if (entry[0] == 0x00) break;
        if (entry[0] == 0xE5) continue;

        if(compIndex == (numToks-1)){
            if(compareFileName(entry,components[compIndex]) == 0){
                *duplicate = true;
            }
        }

        if(entry[11] & 0x10){ // subdirectory attribute
            if (compareFileName(entry,components[compIndex]) == 0){
                uint16_t nextCluster = entry[26] | entry[27] << 8;
                compIndex++;
                foundPos = findDir(p,nextCluster,SECTOR_SIZE,nextCluster+LOGICALOFFSET,components,numToks,compIndex,duplicate);
            }
        }
    }
    if (next == 0) return foundPos;

    int n = getFatEntry(p,next);// get the value in the FAT table

    if (n == 0xFFF){  // if the value is 0xFFF then it is the final sector.
        return foundPos;
    } else {
        foundPos = findDir(p,n,SECTOR_SIZE,n+LOGICALOFFSET,components,numToks,compIndex,duplicate);
    }
    return foundPos;
}


int main(int argc, char *argv[]) {

    if (argc != 3){
        printf("Error please pass the correct number of arguments\n");
        exit(1);
    }
    char *components[MAX];
    struct stat sb;
    struct stat sb2;

    size_t length = strlen(argv[2]);

    char *filename = strrchr(argv[2], '/');

    //get file name out of args
    if (filename != NULL) {
        filename++;
    } else {
        filename = argv[2];
    }

    // make argument into uppercase
    char path[length+1];
    strncpy(path, argv[2],length);
    path[length] = '\0';
    for(int i =0; i < length;i++){
        path[i] = toupper(path[i]);
    }

    char *token = strtok(argv[2 ], "/");
    int numToks = 0;

    // saves into array to be pased into functions
    while (token != NULL && numToks < MAX) {
        components[numToks] = token;
        numToks++;
        token = strtok(NULL, "/");
    }


    int fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("Error: failed to open or create file");
        exit(1);
    }
    fstat(fd, &sb);

    char * p = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
    if (p == MAP_FAILED) {
        close(fd);
        printf("Error: failed to map memory\n");
        exit(1);
    }

    int fd2 = open(filename, O_RDWR);
    if (fd2 < 0) {
        perror("Error: failed to open or create file");
        munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
        close(fd);
        exit(1);
    }

    fstat(fd2, &sb2);
    char * p2 = mmap(NULL, sb2.st_size, PROT_READ, MAP_SHARED, fd2, 0); // p points to the starting pos of your mapped memory
    if (p == MAP_FAILED) {
        close(fd);
        close(fd2);
        munmap(p, sb.st_size);
        printf("Error: failed to map memory\n");
        exit(1);
    }

    // checks if there is enough space in the disk to fit the file.
    if(getFreeSize(p) < sb2.st_size){
        close(fd);
        close(fd2);
        munmap(p, sb.st_size);
        munmap(p2, sb2.st_size);
        printf("Error: Not Enough disk space\n");
        exit(1);
    }
    bool dupe = false;
    int foundPos = findDir(p,0,SIZEROOT,ROOTSECTOR,components,numToks,0, &dupe);

    // checks if the directory exists and if there is already a file in there
    if(foundPos == -1 || dupe == true){
        close(fd);
        close(fd2);
        munmap(p, sb.st_size);
        munmap(p2, sb2.st_size);
        printf("Error:Directory does not exist or file already exists in the directory\n");
        exit(1);
    }


    close(fd);
    close(fd2);
    munmap(p, sb.st_size);
    munmap(p2, sb2.st_size);
    return 0;
}