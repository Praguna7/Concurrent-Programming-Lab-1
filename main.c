#include "main.h"
#include "linked_list.h"
#include "rw_lock.h"
#include "mutex.h"
#include "serial.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define DESIRED_ACCURACY 0.05  // Desired accuracy: ±5%
#define CONFIDENCE_LEVEL 1.96  // t-value for 95% confidence level

// Function to calculate the mean of an array
float calculate_mean(int times[], int n) {
    float sum = 0.0;
    for (int i = 0; i < n; i++) {
        sum += times[i];
    }
    return sum / n;
}

// Function to calculate the standard deviation of an array
float calculate_stddev(int times[], int n, float mean) {
    float variance = 0.0;
    for (int i = 0; i < n; i++) {
        variance += (times[i] - mean) * (times[i] - mean);
    }
    variance /= n;
    return sqrt(variance);
}

// Function to calculate the confidence interval
float calculate_confidence_interval(float stddev, int n) {
    return CONFIDENCE_LEVEL * (stddev / sqrt(n));
}

// Function to calculate the number of iterations required
int calculate_required_iterations(float stddev, float mean) {
    return pow((CONFIDENCE_LEVEL * stddev) / (DESIRED_ACCURACY * mean), 2);
}

// Printing helper function
void print_f_c(FILE *fptr, const char *format, ...) {
    va_list args;
    
    // Print to console
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // Print to file
    va_start(args, format);
    vfprintf(fptr, format, args);
    va_end(args);
}


// Function to run each case with specific proportions and print the results
int run_case(float m_mem, float m_ins, float m_del, int iterations, int n, int m, const char* case_name, int find_iteration_count,FILE *fptr) {
    int ser_times[iterations];
    int mut_times_t1[iterations];
    int mut_times_t2[iterations];
    int mut_times_t4[iterations];
    int mut_times_t8[iterations];

    int rw_times_t1[iterations];
    int rw_times_t2[iterations];
    int rw_times_t4[iterations];
    int rw_times_t8[iterations];
    // printf("\nIterating %s: ",case_name);
    for (int i = 0; i < iterations; i++) {
        // print(fptr,"%i",i);
        ser_times[i] = test_serial(n, m, m_mem, m_ins, m_del);

        mut_times_t1[i] = test_mutex(n, m, m_mem, m_ins, m_del, 1);
        mut_times_t2[i] = test_mutex(n, m, m_mem, m_ins, m_del, 2);
        mut_times_t4[i] = test_mutex(n, m, m_mem, m_ins, m_del, 4);
        mut_times_t8[i] = test_mutex(n, m, m_mem,m_ins, m_del, 8);

        rw_times_t1[i] = test_rwlock(n, m, m_mem, m_ins, m_del, 1);
        rw_times_t2[i] = test_rwlock(n, m, m_mem, m_ins, m_del, 2);
        rw_times_t4[i] = test_rwlock(n, m, m_mem, m_ins, m_del, 4);
        rw_times_t8[i] = test_rwlock(n, m, m_mem, m_ins, m_del, 8);
    }
    print_f_c(fptr,"\n");

    // Calculate means and standard deviations
    float mean_ser = calculate_mean(ser_times, iterations);
    float mean_mut_t1 = calculate_mean(mut_times_t1, iterations);
    float mean_mut_t2 = calculate_mean(mut_times_t2, iterations);
    float mean_mut_t4 = calculate_mean(mut_times_t4, iterations);
    float mean_mut_t8 = calculate_mean(mut_times_t8, iterations);

    float mean_rw_t1 = calculate_mean(rw_times_t1, iterations);
    float mean_rw_t2 = calculate_mean(rw_times_t2, iterations);
    float mean_rw_t4 = calculate_mean(rw_times_t4, iterations);
    float mean_rw_t8 = calculate_mean(rw_times_t8, iterations);

    float stddev_ser = calculate_stddev(ser_times, iterations, mean_ser);
    float stddev_mut_t1 = calculate_stddev(mut_times_t1, iterations, mean_mut_t1);
    float stddev_mut_t2 = calculate_stddev(mut_times_t2, iterations, mean_mut_t2);
    float stddev_mut_t4 = calculate_stddev(mut_times_t4, iterations, mean_mut_t4);
    float stddev_mut_t8 = calculate_stddev(mut_times_t8, iterations, mean_mut_t8);

    float stddev_rw_t1 = calculate_stddev(rw_times_t1, iterations, mean_rw_t1);
    float stddev_rw_t2 = calculate_stddev(rw_times_t2, iterations, mean_rw_t2);
    float stddev_rw_t4 = calculate_stddev(rw_times_t4, iterations, mean_rw_t4);
    float stddev_rw_t8 = calculate_stddev(rw_times_t8, iterations, mean_rw_t8);

    int requred_min_iterations = 0;

    if(find_iteration_count ==1 ){
        int num_it;
        num_it = calculate_required_iterations(stddev_ser,mean_ser);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it = calculate_required_iterations(stddev_mut_t1,mean_mut_t1);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it  = calculate_required_iterations(stddev_mut_t2,mean_mut_t2);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it = calculate_required_iterations(stddev_mut_t4,mean_mut_t4);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it = calculate_required_iterations(stddev_mut_t8,mean_mut_t8);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it  = calculate_required_iterations(stddev_rw_t1,mean_rw_t1);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it = calculate_required_iterations(stddev_rw_t2,mean_rw_t2);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it = calculate_required_iterations(stddev_rw_t4,mean_rw_t4);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        num_it = calculate_required_iterations(stddev_rw_t8,mean_rw_t8);
        if(num_it>requred_min_iterations){
            requred_min_iterations = num_it;
            // print_f_c(fptr,"\n iterations %i \n",num_it);
        }
        return requred_min_iterations;

    }
    else{
         // Print case details
        print_f_c(fptr,"-------------------------\n");
        print_f_c(fptr,"--------- %s --------\n", case_name);
        print_f_c(fptr,"-------------------------\n");
        print_f_c(fptr,"Member Proportion: %.2f\n", m_mem);
        print_f_c(fptr,"Insert Proportion: %.2f\n", m_ins);
        print_f_c(fptr,"Delete Proportion: %.2f\n", m_del);
        print_f_c(fptr,"-------------------------\n");

        // Print results for the current case
        print_f_c(fptr,"\nResults for %s:\n", case_name);
        print_f_c(fptr,"+---------+-----------+-------------+-------------+\n");
        print_f_c(fptr,"| Threads | Type      |  Mean (us)  | Stddev (us) |\n");
        print_f_c(fptr,"+---------+-----------+-------------+-------------+\n");
        print_f_c(fptr,"|    1    | Serial    | %11.2f | %11.2f |\n", mean_ser, stddev_ser);
        print_f_c(fptr,"+---------+-----------+-------------+-------------+\n");

        print_f_c(fptr,"|    1    | Mutex     | %11.2f | %11.2f |\n", mean_mut_t1, stddev_mut_t1);
        print_f_c(fptr,"|    1    | RWLock    | %11.2f | %11.2f |\n", mean_rw_t1, stddev_rw_t1);
        print_f_c(fptr,"+---------+-----------+-------------+-------------+\n");

        print_f_c(fptr,"|    2    | Mutex     | %11.2f | %11.2f |\n", mean_mut_t2, stddev_mut_t2);
        print_f_c(fptr,"|    2    | RWLock    | %11.2f | %11.2f |\n", mean_rw_t2, stddev_rw_t2);
        print_f_c(fptr,"+---------+-----------+-------------+-------------+\n");

        print_f_c(fptr,"|    4    | Mutex     | %11.2f | %11.2f |\n", mean_mut_t4, stddev_mut_t4);
        print_f_c(fptr,"|    4    | RWLock    | %11.2f | %11.2f |\n", mean_rw_t4, stddev_rw_t4);
        print_f_c(fptr,"+---------+-----------+-------------+-------------+\n");

        print_f_c(fptr,"|    8    | Mutex     | %11.2f | %11.2f |\n", mean_mut_t8, stddev_mut_t8);
        print_f_c(fptr,"|    8    | RWLock    | %11.2f | %11.2f |\n", mean_rw_t8, stddev_rw_t8);
        print_f_c(fptr,"+---------+-----------+-------------+-------------+\n");
        return 0;
    }
   }

