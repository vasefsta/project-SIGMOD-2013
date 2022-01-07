#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

#include "jobscheduler.h"
#include "core.h"

void initialize_scheduler(int execution_threads, JobScheduler jscheduler) {
    jscheduler->exec_threads = execution_threads;

    jscheduler->tids = malloc(sizeof(pthread_t)*execution_threads);

    jscheduler->queue = list_create(NULL);

    if (pthread_mutex_init(&(jscheduler->mtx_counter), 0)) {
        perror(" ");
        exit(-1);
    }

    if (pthread_mutex_init(&(jscheduler->mtx_queue), 0)) {
        perror(" ");
        exit(-1);
    }

    if (pthread_mutex_init(&(jscheduler->mtx_document), 0)) {
        perror(" ");
        exit(-1);
    }

    if (pthread_cond_init(&(jscheduler->queue_not_empty), 0)) {
        perror(" ");
        exit(-1);
    }

    if (pthread_cond_init(&(jscheduler->threads_finished), 0)) {
        perror(" ");
        exit(-1);
    }

    if (pthread_cond_init(&(jscheduler->new_doc), 0)) {
        perror(" ");
        exit(-1);
    }

    jscheduler->counter = 0;

    jscheduler->finish = 0;

    execute_all_jobs(jscheduler);

}


void submit_job(JobScheduler sch, Job j) {


    if (pthread_mutex_lock(&(sch->mtx_queue))) {
        perror(" ");
        return ;
    }

    list_insert(sch->queue, j);

    if (pthread_mutex_lock(&(sch->mtx_counter))) {
        perror(" ");
        return ;
    }

    sch->counter++;
    
    
    if (pthread_mutex_unlock(&(sch->mtx_counter))) {
        perror(" ");
        return ;  
    }


    if (pthread_mutex_unlock(&(sch->mtx_queue))) {
        perror(" ");
        return ;
    }
    
    if (pthread_cond_signal(&(sch->queue_not_empty))) {
        perror(" ");
        return ;
    }
     
}

int execute_all_jobs(JobScheduler sch) {
    for (int i = 0; i < sch->exec_threads; i++) {                  
        if (pthread_create(&(sch->tids[i]), 0, help_MatchDocument, 0)) {         // Create i nth thread.
            perror(" ");
            exit(-1);
        }
    }

    return 0;
}

int wait_all_tasks_finish(JobScheduler sch) {
    pthread_cond_broadcast(&(sch->queue_not_empty));
    for (int i = 0; i < sch->exec_threads; i++) {
        if (pthread_join(sch->tids[i], 0)) {
            perror(" ");
            return EC_FAIL;
        }
    }

    return EC_SUCCESS;
}

ErrorCode destroy_scheduler(JobScheduler sch) {
    sch->finish = 1;

    if (wait_all_tasks_finish(sch) == EC_FAIL) 
        return EC_FAIL;
    
    pthread_cond_destroy(&(sch->queue_not_empty));
	pthread_cond_destroy(&(sch->threads_finished));
	pthread_mutex_destroy(&(sch->mtx_counter));
	pthread_mutex_destroy(&(sch->mtx_queue));

    list_destroy(sch->queue, NULL);

    free(sch->tids);

    free(sch);

    return EC_SUCCESS;
}