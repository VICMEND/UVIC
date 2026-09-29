#include "helper.h"

/* function: getFatEntry
 * ----------------------
 * returns the value in the FAT slot
 * ----------------------
 * Parameters: p: memory config of IMA file
 *             n: the FAT slot
 * returns: The next logical cluster of that file
 */
uint16_t getFatEntry(char *p, int n){
    uint16_t fatEntry;

    if ((n % 2) == 0) {
        fatEntry = p[SECTOR_SIZE + (1 + (3 * n) / 2)] & 0x0F;
        fatEntry = (fatEntry << 8) + (unsigned char)p[SECTOR_SIZE + ((3 * n) / 2)];
    } else {
        fatEntry = (unsigned char)p[SECTOR_SIZE + (1 + (3 * n) / 2)];
        fatEntry = (fatEntry << 4) + ((p[SECTOR_SIZE + ((3 * n) / 2)] & 0xF0) >> 4);
    }
    return fatEntry;
}

/* function: getTotalSize
 * ----------------------
 * returns the total size of the disk
 * ----------------------
 * Parameters: p: memory config of IMA file
 * returns: Size of the disk
 */
int  getTotalSize(char *p){
    uint16_t totalSectorCount;
    memcpy(&totalSectorCount, p + 19, sizeof(totalSectorCount));
    return totalSectorCount*SECTOR_SIZE;
}

/* function: print_date_time
 * ----------------------
 * prints time an date information from the current sector ( provided by instructor)
 * ----------------------
 * Parameters: directory_entry_startPos: start of the cluster
 * returns: NOTHING
 */
void print_date_time(char * directory_entry_startPos){

    int time, date;
    int hours, minutes, day, month, year;

    time = *(unsigned short *)(directory_entry_startPos + timeOffset);
    date = *(unsigned short *)(directory_entry_startPos + dateOffset);

    //the year is stored as a value since 1980
    //the year is stored in the high seven bits
    year = ((date & 0xFE00) >> 9) + 1980;
    //the month is stored in the middle four bits
    month = (date & 0x1E0) >> 5;
    //the day is stored in the low five bits
    day = (date & 0x1F);

    printf("%d-%02d-%02d ", year, month, day);
    //the hours are stored in the high five bits
    hours = (time & 0xF800) >> 11;
    //the minutes are stored in the middle 6 bits
    minutes = (time & 0x7E0) >> 5;

    printf("%02d:%02d\n", hours, minutes);

    return ;
}
/* function: print_date_time
 * ----------------------
 * prints time an date information from the current sector ( provided by instructor)
 * ----------------------
 * Parameters: directory_entry_startPos: start of the cluster
 * returns: NOTHING
 */
uint32_t getFileSize(const char *entry) {
    uint32_t fileSize = (unsigned char)entry[28] |
                        ((unsigned char)entry[29] << 8) |
                        ((unsigned char)entry[30] << 16) |
                        ((unsigned char)entry[31] << 24);
    return fileSize;
}
/* function: getFreeSize
 * ----------------------
 * returns the free size in the disk
 * ----------------------
 * Parameters: p:  memory config of .IMA file
 * returns: free size in the disk
 */
int getFreeSize(char *p){
    int emptyCluster = 0;
    int totalSectorCount = (getTotalSize(p)-512*31)/SECTOR_SIZE;

    for(int i = 2; i < totalSectorCount; i++){
        if(getFatEntry(p,i) == 0x00){
            emptyCluster++;
        }
    }
    return emptyCluster*SECTOR_SIZE;
}
/* function: compareFileName
 * ----------------------
 * Compares the given name to the name in the sector
 * ----------------------
 * Parameters: entry: start of the cluster
 *             fName: file name to compare with
 * returns: 0 if successful
 */
int compareFileName(char *entry,char *fName){

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
    return strcmp(fullName,fName);
}