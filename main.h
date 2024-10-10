#ifndef MAIN
#define MAIN

#include <stdatomic.h>
#include <pthread.h>
#include <stdio.h>
#include "linked_list.h"

#define MAX_VALUE 65535  // 2^16 - 1

typedef struct {
    int ins_ops_limit;
    int mem_ops_limit;
    int del_ops_limit;
    int total_ops_limit;

    atomic_int ins_ops_count;
    atomic_int mem_ops_count;
    atomic_int del_ops_count;
    atomic_int total_ops_count;

} operation_limits;

typedef struct {
    struct list_node_s **head;
    operation_limits *op_limits;
} thread_args;

#endif