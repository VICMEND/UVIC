#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct{
    char OSName[9];
    char *diskLabel;
    int diskSize;
    int freeSize;
    int numFiles;
    int FATCopies;
    int sectors;
} diskInfo;


void getOSName(diskinfo * info, char *p){
    strncpy(info->OSName, p[3], 8);
    printf("OS Name:        %s/n", info->OSName);


}


void printInfo(){
    printf("OS Name:        %s/n", );
    printf("Label of the disk:          %s/n", );
    printf("Total size of the disk:         %d/n", );
    printf("Free size of the disk:          %d/n", );
    printf("==============/n", );
    printf("The number of files in the disk/n", );
    printf("(including all files in the root directory and files in all subdirectories):        %d/n", );
    printf("=============/n")
        printf("Number of FAT copies:       %d/n", );
    printf("Sectors per FAT:            %d/n", );
}

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Error please pass the correct number of arguments\n");
        exit(1);
    }
    int fd;
    struct stat sb;
    diskInfo info;

    fd = open(argv[1], O_RDWR);
    fstat(fd, &sb);
    printf("Size: %lu\n\n", (uint64_t)sb.st_size);

    char * p = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
    if (p == MAP_FAILED) {
        printf("Error: failed to map memory\n");
        exit(1);
    }


}

int fileCount = 0;

    for(int i = 0; i < sectorSize/32 ;i++) {
        char *entry = p + startPoint * SECTOR_SIZE + i * 32;

        if (entry[0] == 0x00) break;
        if (entry[0] == 0xE5) continue;

        if(p[11] & 0x10){
            if (e[0] != '.' || (e[1] != ' ' && e[1] != '.')){
                uint16_t nextCluster = e[26] | e[27] << 8;
                fileCount += getNumFiles(p,nextCluster,SECTOR_SIZE,nextCluster+LOGICALOFFSET);
            }
        } else if (e[11] != 0x08 && e[11] != 0x0F){
            fileCount++;
        }
    }
    if (next == 0) return fileCount;

    int n = getFatEntry(p,next);

    if (n == 0xFFF){
        return fileCount;
    } else {
        fileCount += getNumFiles(p,n,SECTOR_SIZE,n+LOGICALOFFSET);
    }
    return fileCount;
}