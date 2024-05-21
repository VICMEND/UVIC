/** @file music_manager.c
 *  @brief A small program to analyze songs data.
 *  @author Mike Z.
 *  @author Felipe R.
 *  @author Hausi M.
 *  @author Jose O.
 *  @author Victoria L.
 *  @author Victor Mendes
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define MAX_LINE_LEN 180

// prototypes
void printer(int DISPLAY_NUM, node_t *LIST, char *SORT_VAR);
void filter(char *FILE_NAME, int DISPLAY_NUM, char *SORT_VAR);

/// @brief Reads file information, filters it in accordance to passed arguments and passes it to the printer.
/// @param FILE_NAME The name of file to be read.
/// @param DISPLAY_NUM The number of entries to display.
/// @param SORT_VAR The column header to be sorted by.
void filter(char *FILE_NAME, int DISPLAY_NUM, char *SORT_VAR){

    char *RESULT = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
    char *line = (char *)malloc(sizeof(char) * MAX_LINE_LEN);
    char *TOKEN = NULL;
    node_t *LIST = NULL;
    double SORT_NUM;
    char *SORT_STR = malloc(sizeof(char) * MAX_LINE_LEN);
    char *SONG_NAME = malloc(sizeof(char) * MAX_LINE_LEN);
    char *ARTIST = malloc(sizeof(char) * MAX_LINE_LEN);
    char *YEAR = malloc(sizeof(char) * MAX_LINE_LEN);

    FILE *IFILE = fopen(FILE_NAME, "r");
    fgets(line, (sizeof(char) * MAX_LINE_LEN), IFILE);

    while (fgets(line, (sizeof(char) * MAX_LINE_LEN), IFILE) != NULL){
        RESULT[0] = '\0';
        TOKEN = strtok(line, ",");
        strcpy(ARTIST, TOKEN);
        TOKEN = strtok(NULL, ",");
        strcpy(SONG_NAME, TOKEN);
        for (int i = 0; i < 3; i++){
            TOKEN = strtok(NULL, ",");
        }
        strcpy(YEAR, TOKEN);
        if ((strcmp(SORT_VAR, "popularity")) == 0){
            TOKEN = strtok(NULL, ",");
            SORT_NUM = strtod(TOKEN, NULL);
            strcpy(SORT_STR, TOKEN);
        }
        else if ((strcmp(SORT_VAR, "energy")) == 0){
            for (int i = 0; i < 3; i++){
                TOKEN = strtok(NULL, ",");
            }
            SORT_NUM = strtod(TOKEN, NULL);
            strcpy(SORT_STR, TOKEN);
        }
        else if ((strcmp(SORT_VAR, "danceability")) == 0){
            for (int i = 0; i < 2; i++){
                TOKEN = strtok(NULL, ",");
            }
            strcpy(SORT_STR, TOKEN);
            SORT_NUM = strtod(TOKEN, NULL);
        }
        sprintf(RESULT, "%s,%s,%s,%s", ARTIST, SONG_NAME, YEAR, SORT_STR);
        LIST = add_inorder(LIST, new_node(RESULT, SONG_NAME, SORT_NUM));
    }
    fclose(IFILE);
    printer(DISPLAY_NUM, LIST, SORT_VAR);
    node_t *temp_n = NULL;
    for (; LIST != NULL; LIST = temp_n){
        temp_n = LIST->next;
        free(LIST->word);
        free(LIST->name);
        free(LIST);
    }
    free(line);
    free(RESULT);
    free(SONG_NAME);
    free(ARTIST);
    free(YEAR);
    free(SORT_STR);
}

/// @brief Prints out formated data to a CSV file.
/// @param DISPLAY_NUM  The number of entries to display.
/// @param LIST The list containing each songs sorted data.
/// @param SORT_VAR The column header to be sorted by.
void printer(int DISPLAY_NUM, node_t *LIST, char *SORT_VAR)
{
    FILE *PFILE = fopen("output.csv", "w");
    fprintf(PFILE, "artist,song,year,%s\n", SORT_VAR);

    for (int i = 0; i < DISPLAY_NUM; i++)
    {
        fprintf(PFILE, "%s\n", LIST->word);
        LIST = LIST->next;
    }
    fclose(PFILE);
}

/**
 * @brief The main function and entry point of the program.
 *
 * @param argc The number of arguments passed to the program.
 * @param argv The LIST of arguments passed to the program.
 * @return int 0: No errors; 1: Errors produced.
 *
 */
int main(int argc, char *argv[])
{
    char *FILE_NAME;
    int DISPLAY_NUM;
    char *SORT_VAR;

    strtok(argv[1], "=");
    SORT_VAR = strtok(NULL, "=");
    strtok(argv[2], "=");
    DISPLAY_NUM = atoi(strtok(NULL, "="));
    strtok(argv[3], "=");
    FILE_NAME = strtok(NULL, "=");

    filter(FILE_NAME, DISPLAY_NUM, SORT_VAR);
    exit(0);
}
