#include "ADTLinkedList.h"
#include "core.h"

struct jobscheduler{
    int exec_threads; // number of execution threads
    List queue; // a queue that holds submitted jobs / tasks
    pthread_t* tids; // execution threads
    pthread_mutex_t mtx;
    pthread_cond_t cond_wakeup;
    pthread_cond_t cond_sleep;
    // mutex, condition variable, ...
};

struct job {
    DocID doc_id;
    const char * doc_str;
    ErrorCode errcode;
};

typedef struct jobscheduler* JobScheduler;

typedef struct job* Job;

JobScheduler initialize_scheduler(int execution_threads);

int submit_job(JobScheduler sch, Job j);

int execute_all_jobs(JobScheduler sch);

int wait_all_tasks_finish(JobScheduler sch);

int destroy_scheduler(JobScheduler sch);

void* help_MatchDocument (void* job);
