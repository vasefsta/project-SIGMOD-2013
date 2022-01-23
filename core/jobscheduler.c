#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>

#include "jobscheduler.h"
#include "core.h"


// Initializes the scheduler for threads.
void initialize_scheduler(int execution_threads, JobScheduler jscheduler) {
    jscheduler->exec_threads = execution_threads;

    // Here we create double threads to use them in StartQuery and MatchDocument.
    jscheduler->tids = malloc(sizeof(pthread_t)*execution_threads*2);

    // The queue that holds submitted Documents.
    jscheduler->queue = list_create(NULL);

    // The queue that holds submitted Queries.
    jscheduler->query = list_create(NULL);


    // Initialize mutex.
    if (pthread_mutex_init(&(jscheduler->mtx_counter), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
    if (pthread_mutex_init(&(jscheduler->mtx_counter_query), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
    if (pthread_mutex_init(&(jscheduler->mtx_queue), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
    if (pthread_mutex_init(&(jscheduler->mtx_query), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
    if (pthread_mutex_init(&(jscheduler->mtx_document), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
    if (pthread_mutex_init(&(jscheduler->mtx_finish), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
     if (pthread_mutex_init(&(jscheduler->mtx_exact_match), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
     if (pthread_mutex_init(&(jscheduler->mtx_hamming_dist), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize mutex.
     if (pthread_mutex_init(&(jscheduler->mtx_edit_dist), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize condition signal.
    if (pthread_cond_init(&(jscheduler->queue_not_empty), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize condition signal.
    if (pthread_cond_init(&(jscheduler->query_not_empty), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize condition signal.
    if (pthread_cond_init(&(jscheduler->threads_finished), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize condition signal.
    if (pthread_cond_init(&(jscheduler->queries_finished), 0)) {
        perror(" ");
        exit(-1);
    }

    // Initialize condition signal.
    if (pthread_cond_init(&(jscheduler->new_doc), 0)) {
        perror(" ");
        exit(-1);
    }

    // Counter for documents
    jscheduler->counter = 0;

    // Variable to let threads know we finished
    jscheduler->finish = 0;

    // Counter for queries
    jscheduler->counter_query = 0;

    execute_all_jobs(jscheduler);

}

// Submit a MatchDocument job
void submit_job(JobScheduler sch, Job j) {


    // Lock the mutex
    if (pthread_mutex_lock(&(sch->mtx_queue))) {
        perror(" ");
        return ;
    }
    
    // Lock the mutex
    if (pthread_mutex_lock(&(sch->mtx_counter))) {
        perror(" ");
        return ;
    }

    // Insert job in list.
    list_insert(sch->queue, j);
    // Increase the counter of jobs.
    sch->counter++;
    
     
    // Signal threads that a new job has been assigned
    if (pthread_cond_signal(&(sch->queue_not_empty))) {
        perror(" ");
        return ;
    }
    

    // Unlock mutex
    if (pthread_mutex_unlock(&(sch->mtx_counter))) {
        perror(" ");
        return ;  
    }

    // Unlock mutex
    if (pthread_mutex_unlock(&(sch->mtx_queue))) {
        perror(" ");
        return ;
    }
   
     
}


// Submit a StartQuery job
void submit_query(JobScheduler sch, Query query) {


    // Lock mutex
    if (pthread_mutex_lock(&(sch->mtx_query))) {
        perror(" ");
        return ;
    }

    // Lock mutex
    if (pthread_mutex_lock(&(sch->mtx_counter_query))) {
        perror(" ");
        return ;
    }

    // Insert a new query in list.
    list_insert(sch->query, query);
    // Increate counter for queries.
    sch->counter_query++;   
     

    // Signal threads that a new Query has been assigned.
    if (pthread_cond_signal(&(sch->query_not_empty))) {
        perror(" ");
        return ;
    }

    // Unlock mutex.
    if (pthread_mutex_unlock(&(sch->mtx_counter_query))) {
        perror(" ");
        return ;
    }

    // Unlock mutex.
    if (pthread_mutex_unlock(&(sch->mtx_query))) {
        perror(" ");
        return ;
    }
   
}


//Create threads.
int execute_all_jobs(JobScheduler sch) {

    // Create exec_threads number of threads gor StartQuery and
    for (int i = 0; i < sch->exec_threads; i++) {                  
        if (pthread_create(&(sch->tids[i]), 0, Insert_Query, 0)) {         // Create i nth thread.
            perror(" ");
            exit(-1);
        }
    }
    
    // Create exec_threads number of threads for MatchDocument and
    for (int i = 0; i < sch->exec_threads; i++) {                  
        if (pthread_create(&(sch->tids[i+sch->exec_threads]), 0, help_MatchDocument, 0)) {         // Create i nth thread.
            perror(" ");
            exit(-1);
        }
    }

    return 0;
}

// Wait for all threads to finish
int wait_all_tasks_finish(JobScheduler sch) {

    // Lock mutex for queue list
    if (pthread_mutex_lock(&(sch->mtx_queue))) {
        perror(" ");
        return EC_FAIL;
    }

    // Signal threads to wake up if they are blocked. 
    pthread_cond_broadcast(&(sch->queue_not_empty));
    
    // Unlock mutex.
    if (pthread_mutex_unlock(&(sch->mtx_queue))) {
        perror(" ");
        return EC_FAIL;
    }

    // Lock mutex for queries list
    if (pthread_mutex_lock(&(sch->mtx_query))) {
        perror(" ");
        return EC_FAIL;
    }

    // Signal threads to wake up if they are blocked. 
    pthread_cond_broadcast(&(sch->query_not_empty));
    
    // Unlock mutex.
     if (pthread_mutex_unlock(&(sch->mtx_query))) {
        perror(" ");
        return EC_FAIL;
    }
   
    // Join all threads back into one.
    for (int i = 0; i < sch->exec_threads*2; i++) {
        if (pthread_join(sch->tids[i], 0)) {
            perror(" ");
            return EC_FAIL;
        }
    }

    return EC_SUCCESS;
}


// Destroy JobScheduler
ErrorCode destroy_scheduler(JobScheduler sch) {

    // Lock mutex
    if (pthread_mutex_lock(&(sch->mtx_finish))) {
        perror(" ");
        exit(-1);
    }

    // Change variable to let threads know we finished.
    sch->finish = 1;

    // Unlock mutex.
    if (pthread_mutex_unlock(&(sch->mtx_finish))) {
        perror(" ");
        exit(-1);
    }

    // Wait for all threads to join.
    if (wait_all_tasks_finish(sch) == EC_FAIL) 
        return EC_FAIL;
    

    // Destroy every single scheduler element
    pthread_cond_destroy(&(sch->queue_not_empty));
	pthread_cond_destroy(&(sch->query_not_empty));
	pthread_cond_destroy(&(sch->threads_finished));
    pthread_cond_destroy(&(sch->queries_finished));
    pthread_cond_destroy(&(sch->new_doc));

	pthread_mutex_destroy(&(sch->mtx_counter));
	pthread_mutex_destroy(&(sch->mtx_queue));
    pthread_mutex_destroy(&(sch->mtx_exact_match));
	pthread_mutex_destroy(&(sch->mtx_hamming_dist));
	pthread_mutex_destroy(&(sch->mtx_edit_dist));
    pthread_mutex_destroy(&(sch->mtx_counter_query));
	pthread_mutex_destroy(&(sch->mtx_query));
    pthread_mutex_destroy(&(sch->mtx_finish));
	pthread_mutex_destroy(&(sch->mtx_document));


    list_destroy(sch->queue, NULL);

    list_destroy(sch->query, NULL);


    free(sch->tids);

    // Destroy scheduler
    free(sch);

    return EC_SUCCESS;
}