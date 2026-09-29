#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"

Node* head = NULL;
#define MAX_LINE_LEN 256

/* function: filereaderStat
 * ----------------------
 * This function reads through the files /proc/<pid>/stat
 * and print  the values of comm, state, utime, stime, rss.
 * ----------------------
 * Parameters:  *path = the file path of the file to opened
 * returns: Nothing
 */
void fileReaderStat( char *path) {
    char line[MAX_LINE_LEN];
    char *token;
    FILE *stat = fopen(path, "r");

    if (stat == NULL) {
        printf("Error: failed to open file");
        return;
    }
    fgets(line, sizeof(line), stat);
    //Printing comm, state, utime, stime, rss info
    token = strtok(line, " ");
    for (int i = 0; i < 25; i++) {
        token = strtok(NULL, " ");
        switch (i) {
            case 0:
                printf("Comm:                           %s \n", token);
                break;
            case 1:
                printf("State:                          %s \n", token);
                break;
            case 12:
                printf("Utime:                          %s \n", token);
                break;
            case 13:
                printf("Stime:                          %s \n", token);
                break;
            case 22:
                printf("Rss:                            %s \n", token);
                break;
            default:
                // Do nothing for other cases
                break;
        }
    }
    fclose(stat);
    return;
}
/* function: filereaderStatus
 * ----------------------
 * This function reads through the files /proc/<pid>/status and prints
 * the voluntary_ctxt_switches, nonvoluntary_ctxt_switches
 * ----------------------
 * Parameters:  *path = the file path of the file to opened
 * returns: Nothing
 */
void fileReaderStatus( char *path){
    char line[MAX_LINE_LEN];
    FILE *status = fopen(path, "r");
    if (status == NULL) {
        printf("Error: failed to open file");
        return;
    }
    while (fgets(line, sizeof(line), status)) {
            // Check if the line starts with "voluntary_ctxt_switches"
            if (strncmp(line, "voluntary_ctxt_switches:", 24) == 0) {
                printf("%s", line);
            }
                // Check if the line starts with "nonvoluntary_ctxt_switches"
            else if (strncmp(line, "nonvoluntary_ctxt_switches:", 27) == 0) {
                printf("%s", line);
            }
        }
    fclose(status);
    return;
}

/* function: func_BG
 * -----------------
 * This function creates a child process using fork() to run in the background
 * and runs a program in the new child process using execvp().
 * -----------------
 * Parameters: cmd = an array of the arguments passed with the "bg" command
 * returns: Nothing
 */
void func_BG(char **cmd){
        pid_t pid;
        pid = fork();
        if (pid < 0) { // Error in fork
            fprintf(stderr,"Failed to fork \n");
        }
        else if (pid == 0) { //in the child process
            execvp(cmd[1],&cmd[1]); // file name = cmd[1], &cmd[1] = array of args
            fprintf(stderr, "Failed to run file\n");  // only runs if execvp fails.
            exit(EXIT_FAILURE);
        }
        else { // in the parent process
            head = add_newNode(head, pid, cmd[1]);
            printf("Child %d Complete \n", pid);
        }
}


/* function: func_BGlist
 * ---------------------
 * This function prints the PID and the path of all the background processes created
 * using the printList() function from linked_list.h
 * ---------------------
 * Parameters: None
 * returns: Nothing
 */
void func_BGlist(){
    printList(head);
}

/* function: func_BGkill
 * ---------------------
 * This function terminates the process associated with the PID passed to it
 * using the kill() function and passing SIGTERM as a parameter.
 * ---------------------
 * Parameters: str_pid = a string representation of a PID.
 * returns: Nothing
 */
void func_BGkill(char * str_pid){
    if (str_pid == NULL) {
        printf("Error: pid is NULL\n");
        return;
    }
    int status;
    pid_t pid = atoi(str_pid);
    if(!PifExist(head,pid)){ // checks if the pid even exists
        printf("Error: Process %d does not exist.\n", pid);
    } else {
        waitpid(pid, &status, WNOHANG); //checks pid status
        if (kill(pid,SIGTERM) == 0 ){ //kills pid
            waitpid(pid, &status, WNOHANG);
            head = deleteNode(head, pid); // deletes the node corresponding to the pid if the kill command was successful
            printf("Process %d  terminated. \n", pid);
        } else {
            printf("Error: Process %d failed to terminate.\n", pid);
            head = deleteNode(head, pid); // removes from list since it was busted and not real in the first place.
        }
    }
}

