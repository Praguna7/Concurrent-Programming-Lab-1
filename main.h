#ifndef MAIN
#define MAIN

#include <pthread.h>
#include <stdio.h>
#include "linked_list.h"

#define MAX_VALUE 65535  // 2^16 - 1

typedef struct {
    int ins_ops_limit;
    int mem_ops_limit;
    int del_ops_limit;
    int total_ops_limit;

} operation_limits;

typedef struct {
    struct list_node_s **head;
    operation_limits *op_limits;
} thread_args;

#endif