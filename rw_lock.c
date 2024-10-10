#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <stdatomic.h>
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

    int thread_id = pthread_self(); 
    srand(time(0)+thread_id); // Use current time + thread id as random seed

    int operation;
    int rand_val;

    while (atomic_load(&op_limits->total_ops_count) < op_limits->total_ops_limit)
    {
        operation = rand()%3;
        rand_val = rand()%MAX_VALUE;

        if(operation==0 && atomic_load(&(op_limits->ins_ops_count)) < op_limits->ins_ops_limit){

            pthread_rwlock_wrlock(&rw_lock);
            Insert(rand_val,head);
            pthread_rwlock_unlock(&rw_lock);

            atomic_fetch_add(&(op_limits->ins_ops_count),1);
            atomic_fetch_add(&(op_limits->total_ops_count),1);
        }
        else if(operation==1 && atomic_load(&(op_limits->mem_ops_count)) < op_limits->mem_ops_limit){

            pthread_rwlock_rdlock(&rw_lock);
            Member(rand_val,*head);
            pthread_rwlock_unlock(&rw_lock);

            atomic_fetch_add(&(op_limits->mem_ops_count),1);
            atomic_fetch_add(&(op_limits->total_ops_count),1);
        }

        else if(operation==2 && atomic_load(&(op_limits->del_ops_count)) < op_limits->del_ops_limit){

            pthread_rwlock_wrlock(&rw_lock);
            Delete(rand_val,head);
            pthread_rwlock_unlock(&rw_lock);

            atomic_fetch_add(&(op_limits->del_ops_count),1);
            atomic_fetch_add(&(op_limits->total_ops_count),1);
        }

    };
    
    
};

int test_rwlock(int n, int m, float m_insert_frac, float m_member_frac, float m_delete_frac, int thread_count){

        struct list_node_s* head = NULL;
        operation_limits op_limits;

        op_limits.ins_ops_limit = (int) ceil(m * m_insert_frac);
        op_limits.mem_ops_limit = (int) ceil(m * m_member_frac);
        op_limits.del_ops_limit = (int) ceil(m * m_delete_frac);
        op_limits.total_ops_limit = m;
        // printf("%i %i %i %i",op_limits.ins_ops_limit,op_limits.mem_ops_limit,op_limits.del_ops_limit,op_limits.total_ops_limit); 

        atomic_init(&(op_limits.ins_ops_count), 0);
        atomic_init(&(op_limits.mem_ops_count), 0);
        atomic_init(&(op_limits.del_ops_count), 0);
        atomic_init(&(op_limits.total_ops_count), 0);


        populate_list(&head,n); // Populate linked-list with n nodes

        struct timeval start;
        struct timeval stop;

        pthread_t threads[thread_count];
        int thread_ids[thread_count];
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
        };

        // Wait for all threads to finish
        for (int i = 0; i < thread_count; i++) {
            pthread_join(threads[i], NULL);
        }

        gettimeofday(&stop, NULL);
        unsigned long time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
        // printf("RW_lock program took %lu us\n", time); 

        return time;
};
