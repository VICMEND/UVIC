#include "helper.h"

int arrIndex = 0; // location in the array holding the subdirectory name and offsets
int subArray[3000] = {}; //subdirectory name and offsets
char subNames[3000][9]; //subdirectory name and offsets



void getFileName(char *entry);
void printFileInfo(char *entry);
void printDirectoryInfo(char *entry);
uint32_t getFileSize(const char *entry);

/* function: getFileName
 * ----------------------
 * Prints the file name
 * ----------------------
  * Parameters: entry: memory config of IMA file of the current entry
 * returns: NOTHING
 */
void getFileName(char *entry){

    char name[9];
    char extension[4];
    char fullName[13];

    int i;

    for(i = 0; i < 8; i++){
        if(entry[i] ==  0x20){
            break;
        } else{
            name[i] = entry[i];
        }
    }
    name[i] = '\0';

    for(i = 0; i < 3; i++){
        if(entry[i + 8] ==  0x20){
            break;
        } else{
            extension[i] = entry[i+8];
        }
    }
    extension[i]= '\0';

    if (extension[0] != '\0') {
        snprintf(fullName, sizeof(fullName), "%s.%s", name, extension);
    } else {
        snprintf(fullName, sizeof(fullName), "%s", name);
    }

    printf("%-20s", fullName);
}

/* function: printFileInfo
 * ----------------------
 * Prints the file info ( size,name, type and date)
 * ----------------------
  * Parameters: entry: memory config of IMA file of the current entry
 * returns: NOTHING
 */
void printFileInfo(char *entry){
    uint32_t size = getFileSize(entry);
    printf("F %-10u ",size);
    getFileName(entry);
    print_date_time(entry);
}

/* function: printDirectoryInfo
 * ----------------------
 * Prints the directory info (name, type) Date is bugged for some reason
 * ----------------------
  * Parameters: entry: memory config of IMA file of the current entry
 * returns: NOTHING
 */
void printDirectoryInfo(char *entry){
    printf("D            ");
    getFileName(entry);
    printf("\n");
//    print_date_time(entry);
}

/* function: subDirectories
 * ----------------------
 * Traverses through sub directories
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             next: the next cluster of the directory
 *             startPoint: The starting point of the directory
 * returns: NOTHING
 */
void subDirectories(char*p,int next,int startPoint){

    for(int i = 0; i < 16 ;i++) {
        char *entry = p + startPoint * SECTOR_SIZE + i * 32;

        if (entry[0] == 0x00) break;
        if (entry[0] == 0xE5) continue;

        if(entry[11] & 0x10){
            if (entry[0] != '.' || (entry[1] != ' ' && entry[1] != '.')){
                uint16_t nextCluster = entry[26] | entry[27] << 8;
                subArray[arrIndex] = nextCluster;
                memcpy(&subNames[arrIndex], entry, 8);
                subNames[arrIndex][8] = '\0';
                printDirectoryInfo(entry);
                arrIndex++;
            }
        } else if (entry[11] != 0x08 && entry[11] != 0x0F){
            printFileInfo(entry);
        }
    }
    if (next == 0) return;
    int n = getFatEntry(p,next);
    if (n == 0xFFF){
        return;
    } else {
        subDirectories(p,n,n+LOGICALOFFSET);
    }
    return;
}

/* function: printFiles
 * ----------------------
 * Traverses through root and other sub directories and prints each ones information
 * ----------------------
 * Parameters: p: memory config of IMA file
 * returns: NOTHING
 */
void printFiles(char *p){

    int sectorCount = getTotalSize(p)/SECTOR_SIZE;

    printf("ROOT\n==================\n");

    for(int i = 0; i < SIZEROOT/32 ;i++) {
        char *entry = p + ROOTSECTOR * SECTOR_SIZE + i * 32;

        if (entry[0] == 0x00) break;
        if (entry[0] == 0xE5) continue;


        if(entry[11] & 0x10){

            if (entry[0] != '.' || (entry[1] != ' ' && entry[1] != '.')){

                uint16_t nextCluster = entry[26] | entry[27] << 8;
                subArray[arrIndex] = nextCluster;
                memcpy(&subNames[arrIndex], entry, 8);
                subNames[arrIndex][8] = '\0';
                printDirectoryInfo(entry);
                arrIndex++;
            }
        } else if (entry[11] != 0x08 && entry[11] != 0x0F){
            printFileInfo(entry);
        }
    }
    printf("==================\n");
    for(int i = 0; i < sectorCount; i++){
        if(subArray[i] != 1 && subArray[i] !=0){
            printf("%s\n==================\n",subNames[i]);
            subDirectories(p,subArray[i],subArray[i]+LOGICALOFFSET);
            printf("==================\n");

        }
    }
    return;
}



int main(int argc, char *argv[]) {
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

    printFiles(p);

    munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
    close(fd);
    return 0;
}