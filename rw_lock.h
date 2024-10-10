#ifndef RW_LOCK
#define RW_LOCK

#include <stdatomic.h>
#include <pthread.h>
#include <stdio.h>
#include "linked_list.h"


int test_rwlock(int n, int m, float m_insert_frac, float m_member_frac, float m_delete_frac, int thread_count);
void thread_func_rw(void* args);

#endif