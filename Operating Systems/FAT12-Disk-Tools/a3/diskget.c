#include "helper.h"

/* function: findFile
 * ----------------------
 * locates the file given in the IMA file
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             fName: name of the file to look for
 *             LogicalCluster: the corresponding sector in data segment of the disk
 * returns: the files size
 */
uint32_t findFile(char *p, char* fName,uint16_t *LogicalCluster) {

    for (int i = 0; i < SIZEROOT / 32; i++) {
        char *entry = p + ROOTSECTOR * SECTOR_SIZE + i * 32;

        if (entry[0] == 0x00) break;
        if (entry[0] == 0xE5) continue;

        if (compareFileName(entry, fName) == 0) {
            *LogicalCluster = entry[26] | entry[27] << 8;
            return getFileSize(entry);
        }
    }
    return -1;
}

/* function: extractFile
 * ----------------------
 * Makes a copy of the file in the .IMA file on the linux directory
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             p2: The Linux directory file to copy the file in the IMA to
 *             LogicalCluster: the corresponding sector in data segment of the disk
 *             sizeFile: the size of the file to be copied.
 *             sector: the current sector of the file to copy. ex first sector if it's the first 512 bytes of the file
 * returns: NOTHING
 */
void extractFile(char *p, char *p2, uint16_t logicalCluster,uint32_t sizeFile,int sector){

    int dataCluster = (logicalCluster + LOGICALOFFSET) * SECTOR_SIZE;
    char *origin = p + dataCluster;

    if (sizeFile > SECTOR_SIZE) {
        memcpy(p2+(sector*SECTOR_SIZE),origin,SECTOR_SIZE);

    } else {
        memcpy(p2+(sector*SECTOR_SIZE),origin,sizeFile);
    }
     int nextCluster = getFatEntry(p,logicalCluster);
     if(nextCluster == 0xFFF){
         return;
     } else {
         extractFile(p,p2,nextCluster,sizeFile,sector+1);
     }
}

int main(int argc, char *argv[]) {

    if (argc != 3){
        printf("Error please pass the correct number of arguments\n");
        exit(1);
    }

    int fd;
    struct stat sb;


    fd = open(argv[1], O_RDWR);
    if (fd < 0) {
        perror("Error: failed to open or create file");
        exit(1);
}
    fstat(fd, &sb);

    size_t length = strlen(argv[2]);

    //turns args to uppercase
    char fileName[length+1];
    strncpy(fileName, argv[2],length);
    fileName[length] = '\0';
    for(int i =0; i < length;i++){
        fileName[i] = toupper(fileName[i]);
    }

    char * p = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0); // p points to the starting pos of your mapped memory
    if (p == MAP_FAILED) {
        close(fd);
        printf("Error: failed to map memory\n");
        exit(1);
    }

    uint16_t logicalCluster;
    uint32_t sizeFile = findFile(p,fileName,&logicalCluster);

    if(sizeFile == -1){
        munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
        close(fd);
        printf("File Not Found\n");
        exit(0);
    }

    int fd2 = open(argv[2], O_RDWR | O_CREAT | O_EXCL, 0666);
    if (fd2 < 0) {
        perror("Error: failed to open or create file");
        munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
        close(fd);
        exit(1);
    }

    // reserves enough space in the file
    if (ftruncate(fd2, sizeFile) == -1) {
        perror("Error: failed to set file size");
        close(fd2);
        exit(1);
    }

    char *p2 = mmap(NULL,sizeFile , PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);
    if (p2 == MAP_FAILED) {
        close(fd);
        close(fd2);
        munmap(p, sb.st_size);
        printf("Error: failed to map memory\n");
        exit(1);
    }
    extractFile(p,p2,logicalCluster,sizeFile,0);

    munmap(p, sb.st_size); // the modifed the memory data would be mapped to the disk image
    munmap(p2, sizeFile); // the modifed the memory data would be mapped to the disk image
    close(fd);
    close(fd2);
    return 0;
}