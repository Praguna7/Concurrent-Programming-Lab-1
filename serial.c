#include <stdio.h>
#include <time.h>
#include "linked_list.h"
#include "serial.h"
#define MAX_VALUE 65535  // 2^16 - 1

int test_serial(struct list_node_s ** head, int n, int m, float m_insert_frac, double m_member_frac, double m_delete_frac){

    populate_list(head,n);

    srand(time(0)); // Use current time as random seed

    int ins_ops_limit = (int) (m * m_insert_frac);
    int mem_ops_limit = (int) (m * m_member_frac);
    int del_ops_limit = (int) (m * m_delete_frac);

    int ins_ops_count = 0;
    int mem_ops_count = 0;
    int del_ops_count = 0;   
    int total_ops_count = 0;

    int operation;
    int rand_val;

    while(total_ops_count<m){
        operation = rand()%3;
        rand_val = rand()%MAX_VALUE;

        if(operation==0 && ins_ops_count<ins_ops_limit){
            Insert(rand_val,head);
            ins_ops_count++;
            total_ops_count++;
        }
        else if(operation==1 && mem_ops_count<mem_ops_limit){
            Member(rand_val,*head);
            mem_ops_count++;
            total_ops_count++;
            
        }
        else if (operation==2 && del_ops_count<del_ops_limit){
            Delete(rand_val,head);
            del_ops_count++;
            total_ops_count++;
        }
    }
    
};

void populate_list(struct list_node_s ** head, int n){
    int count = 0;
    while(count<n){
        int value = rand() % MAX_VALUE;
        if(Insert(value,head)){
            count++;
        }
    }
};

int main () {
    struct list_node_s* head = NULL;
    test_serial(&head,50,1000,0.9,0.05,0.05);
    PrintList(head);
    
    return 0;
};