#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "rw_lock.h"
#include "serial.h"
#include <pthread.h>
#include "linked_list.h"

// Read-Write lock
pthread_rwlock_t rw_lock;

void thread_func_rw(void* args){

    thread_args *t_args = (thread_args*) args;  // Cast back to correct type
    struct list_node_s **head = t_args->head;
    operation_limits *op_limits = t_args->op_limits;

    srand(time(0) + (unsigned int) pthread_self());  // Seed random number generation for each thread
    
    // Per-thread counters
    int total_ops_count = 0;
    int ins_ops_count = 0;
    int mem_ops_count = 0;
    int del_ops_count = 0;

    int operation;
    int rand_val;

    

    while (total_ops_count < op_limits->total_ops_limit){
        operation = rand()%3;  // Randomly choose operation (0=insert, 1=member, 2=delete)
        rand_val = rand()%MAX_VALUE;

        if(operation==0 && ins_ops_count < op_limits->ins_ops_limit){
            pthread_rwlock_wrlock(&rw_lock);
            Insert(rand_val,head);
            pthread_rwlock_unlock(&rw_lock);

            ins_ops_count++;
            total_ops_count++;
        }
        else if(operation==1 && mem_ops_count < op_limits->mem_ops_limit) {
            pthread_rwlock_rdlock(&rw_lock);
            Member(rand_val,*head);
            pthread_rwlock_unlock(&rw_lock);

            mem_ops_count++;
            total_ops_count++;
        }
        else if (operation==2 && del_ops_count < op_limits->del_ops_limit){
            pthread_rwlock_wrlock(&rw_lock);
            Delete(rand_val,head);
            pthread_rwlock_unlock(&rw_lock);

            del_ops_count++;
            total_ops_count++;
        }
    }
}

int test_rwlock(int n, int m, float m_insert_frac, float m_member_frac, float m_delete_frac, int thread_count){

    struct list_node_s* head = NULL;
    operation_limits op_limits;

    op_limits.ins_ops_limit = (int) ceil(m * m_insert_frac / thread_count);
    op_limits.mem_ops_limit = (int) ceil(m * m_member_frac / thread_count);
    op_limits.del_ops_limit = (int) ceil(m * m_delete_frac / thread_count);
    op_limits.total_ops_limit = (int) ceil(m / thread_count);

        populate_list(&head,n); // Populate linked-list with n nodes
        struct timeval start;
        struct timeval stop;

    pthread_t threads[thread_count];
    thread_args t_args[thread_count];

    gettimeofday(&start, NULL);

    for (int i = 0; i < thread_count; i++) {
        t_args[i].head = &head;
        t_args[i].op_limits = &op_limits;

        int res = pthread_create(&threads[i], NULL, (void* (*)(void*)) thread_func_rw, &t_args[i]);
        if (res != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to finish
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&stop, NULL);
    unsigned long time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    // printf("RW_lock program took %lu us\n", time);

    return time;
}
