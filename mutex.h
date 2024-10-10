#ifndef MUTEX
#define MUTEX

#include <pthread.h>
#include <stdio.h>
#include "linked_list.h"
#include "main.h"


int test_mutex (int n, int m, float m_insert_frac, float m_member_frac, float m_delete_frac, int thread_count);
void thread_func_mutex(void* args);

#endif