/* function: func_BGstop
 * ---------------------
 * This function stops the process associated with the PID passed to it
 * using the kill() function and passing SIGSTOP as a parameter.
 * ---------------------
 * Parameters: str_pid = a string representation of a PID.
 * returns: Nothing
 */
void func_BGstop(char * str_pid){
    if (str_pid == NULL) {
        printf("Error: pid is NULL\n");
        return;
    }
    pid_t pid = atoi(str_pid);
    if(!PifExist(head,pid)){ // checks if the pid even exists
        printf("Error: Process %d does not exist.\n", pid);
    } else {
        if  (kill(pid,SIGSTOP) != 0 ){
            printf("Error: Process %d failed to stop.\n", pid);
        } else{
            printf("Process %d stopped successfully.\n", pid);
        }
    }
}


/* function: func_BGstart
 * ----------------------
 * This function terminates the process associated with the PID passed to it
 * using the kill() function and passing SIGCONT as a parameter.
 * ----------------------
 * Parameters: str_pid = a string representation of a PID.
 * returns: Nothing
 */
void func_BGstart(char * str_pid){
    if (str_pid == NULL) {
        printf("Error: pid is NULL\n");
        return;
    }
    pid_t pid = atoi(str_pid);
    if(!PifExist(head,pid)){ // checks if the pid even exists
        printf("Error: Process %d does not exist.\n", pid);
    } else {
        if  (kill(pid,SIGCONT) != 0 ){
            printf("Error: Process %d failed to start.\n", pid);
        }else{
            printf("Process %d started successfully.\n", pid);
        }
    }
}

/* function: func_pstat
 * ----------------------
 * This function prints out the following parameters of the process associated with the pid passed to it
 * comm, state, utime, stime, rss, voluntary_ctxt_switches, nonvoluntary_ctxt_switches
 * ----------------------
 * Parameters: str_pid = a string representation of a pid.
 * returns: Nothing
 */
void func_pstat(char *str_pid){
    if (str_pid == NULL) { // pid invalid
        printf("Error: pid is NULL\n");
        return;
    }
    char stat[MAX_LINE_LEN];
    char status[MAX_LINE_LEN];
    pid_t pid = atoi(str_pid);

    if(!PifExist(head,pid)){ // checks if the pid even exists
        printf("Error: Process %d does not exist. \n", pid);
        return;
    } else {
        // getting /proc file paths and parsing through them
        snprintf(stat, sizeof(stat), "/proc/%d/stat", pid);
        snprintf(status, sizeof(status), "/proc/%d/status", pid);
        fileReaderStat(stat);
        fileReaderStatus(status);
    }
}


/*
 * Code segment provided by instructor
 */
int main(){
    char user_input_str[50];
    while (true) {
      printf("Pman: > ");
      fgets(user_input_str, 50, stdin);
      printf("User input: %s \n", user_input_str);
      char * ptr = strtok(user_input_str, " \n");
      if(ptr == NULL){
        continue;
      }
      char * lst[50];
      int index = 0;
      lst[index] = ptr;
      index++;
      while(ptr != NULL){
        ptr = strtok(NULL, " \n");
        lst[index]=ptr;
        index++;
      }
      if (strcmp("bg",lst[0]) == 0){
        func_BG(lst);
      } else if (strcmp("bglist",lst[0]) == 0) {
        func_BGlist();
      } else if (strcmp("bgkill",lst[0]) == 0) {
        func_BGkill(lst[1]);
      } else if (strcmp("bgstop",lst[0]) == 0) {
        func_BGstop(lst[1]);
      } else if (strcmp("bgstart",lst[0]) == 0) {
        func_BGstart(lst[1]);
      } else if (strcmp("pstat",lst[0]) == 0) {
        func_pstat(lst[1]);
      } else if (strcmp("q",lst[0]) == 0) {
        printf("Bye Bye \n");
        exit(0);
      } else {
        printf("Invalid input\n");
      }
    }

  return 0;
}

