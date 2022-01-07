#include "ADTLinkedList.h"
#include "core.h"

struct jobscheduler{
    int exec_threads; // number of execution threads
    List queue; // a queue that holds submitted jobs / tasks
    pthread_t* tids; // execution threads
    pthread_mutex_t mtx_queue;
    pthread_mutex_t mtx_counter;
    pthread_mutex_t mtx_document;
    int counter;
    int finish;
    pthread_cond_t queue_not_empty;
    pthread_cond_t threads_finished;
    // mutex, condition variable, ...
};

struct job {
    DocID doc_id;
    const char * doc_str;
    ErrorCode errcode;
};

typedef struct jobscheduler* JobScheduler;

typedef struct job* Job;

void initialize_scheduler(int execution_threads, JobScheduler jscheduler);

void submit_job(JobScheduler sch, Job j);

int execute_all_jobs(JobScheduler sch);

int wait_all_tasks_finish(JobScheduler sch);

ErrorCode destroy_scheduler(JobScheduler sch);

void* help_MatchDocument (void* job);
