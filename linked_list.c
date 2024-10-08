#include <stdio.h>
#include <stdlib.h>
#include "linked_list.h"


int Member(int value, struct list_node_s* head_p){
    struct list_node_s* curr_p = head_p;

    while (curr_p != NULL && curr_p->data<value)
    {
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data>value){
        return 0;
    }
    else{
        return 1;
    }
    
}

int Insert(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data<value)
    {
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data >value){
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL){
            *head_pp = temp_p; // When initial linked list is empty
        }
        else{
            pred_p->next = temp_p;
        }
        return 1;
    }
    else{ // Value already in list
        return 0; 
    }
}

int Delete(int value, struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while(curr_p != NULL && curr_p->data<value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p != NULL && curr_p->data == value){
        if (pred_p == NULL){ // Delete head node
            *head_pp = curr_p->next;
            free(curr_p);
        }
        else{
            pred_p->next = curr_p->next;
            free(curr_p);
        }
        return 1;
    }
    else{ // Value dossn't exist in list
        return 0;
    }
}

void PrintList(struct list_node_s* head) {
    struct list_node_s* curr = head;
    while (curr != NULL) {
        printf("%d -> ", curr->data);
        curr = curr->next;
    }
    printf("NULL\n");
}

// int main() {
//     struct list_node_s* head = NULL; // Initialize head as NULL

//     Insert(5, &head);
//     Insert(3, &head);
//     Insert(8, &head);
//     Insert(1, &head);

//     PrintList(head); // Output the list

//     // Free the list (cleanup)
//     struct list_node_s* temp;
//     while (head != NULL) {
//         temp = head;
//         head = head->next;
//         free(temp);
//     }

//     return 0;
// }