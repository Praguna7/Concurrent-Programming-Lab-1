#include <stdio.h>
#include <time.h>
#include <math.h>
#include "linked_list.h"
#include "serial.h"
#include <sys/time.h>


int test_serial(int n, int m, float m_insert_frac, float m_member_frac, float m_delete_frac){

    struct list_node_s* head = NULL;
    populate_list(&head,n);

    srand(time(0)); // Use current time as random seed

    int ins_ops_limit = (int) ceil(m * m_insert_frac);
    int mem_ops_limit = (int) ceil(m * m_member_frac);
    int del_ops_limit = (int) ceil(m * m_delete_frac);

    int ins_ops_count = 0;
    int mem_ops_count = 0;
    int del_ops_count = 0;   
    int total_ops_count = 0;

    int operation;
    int rand_val;

    struct timeval start;
    struct timeval stop;
    gettimeofday(&start, NULL); 
    while(total_ops_count<m){
        // printf("tot ops %i",total_ops_count);
        operation = rand()%3;
        rand_val = rand()%MAX_VALUE;

        if(operation==0 && ins_ops_count<=ins_ops_limit){
            Insert(rand_val,&head);
            ins_ops_count++;
            total_ops_count++;
        }
        else if(operation==1 && mem_ops_count<=mem_ops_limit){
            Member(rand_val,head);
            mem_ops_count++;
            total_ops_count++;
            
        }
        else if (operation==2 && del_ops_count<=del_ops_limit){
            Delete(rand_val,&head);
            del_ops_count++;
            total_ops_count++;
        }
    }

    gettimeofday(&stop, NULL); 
    unsigned long time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    // printf("Serial program took %lu us\n", time); 

    return time;
    
};


