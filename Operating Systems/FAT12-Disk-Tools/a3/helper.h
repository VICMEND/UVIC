#ifndef HELPER_FILE_H
#define HELPER_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<string.h>
#include <ctype.h>

#define SECTOR_SIZE 512
#define TRUE 1
#define FALSE 0
#define LOGICALOFFSET 31
#define SIZEROOT 7168
#define ROOTSECTOR 19
#define timeOffset 14 //offset of creation time in directory entry
#define dateOffset 16

uint16_t getFatEntry(char *p, int n);
int  getTotalSize(char *p);
void print_date_time(char * directory_entry_startPos);
uint32_t getFileSize(const char *entry);
int getFreeSize(char *p);
int compareFileName(char *entry,char *fName);

#endif // HELPER_FILE_H