/** @file event_manager.c
 *  @brief A pipes & filters program that uses conditionals, loops, and string processing tools in C to process iCalendar
 *  events and printing them in a user-friendly format.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author Victoria L.
 *  @author Victor Mendes
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
 * @brief The maximum line length.
 *
 */
#define MAX_LINE_LEN 132

//Prototypes
void reader(char* FILE_NAME,int FULL_DATESTRT, int FULL_DATEND);
void processing(int DATE_RANGESTRT, int DATE_RANGEND,char *PREV_DATE, int OFFSET, char info[5][1024]);
int timeconverter(int TIME);
void writer(char* TIMESTRTAMPM,char* TIMENDAMPM, int YEAR, int MONTH, int DAY, char info[5][1024] ,int SAME_DATE_FLAG);
int  dateconverter(int DAY, int MONTH, int YEAR);

 /* Function: reader
 * --------------
 * @brief loops through calendar lines and passes event information to reader
 * @param FILE_NAME file name of calendar file to loop through.
 * @param FULL_DATESTRT date start range.
 * @param FULL_DATEND date end range.
 * @return void
 */
 void reader(char* FILE_NAME, int FULL_DATESTRT, int FULL_DATEND){
    char *token;
    int BEGIN_FLAG = 0, num = 0 , RRULE = 0, OFFSET = 0;
    char info[5][1024],  PREV_DATE[MAX_LINE_LEN], DATE_COMPARE[1024], line[MAX_LINE_LEN];
   

    FILE *CALENDAR = fopen(FILE_NAME, "r");

    while (fgets(line, sizeof(line), CALENDAR) != NULL) {
        if (strcmp(line,"BEGIN:VEVENT\n") == 0 || BEGIN_FLAG == 1 ){ //filters out irrelevant lines like BEGIN:CALENDAR.
            BEGIN_FLAG = BEGIN_FLAG+ 1;
        } else if (strcmp(line,"END:VEVENT\n") == 0){
            BEGIN_FLAG = 0;
            num = 0;
        }

        //saves relevant line information in an array
        if( BEGIN_FLAG == 2){
            token = strtok(line, ":");
             if (strcmp(token, "RRULE") == 0){
                for (int i = 0; i < 4; i++){
                    token = strtok(NULL, "=");
                }
              strncpy(info[4], token, MAX_LINE_LEN - 1);

               RRULE = 1;
             } else {
            strncpy(info[num], strtok(NULL, ":"), MAX_LINE_LEN - 1);
             num++;
            }
        }
        // sends info for processing
        if (num ==4){
            if (RRULE == 1){
            strcpy(DATE_COMPARE, info[0]);
            OFFSET = (atoi(strtok(info[4], "T")) - atoi(strtok(DATE_COMPARE, "T")))/7;
             for (int i =0; i <= OFFSET; i++){
                processing(FULL_DATESTRT, FULL_DATEND, PREV_DATE, i*7, info);
             }
            RRULE = 0;
            } else{
            processing(FULL_DATESTRT, FULL_DATEND, PREV_DATE, 0, info);
            strcpy(PREV_DATE , info[0]);
            }
            num =0;
        }
    }
         fclose(CALENDAR);
}

// Function: processing
// --------------
/// @brief Processes information given by the r eader and feeds it into the writer
/// @param DATE_RANGESTRT int with lower bound date rangeto compare to
/// @param DATE_RANGEND int with upper bound date range to compare to
/// @param PREV_DATE previous date to check for formatting purposes
/// @param OFFSET  offset for weekly reccuring events
/// @param info array containing event start, end, location and description information
void processing(int DATE_RANGESTRT, int DATE_RANGEND,char *PREV_DATE,int OFFSET, char info[5][1024]){
    char TIMESTRTAMPM[MAX_LINE_LEN], TIMENDAMPM[MAX_LINE_LEN], *token;
    int DATESTRT, TIMESTRT ,DATEEND, TIMEND, YEAR, MONTH, DAY ,SAME_DATE_FLAG = 0;

    // checks if the date as the same as previous events date
   if (strncmp(info[0], PREV_DATE, 8) == 0) {
                SAME_DATE_FLAG = 1;
            } else {
                SAME_DATE_FLAG = 0 ;
            }
    
    // formats event information
    token = strtok(info[2], "\n");
    strncpy(info[2], token, sizeof(info[2])-1);
     token = strtok(info[3], "\n");
   strncpy(info[3], token, sizeof(info[3])-1);


    sscanf(info[0], "%4d%2d%2dT%4d", &YEAR, &MONTH, &DAY, &TIMESTRT);
    sscanf(info[1], "%8dT%4d",&DATEEND, &TIMEND);



     DATESTRT = YEAR * 10000 + MONTH * 100 + DAY;

    if( DATESTRT >= DATE_RANGESTRT && DATEEND <= DATE_RANGEND){
    
    DAY += OFFSET;
    
    if(TIMESTRT >= 1200){
        if(TIMESTRT >= 1300){
        TIMESTRT = timeconverter(TIMESTRT);
        }
        sprintf(TIMESTRTAMPM, "%d:%02d PM",TIMESTRT/100,TIMESTRT%100);
    } else {
         sprintf(TIMESTRTAMPM, "%d:%02d AM",TIMESTRT/100,TIMESTRT%100);
    }

    if(TIMEND >= 1200){
         if(TIMEND >= 1300){
        TIMEND = timeconverter(TIMEND);
         }
         sprintf(TIMENDAMPM, "%d:%02d PM",TIMEND/100,TIMEND%100);
    } else{
         sprintf(TIMENDAMPM, "%d:%02d AM",TIMEND/100,TIMEND%100);
    }
    writer(TIMESTRTAMPM,TIMENDAMPM,YEAR,MONTH,DAY,info,SAME_DATE_FLAG);
    }
}

 /* Function: writer
 * --------------
 * @brief displays filtered event information
 * @param TIMESTRTAMPM Start time in HH:MM format
 * @param TIMENDAMPM END time in HH:MM format
 * @param YEAR Event year
 * @param MONTH Event month
 * @param DAY Event day
 * @param info array containing event description and location
 * @param SAME_DATE_FLAG Flag to determine if event is happening in the same day as the previous event.
 * @return int.
 */
