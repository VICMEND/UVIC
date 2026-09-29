#include "helper.h"


/* function: getOSName
 * ----------------------
 * Saves OS name into name
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             name: Variable to save OS Name
 * returns: NOTHING
 */
void getOSName(char *p, char *name){
    memcpy(name, p+3, 8);
    name[8] = '\0';
}

/* function: getDiskLabel
 * ----------------------
 * Saves disk label into name
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             name: Place to save disk label
 * returns: NOTHING
 */
void getDiskLabel(char *p, char *name){
    for(int i = 0; i < 224; i++){
        char *entry = p + ROOTSECTOR * SECTOR_SIZE + i * 32;

        if (entry[0] == 0x00) break;

        if(entry[11] & 0x08){
           for(int i =0; i <8; i++){
               if(entry[i] == ' '){
                   name[i] = '\0';
                   break;
               }
               name[i] = entry[i];
           }
        }
    }
}


/* function: getNumFiles
 * ----------------------
 * Return number of files. recursively goes through directories
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             next: the next cluster of the directory
 *             sectorSize: Size of the sector
 *             startPoint: The starting point of the directory
 * returns: Number of files
 */
int getNumFiles(char*p, int next, int sectorSize, int startPoint){
    
    int fileCount = 0;

    for(int i = 0; i < sectorSize/32 ;i++) {
        char *entry = p + startPoint * SECTOR_SIZE + i * 32; // 19th sector start and increments by 32 each loop.

        if (entry[0] == 0x00) break;
        if (entry[0] == 0xE5) continue;

        if(entry[11] & 0x10){ // subdirectory attribute
            if (entry[0] != '.' || (entry[1] != ' ' && entry[1] != '.')){
                uint16_t nextCluster = entry[26] | entry[27] << 8;
                fileCount += getNumFiles(p,nextCluster,SECTOR_SIZE,nextCluster+LOGICALOFFSET);
            }
        } else if (entry[11] != 0x08 && entry[11] != 0x0F){
            fileCount++;
        }
    }
    if (next == 0) return fileCount;

    int n = getFatEntry(p,next);// get the value in the FAT table

    if (n == 0xFFF){  // if the value is 0xFFF then it is the final sector.
        return fileCount;
    } else {
        fileCount += getNumFiles(p,n,SECTOR_SIZE,n+LOGICALOFFSET);
    }
    return fileCount;
}

/* function: getNumFatCopies
 * ----------------------
 * Gets the number of FAT copies
 * ----------------------
 * Parameters: p: memory config of IMA file
 * returns: NOTHING
 */
int getNumFatCopies(char *p){
    return p[16];
}

/* function: getSectorsFAT
 * ----------------------
 * Grabs the number sectors per FAT
 * ----------------------
  * Parameters: p: memory config of IMA file
 * returns: NOTHING
 */
int getSectorsFAT(char *p){
    return p[22] | p[23] << 8;
}


int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Error please pass the correct number of arguments\n");
        exit(1);
    }
    int fd;
    struct stat sb;


    fd = open(argv[1], O_RDWR);
    fstat(fd, &sb);


    char * p = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
    if (p == MAP_FAILED) {
        printf("Error: failed to map memory\n");
        exit(1);
    }
    char OSName[9];
    char OSLabel[11];

    getOSName(p,OSName);
    getDiskLabel(p,OSLabel);
    int totalSize = getTotalSize(p);
    int freeSize = getFreeSize(p);
    int numFiles = getNumFiles(p,0,SIZEROOT,ROOTSECTOR);
    uint8_t numFatCopies = getNumFatCopies(p);
    uint16_t sectorsFat = getSectorsFAT(p);

    // prints all the disk information
    printf("OS Name:                         %s\n", OSName);
    printf("Label of the disk:               %s\n",OSLabel);
    printf("Total size of the disk:          %d\n", totalSize);
    printf("Free size of the disk:           %d\n", freeSize);
    printf("==================================\n");
    printf("Number of files in the disk:     %d\n",numFiles);
    printf("==================================\n");
    printf("Number of FAT copies:            %d\n",numFatCopies);
    printf("Sectors per FAT:                 %d\n",sectorsFat);

    munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
    close(fd);
    return 0;
}