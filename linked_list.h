#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "main.h"



// Node structure for the linked list
struct list_node_s {
    int data;
    struct list_node_s* next;
};

// Function prototypes for linked list operations

// Check if a value is a member of the list
int Member(int value, struct list_node_s* head_p);

// Insert a value into the linked list
int Insert(int value, struct list_node_s** head_pp);

// Delete a value from the linked list
int Delete(int value, struct list_node_s** head_pp);

// Print the entire linked list
void PrintList(struct list_node_s* head);

void populate_list(struct list_node_s ** head, int n);

#endif 