void writer(char* TIMESTRTAMPM,char* TIMENDAMPM, int YEAR, int MONTH, int DAY, char info[5][1024] ,int SAME_DATE_FLAG){
    static int spacer = 0, ONEDIGIT1 =0, ONEDIGIT2=0;
    char OUTPUT_TIME[MAX_LINE_LEN],  OUTPUT_DATE[MAX_LINE_LEN];
    char MONTHS[12][MAX_LINE_LEN] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    // skips repeating date if same date as previous event
    if(SAME_DATE_FLAG != 1){
          if (spacer > 0) {
            fputs("\n", stdout);
        }
    sprintf( OUTPUT_DATE, "%s %02d, %d\n",MONTHS[MONTH-1], DAY, YEAR);
    fputs(OUTPUT_DATE, stdout);
    for (int i = 0; i < strlen(OUTPUT_DATE)-1; i++) {
        fputs("-", stdout);
    }
    fputs("\n", stdout);
    spacer =0;
    }
   
    ONEDIGIT1 = (strlen(TIMESTRTAMPM) == 7);
    ONEDIGIT2 = (strlen(TIMENDAMPM) == 7);

    if (ONEDIGIT1 && ONEDIGIT2) {

        //print formats for white space
        sprintf(OUTPUT_TIME, " %s to  %s: %s {{%s}}\n", TIMESTRTAMPM, TIMENDAMPM, info[3], info[2]);
    } else if(ONEDIGIT1){

        sprintf(OUTPUT_TIME, " %s to %s: %s {{%s}}\n", TIMESTRTAMPM, TIMENDAMPM, info[3], info[2]);
    }else if(ONEDIGIT2) {
 
        sprintf(OUTPUT_TIME, "%s to  %s: %s {{%s}}\n", TIMESTRTAMPM, TIMENDAMPM, info[3], info[2]);
    }else if(!ONEDIGIT1 && !ONEDIGIT2) {
        sprintf(OUTPUT_TIME, "%s to %s: %s {{%s}}\n", TIMESTRTAMPM, TIMENDAMPM, info[3], info[2]);
    }
    fputs(OUTPUT_TIME, stdout);
    spacer++;
}

/**
 * Function: timeconv
 * --------------
 * @brief Converts 24H time into 12H time.
 * @param TIME Time in 24H format
 * @return int the time value in 12H format
 *
 */
int timeconverter(int TIME){
        return TIME % 1200;
}

/**
 * Function: dateconverter
 * --------------
 * @brief Takes separate day, month, year values and combines them into YYYYMMDD format.
 * @param DAY
 * @param MONTH
 * @param YEAR 
 * @return int date in YYYYMMDD format
 */
int dateconverter(int DAY, int MONTH, int YEAR){
    return DAY + MONTH*100 + YEAR*10000;
}

/**
 * Function: main
 * --------------
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The list of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int main(int argc, char *argv[])
{  char  *token, *FILE_NAME;
   int FULL_DATESTRT, FULL_DATEND, YEAR_START, MONTH_START, DAY_START, YEAR_END, MONTH_END, DAY_END;

    //takes relevant information out of the arguments
    strtok(argv[1], "=");
    token = strtok(NULL, "");
    sscanf(token, "%d/%d/%d", &YEAR_START, &MONTH_START, &DAY_START);
    FULL_DATESTRT = dateconverter(DAY_START,MONTH_START,YEAR_START);

    strtok(argv[2], "=");
    token = strtok(NULL, "");
     sscanf(token, "%d/%d/%d", &YEAR_END, &MONTH_END, &DAY_END);
    FULL_DATEND = dateconverter(DAY_END,MONTH_END,YEAR_END);
    

    strtok(argv[3], "=");
    FILE_NAME = strtok(NULL, "=");

    reader(FILE_NAME,FULL_DATESTRT,FULL_DATEND);
    return 0;
}