int calculate_minimum_required_iterations(int initial_iterations,int n,int m){
    int it_max;
    int iterations=1;
    FILE *fptr = fopen("find_required_iterations.txt", "w");

    print_f_c(fptr,"Finding required number of iterations by running %i iterations initially per each case.\n",initial_iterations);
    it_max = run_case(0.99, 0.005, 0.005, initial_iterations, n, m, "Case 1",1,fptr);
    if (it_max>iterations){
            iterations = it_max;
    }
    it_max = run_case(0.90, 0.05, 0.05, initial_iterations, n, m, "Case 2",1,fptr);
    if (it_max>iterations){
            iterations = it_max;
    }
    it_max = run_case(0.50, 0.25, 0.25, initial_iterations, n, m, "Case 3",1,fptr);
    if (it_max>iterations){
            iterations = it_max;
    }
    print_f_c(fptr,"\nMinimum required number of iterations is %i.\n",iterations);
    fclose(fptr);
    return iterations;
}

int main() {
    int n = 1000;
    int m = 10000;
    int iterations=238;
    int initial_iterations=100;
    FILE *fptr = fopen("results.txt", "w");
    
    // iterations = calculate_minimum_required_iterations(initial_iterations,n,m);
    
    print_f_c(fptr,"\nRunning each case with  %i iterations.\n",iterations);

    run_case(0.99, 0.005, 0.005, iterations, n, m, "Case 1",0,fptr);
    run_case(0.90, 0.05, 0.05, iterations, n, m, "Case 2",0,fptr);
    run_case(0.50, 0.25, 0.25, iterations, n, m, "Case 3",0,fptr);

    fclose(fptr);
    return 0;
}
