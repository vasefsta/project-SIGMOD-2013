#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

#include "jobscheduler.h"
#include "core.h"

JobScheduler initialize_scheduler(int execution_threads) {
    JobScheduler jscheduler = malloc(sizeof(struct jobscheduler));
    jscheduler->exec_threads = execution_threads;

    jscheduler->tids = malloc(sizeof(pthread_t)*execution_threads);

    // for (int i = 0; i < execution_threads; i++) {                  
    //     puts("HELLO");
    //     printf("%d %d\n", i, execution_threads);
    //     if (pthread_create(&(jscheduler->tids[i]), 0, help_MatchDocument, 0)) {         // Δημιουργεία του i Thread 
    //         perror(" ");
    //         exit(-1);
    //     }
    // }
    jscheduler->queue = list_create(NULL);

    if (pthread_mutex_init(&(jscheduler->mtx), 0)) {
        perror(" ");
        exit(-1);
    }

    if (pthread_cond_init(&(jscheduler->cond_wakeup), 0)) {
        perror(" ");
        exit(-1);
    }

    if (pthread_cond_init(&(jscheduler->cond_sleep), 0)) {
        perror(" ");
        exit(-1);
    }

    return jscheduler;
}


int submit_job(JobScheduler sch, Job j) {
    return -1;
}

int execute_all_jobs(JobScheduler sch) {
    return -1;
}

int wait_all_tasks_finish(JobScheduler sch) {
    return -1;
}

int destroy_scheduler(JobScheduler sch) {
    return -1;
}