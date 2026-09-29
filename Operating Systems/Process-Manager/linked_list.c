#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "linked_list.h"


/* function: add_newNode
 * ---------------------
 * This function creates a node containing a pid and path  and adds it to a linked list
 * ---------------------
 * Parameters: head: type Node* The current head of the linked list.
 *             new_pid: type pid_t The pid of the node to be created.
 *             path: type char*  The path of the executable being run in the process.
 * returns: Type Node* The head of the linked list.
 */
Node * add_newNode(Node* head, pid_t new_pid, char * new_path){
    Node *new_node = (struct Node*)malloc(sizeof(struct Node)); //allocate memory space for the new node
    Node *cur = head;

    new_node->pid = new_pid;
    new_node->path = malloc(strlen(new_path) + 1);
    strcpy(new_node->path,new_path);
    new_node->next = NULL;

    if(head == NULL){
        head = new_node; // if list is empty new node is head
    } else {
        while(cur->next != NULL){
            cur = cur->next;
        }
        cur->next = new_node;
    }
	return head;
}

/* function: deleteNode
 * --------------------
 * This function deletes a node from the linked list based on a pid given to the function.
 * If the node does not exist sends an error message.
 * --------------------
 * Parameters: head: type Node* The current head of the linked list.
 *             pid: type pid_t The pid of the node to be deleted.
 * returns: Type Node* The head of the linked list.
 */
Node * deleteNode(Node* head, pid_t pid){
    Node *cur = head;
    Node *deleteNode = head;

    // if the deleted Node is the head
    if (cur != NULL && cur->pid == pid) {
        head = cur->next;
        free(cur);
    // if the deleted node is elsewhere
    } else {
        while(cur != NULL && cur->next != NULL){
            if (cur->next->pid == pid){
                deleteNode = cur->next;
                cur->next =  cur->next->next;
                free(deleteNode);
            }
            cur = cur->next;
        }
    }
	return head;
}

/* function: printList
 * -------------------
 * This function prints the pid and path of all process nodes in the linked list.
 * Also prints the total number of process nodes in the linked list.
 * -------------------
 * Parameters: type Node* supposed to be the head of the list.
 * returns: Nothing
 */
void printList(Node *head){
    int count = 0;
    Node *cur = head;
    while(cur != NULL){
        printf("%d: %s  \n", cur->pid, cur->path);
        count++;
        cur = cur->next;
    }
    printf("Total background jobs: %d \n", count);
}

/* function: PifExist
 * -------------------
 * This function checks if a node with a specific pid exists in the list.
 * returns 1 if yes or 0 if no.
 * -------------------
 * Parameters: node: type Node* supposed to be the head of the list.
 *             pid: type pid_t pid of the node being searched for.
 * returns: 1 if a node with the given pid exists or 0 if no such node exists.
 */
int PifExist(Node *head, pid_t pid){
    Node *cur = head;

    while(cur != NULL) {
        if(cur->pid == pid){
            return 1;
        }
        cur = cur->next;
    }
  	return 0;
}

