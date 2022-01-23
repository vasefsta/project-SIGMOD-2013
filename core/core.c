#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>


#include "ADTIndex.h"
#include "misc.h"
#include "ADTMap.h"
#include "ADTBKTree.h"
#include "core.h"
#include "jobscheduler.h"

#define THREADS 4                                                               // Number of threads for every job.

struct document {
	DocID doc_id;
	unsigned int num_res;
	QueryID* query_ids;
};

typedef struct document* Document;

int ctn = 0;


Index Index_Exact;                                                          
Index Index_Edit;
Index Index_Hamming;

Map Map_Queries;                                                                // Map of all queries

List doc_list;                                                                  // List for ready documents

JobScheduler jscheduler;                                                        // Scheduler for threads.

int compare_entries(Entry e1, Entry e2) {                                       // Compare entries words.
    return strcmp(e1->word, e2->word);
}

int compare_doc(Document doc1, Document doc2) {                                 // Compare documents ids.
    return doc1->doc_id - doc2->doc_id;
}

int comp_spec(Special s1, Special s2){                                          // Compare Specials query ids.
    return (s1->query->queryID) - (s2->query->queryID);
}

int hash_entries(Entry entry) {                                                 // Hash entry by word.
    return hash_string(entry->word);
}

int hash_queries(Query query) {                                                 // Hash query.
    return query->queryID;
}

int hash_spec(Special s){                                                       // Hash Special.
    return s->query->queryID;
}

Map get_map_queries() {                                                         // Return Map_Queries.
    return Map_Queries;
}

Index get_index_exact() {                                                       // Return Index_Exact.
    return Index_Exact;
}

Index get_index_edit() {                                                        // Return Index_Edit.
    return Index_Edit;
}

Index get_index_hamming() {                                                     // Return Index_Hamming.
    return Index_Hamming;
}

void destroy_entry_only(Entry entry) {                                          // Destroy only the entry.
    list_destroy(entry->payload, NULL);
    free(entry);
}


void destroy_entry(Entry entry) {                                               // Destroy entry and word.
    list_destroy(entry->payload, NULL);
    free(entry->word);
    free(entry);
}

void destroy_entry_full(Entry entry) {                                          // Destroy everything in entry.
    list_destroy(entry->payload, (DestroyFunc) free);
    free(entry->word);
    free(entry);
}

void destroy_query(Query query) {                                               // Destroy Query.
    free(query->words);
    free(query);
}


void destroy_document(Document doc){                                            // Destroy Document.
    free(doc->query_ids);
    free(doc);
}

void destroy_special(Special s){                                                // Destroy special.
    list_destroy(s->words, NULL);
    free(s);
}

int compare_ids(const void *a, const void *b) {                                 // Compare ids.
    return ( *(int*)a - *(int*)b );
}

int compare_queries(Query q1, Query q2) {                                       // Compare queries.
    return q1->queryID - q2->queryID;
}

ErrorCode InitializeIndex() {

    // Creates the map that will store all queries.
    Map_Queries = map_create((CompareFunc)compare_queries, 600000);
    if(Map_Queries == NULL)
        return EC_NO_AVAIL_RES;

    // Set the hash_function for Map_Queries.
    map_set_hash_function(Map_Queries, (HashFunc) hash_queries);

    // Create Index to store MT_EXACT_MATCH queries.
    Index_Exact = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entries, 600000);
    if(Index_Exact == NULL)
        return EC_NO_AVAIL_RES;

    // Set the hash_function for Index_Exact.
    map_set_hash_function(index_index(Index_Exact), (HashFunc) hash_entries);

/////////////////////////////////////////////////////////////////////////////////////

    // Create Index to store MT_EDIT_DIST queries.
    Index_Edit = create_index(MT_EDIT_DIST, (CompareFunc)compare_entries, 0);
    if(Index_Edit == NULL)
        return EC_NO_AVAIL_RES;

/////////////////////////////////////////////////////////////////////////////////////

    // Create Index to store MT_HAMMING_DIST queries.
    Index_Hamming = create_index(MT_HAMMING_DIST, (CompareFunc)compare_entries, 0);
    if(Index_Hamming == NULL)
        return EC_NO_AVAIL_RES;

    // Create list to store Documents.
    doc_list = list_create((CompareFunc)compare_doc);
    if (doc_list == NULL)
        return EC_NO_AVAIL_RES;

    // Create the scheduler
    jscheduler = malloc(sizeof(struct jobscheduler));

    // Initialize the scheduler.
    initialize_scheduler(THREADS, jscheduler);

    return EC_SUCCESS;
}

ErrorCode DestroyIndex() {

    // Destroy scheduler
    destroy_scheduler(jscheduler);
    
    // Destroy Index_Exact.
    destroy_entry_index(Index_Exact,(DestroyFunc) destroy_entry);

    // Destroy Index_Edit.
    destroy_entry_index(Index_Edit,(DestroyFunc) destroy_entry);

    // Destroy Index_Hamming.
    destroy_entry_index(Index_Hamming,(DestroyFunc) destroy_entry);

    // Destroy doc_list
    list_destroy(doc_list, (DestroyFunc) destroy_document);
    
    // Destroy Map_Queries.
    map_destroy(Map_Queries, (DestroyFunc)destroy_query);


    return EC_SUCCESS;
}


// Inserts the query in Index_Exact or Index_Edit or Index_Hamming based on query->match_type.
// Thread Routine
void* Insert_Query(void* job) {

    while (1){
        // Lock mutex.
        if (pthread_mutex_lock(&(jscheduler->mtx_query))) {
            perror(" ");
            exit(-1);
        }

        // If there is no job.
        if(list_size(jscheduler->query) == 0){
            // Lock mutex.
            if (pthread_mutex_lock(&(jscheduler->mtx_finish))) {
                perror(" ");
                exit(-1);
            }

            // Check if we finished for good.
            if (jscheduler->finish == 1) {
                // Unlock mutex.
                if (pthread_mutex_unlock(&(jscheduler->mtx_finish))) {
                    perror(" ");
                    exit(-1);
                }

                // Unlock mutex.
                if (pthread_mutex_unlock(&(jscheduler->mtx_query))) {
                    perror(" ");
                    exit(-1);
                }
                // exit from loop
                break;
            }  
            // If there are still jobs left
            else {
                // Unlock mutex.
                if (pthread_mutex_unlock(&(jscheduler->mtx_finish))) {
                    perror(" ");
                    exit(-1);
                }
                // Wait for parent thread to signal there is a new job for us.
                pthread_cond_wait(&jscheduler->query_not_empty, &jscheduler->mtx_query);
            }
        }

        // Get A query to insert from the list.
        Query query = list_remove_first(jscheduler->query);

        // Unlock mutex.
        if (pthread_mutex_unlock(&(jscheduler->mtx_query))) {
            perror(" ");
            exit(-1);
        }

        // If no query was retrieved start over.
        if(query == NULL )
            continue;


        // Array stores every word of query.
        String *Array = Seperate_sentence(query);

        // For every word in query.
        for (int i = 0; i < query->length; i++) {

            //Create a new entry with word = Array[i].
            Entry newentry = create_entry(Array[i], (CompareFunc) compare_queries);
            //Add query to payload.
            list_insert(newentry->payload, query);

            // If query's matchtype is MT_EXACT_MATCH.
            if (query->match_type == MT_EXACT_MATCH) {

                if (pthread_mutex_lock(&(jscheduler->mtx_exact_match))) {
                    perror(" ");
                    exit(-1);
                }       

                // Check if newentry already exists in index.
                Entry entry = map_find(index_index(Index_Exact), newentry);

                if(!entry) {
                    // If no entry was found in index, insert it.
                    map_insert(index_index(Index_Exact), newentry);
                } else {
                    // If entry was found in index, we destroy newentry and insert the query in the existing entry's payload.
                    destroy_entry(newentry);
                    list_insert(entry->payload, query);
                }

                if (pthread_mutex_unlock(&(jscheduler->mtx_exact_match))) {
                    perror(" ");
                    exit(-1);
                }       
            // If query's matchtype is MT_EDIT_DIST.
            } else if (query->match_type == MT_EDIT_DIST) {

                if (pthread_mutex_lock(&(jscheduler->mtx_edit_dist))) {
                    perror(" ");
                    exit(-1);
                }       
                // Insert newentry in Index_Edit.
                bk_insert(index_index(Index_Edit), newentry);

                if (pthread_mutex_unlock(&(jscheduler->mtx_edit_dist))) {
                    perror(" ");
                    exit(-1);
                }   

            // If query's matchtype is MT_HAMMING_DIST.
            } else if (query->match_type == MT_HAMMING_DIST) {

                if (pthread_mutex_lock(&(jscheduler->mtx_hamming_dist))) {
                    perror(" ");
                    exit(-1);
                }   

                // Insert newentry in Index_Hamming.
                bk_insert(index_index(Index_Hamming), newentry);

                if (pthread_mutex_unlock(&(jscheduler->mtx_hamming_dist))) {
                    perror(" ");
                    exit(-1);
                }   

            } else {
                // Wrong query->match_type
                return NULL;
            }
        }

        // Free allocated memory for Array.
        free(Array);

        // Lock mutex.
        if (pthread_mutex_lock(&(jscheduler->mtx_counter_query))) {
            perror(" ");
            exit(-1);
        }
        // Decrease counter for queries.
        jscheduler->counter_query--;

        // If no more queries are to be iinserted
        if(jscheduler->counter_query == 0)
            // Signal(MatchDocument) we finished for now.
            pthread_cond_signal(&(jscheduler->queries_finished));

        // Unlock mutex
        if (pthread_mutex_unlock(&(jscheduler->mtx_counter_query))) {
            perror(" ");
            exit(-1);
        }

    }
    // Exit.
    pthread_exit(0);
}


ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist) {
    //Lock mutex_counter to check jscheduler->counter
    if (pthread_mutex_lock(&(jscheduler->mtx_counter))) {
        perror(" ");
        exit(-1);
    }                                            
    // If couter is > 0 means that a thread did not finish from its routine so we wait.
    if (jscheduler->counter > 0) {                                                           
        if (pthread_cond_wait(&(jscheduler->threads_finished), &(jscheduler->mtx_counter))) {
            perror(" ");
            exit(-1);
        }

    }
    
    if (pthread_mutex_unlock(&(jscheduler->mtx_counter))) {
        perror(" ");
        exit(-1);
    }
    

    int count = 0;                              //Count words.

    // Deduplicate query in case two or more words are the same.
    List ded_query = deduplicated_words_map_small((char *)query_str);

    char buffer[256];
    strcpy(buffer, "");

    for(ListNode node = list_first(ded_query); node != NULL; node = list_find_next(node) ){
        strcat(buffer, list_node_value(node));
        strcat(buffer, " ");
    }
    buffer[strlen(buffer)-1] = '\0';
 
    // Get query_str.
    String saveptr;
    String string = strdup(buffer);
    String token = strtok_r(string, " \t\n", &saveptr);

    // Count words of query.
    while (token != NULL ) {                             
        count++;
        token = strtok_r(NULL, "  \t\n", &saveptr);   
    }

    // Free allocated memory for string.
    free(string);

    // Create the new query.
    Query query = malloc(sizeof(*query));
	query->queryID=query_id;
    query->words = strdup(buffer);
	query->match_type=match_type;
	query->match_dist=match_dist;
    query->length = count;

    // Insert query to Map_Queries.
    map_insert(Map_Queries, query);


    submit_query(jscheduler, query);
    // puts("Finished StartQuery");
    list_destroy(ded_query, free);

    return EC_SUCCESS;
}

ErrorCode EndQuery(QueryID query_id) {
    // For Match Document threads.
    //Lock mutex_counter to check jscheduler->counter
    if (pthread_mutex_lock(&(jscheduler->mtx_counter))) {
        perror(" ");
        exit(-1);
    }                                            
    // If couter is > 0 means that a thread did not finish from its routine so we wait.
    if (jscheduler->counter > 0) {    
        // Wait for threads to finish their job.    
        if (pthread_cond_wait(&(jscheduler->threads_finished), &(jscheduler->mtx_counter))) {
            perror(" ");
            exit(-1);
        }

    }
    // Mutex unlock.
    if (pthread_mutex_unlock(&(jscheduler->mtx_counter))) {
        perror(" ");
        exit(-1);
    }

    // For start query threads
    // Mutex lock.
    if (pthread_mutex_lock(&(jscheduler->mtx_counter_query))) {
        perror(" ");
        exit(-1);
    }                                            
    // If couter is > 0 means that a thread did not finish from its routine so we wait.
    if (jscheduler->counter_query > 0) {                                                           
        if (pthread_cond_wait(&(jscheduler->queries_finished), &(jscheduler->mtx_counter_query))) {
            perror(" ");
            exit(-1);
        }
    }
    
    // Mutex unlock.
    if (pthread_mutex_unlock(&(jscheduler->mtx_counter_query))) {
        perror(" ");
        exit(-1);
    }
    

    // Create a dummy query with id = query_id.
    struct query tmpquery;
    tmpquery.queryID = query_id;

    // Search if query with such query_id exists.
    Query query = map_find(Map_Queries, &tmpquery);

    // If no query was found there is an error.

    // words stores every word of query.
    String* words = Seperate_sentence(query);
    Index index;
    
    // index is assigned the right Index(Exact, edit or hamming)
    if (query->match_type == MT_EXACT_MATCH)
        index = Index_Exact;
    else if (query->match_type == MT_EDIT_DIST)
        index = Index_Edit;
    else
        index = Index_Hamming;
    
    // For every word of query.
    for (int i = 0; i < query->length; i++) {
        
        //Create a dummy entry with word = words[i]
        struct entry tmpentry;
        tmpentry.word = words[i];
        tmpentry.payload = NULL;

        // If query has match type MT_EXACT_MATCH
        if (query->match_type == MT_EXACT_MATCH) {
            // Check if entry exists in index.
            Entry entry = map_find((Map)index_index(index), &tmpentry);
            
            // If no entry was found we free words and exit.
            if (!entry){
                for (int i = 0; i < query->length; i++)
                    free(words[i]);

                free(words);
                return EC_NO_AVAIL_RES;
            } 
            
            // If an entry was found we remove the query from this entry's payload.
            ErrorCode errcode = list_remove(entry->payload, NULL, query); 

            // If an error occured free allocated memory and exit.
            if (errcode == EC_NO_AVAIL_RES){
                
                for (int i = 0; i < query->length; i++)
                    free(words[i]);
                
                free(words);
                return errcode;           
            }
        
        // If Match type is MT_EDIT_DIST or MT_HAMMING_DIST.
        } else if (query->match_type == MT_EDIT_DIST || query->match_type == MT_HAMMING_DIST) {    
            
            // Check if entry exists in index.
            Entry entry = bk_find_entry((BKTree)index_index(index), words[i]);

            //If no entry was found free words.
            if (!entry){
                for (int i = 0; i < query->length; i++)
                    free(words[i]);

                free(words);
                return EC_NO_AVAIL_RES;
            }

            // If an entry was found we remove the query from this entry's payload.
            ErrorCode errcode = list_remove(entry->payload, NULL, query);


            // If an error occured free allocated memory and exit.
            if (errcode == EC_NO_AVAIL_RES) {
                for (int i = 0; i < query->length; i++)
                    free(words[i]);

                free(words);
                return errcode;           
            }
        }
    }

    // Free allocated memory needed.
    for (int i = 0; i < query->length; i++)
        free(words[i]);

    free(words);
    return EC_SUCCESS;

    // puts("FinishedEndQuery");

}

// Match a document with queries in Indexes.
// Thread Routine
void* help_MatchDocument (void* tmpjob) {
    // puts("Entering help_matchdocument");
    while(1) {

        // Mutex lock
        if (pthread_mutex_lock(&(jscheduler->mtx_queue))) {
            perror(" ");
            exit(-1);
        }

        // If there is no job to retrieve.
        if(list_size(jscheduler->queue) == 0){
            if (pthread_mutex_lock(&(jscheduler->mtx_finish))) {
                perror(" ");
                exit(-1);
            }

            // Check if we finished for good
            if (jscheduler->finish == 1) {
                // Unlock mutex.
                if (pthread_mutex_unlock(&(jscheduler->mtx_finish))) {
                    perror(" ");
                    exit(-1);
                }

                // Unlock mutex.
                if (pthread_mutex_unlock(&(jscheduler->mtx_queue))) {
                    perror(" ");
                    exit(-1);
                }
                // Exit from loop.
                break;
            }  
            // If there are still jobs to get
            else {
                // Unlock mutex.
                if (pthread_mutex_unlock(&(jscheduler->mtx_finish))) {
                    perror(" ");
                    exit(-1);
                }
                // Wait for parent thread to inform us.
                pthread_cond_wait(&jscheduler->queue_not_empty, &jscheduler->mtx_queue);                                //wait until there are some data to retrieve
            }
        }

        // Get a job from queue.
        Job job = list_remove_first(jscheduler->queue);

        // Unlock mutex.
        if (pthread_mutex_unlock(&(jscheduler->mtx_queue))) {
            perror(" ");
            exit(-1);
        }

        // If no job was retrieved start over.
        if(job == NULL )
            continue;

        // Create a new document.
        Document document = malloc(sizeof(*document));
        document->doc_id = job->doc_id;
        document->num_res = 0;
        document->query_ids = NULL;

        // Get document's string.cd 
        String doc_str1 = strdup(job->doc_str);
        
        free(job->doc_str);                  // Free job.
        free(job);
        
        
        // list_words contains every word of document's string but has no duplicate strings.
        List list_words = deduplicated_words_map(doc_str1);

        // Create a map to store Specials.
        Map map_result = map_create((CompareFunc)comp_spec, 20000);
        // Set a hash_function for map_result.
        map_set_hash_function(map_result, (HashFunc) hash_spec);

        // Create a list to store the ids of the queries that fully match the document.
        List complete_queries = list_create((CompareFunc)compare_ids);

        // max_thres is 3 ( core.h, line: 152)
        int max_thres = 3;

        // For every word in list_words.
        for (ListNode node = list_first(list_words); node != NULL; node = list_find_next(node)) {
            // Get doc_word.
            String doc_word = list_node_value(node);
            // Look for this word in every index.
            // lookup_entry_index will search if any query matches this document.
            // map_result contains Specials. Struct Special has two members. A query and a list of strings.
            // If query->length is equal to the size of list of strings then query fully matches document.

            // time_t t1 = clock();
            lookup_entry_index(Index_Exact, doc_word, max_thres, map_result, complete_queries, (CompareFunc)compare_queries); 
            // time_t t2 = clock();
            // printf("Exact = %lf\n", (double)(t2-t1)/CLOCKS_PER_SEC );
            // t1 = clock();
            lookup_entry_index(Index_Hamming, doc_word, max_thres, map_result, complete_queries, (CompareFunc)compare_queries); 
            // t2 = clock();
            // printf("Hamming = %lf\n", (double)(t2-t1)/CLOCKS_PER_SEC );

            // t1 = clock();
            lookup_entry_index(Index_Edit, doc_word, max_thres, map_result, complete_queries, (CompareFunc)compare_queries);   
            // t2 = clock();
            // printf("Edit = %lf\n", (double)(t2-t1)/CLOCKS_PER_SEC );
        }   



        // Assign values to document.
        document->num_res = list_size(complete_queries);
        document->query_ids = malloc(sizeof(QueryID)*document->num_res);
        
        // Position of document->query_ids.s
        int i = 0;
        // For every id in complete_queries.
        for(ListNode node = list_first(complete_queries); node != NULL; node = list_find_next(node) ){
            // Get id.
            QueryID* queryid = list_node_value(node);
            // Add id to the table of fully matched queries of document.
            document->query_ids[i] = *queryid;
            i++;
        }

        // Quick sort ids in ascending order.
        qsort(document->query_ids, document->num_res, sizeof(QueryID), (compare_ids));
        
        if (pthread_mutex_lock(&(jscheduler->mtx_document))) {
            perror(" ");
            exit(-1);
        }

        // Insert document in doc_list.
        list_insert(doc_list, document);
        
        // Signal GetNextAvailRes 
        pthread_cond_signal(&(jscheduler->new_doc));

        // Unlock mutex.
        pthread_mutex_unlock(&(jscheduler->mtx_document));

        // Destroy allocated memory needed.
        list_destroy(list_words, free);
        list_destroy(complete_queries, free);
        map_destroy(map_result, (DestroyFunc)destroy_special);
        free(doc_str1);

        
        // Lock mutex.
        if (pthread_mutex_lock(&(jscheduler->mtx_counter))) {
            perror(" ");
            exit(-1);
        }
        // Decrease scheduler counter.
        jscheduler->counter--;

        // If all jobs are finished
        if(jscheduler->counter == 0)
            // Signal StartQuery and EndQuery.
            pthread_cond_signal(&(jscheduler->threads_finished));

        // Unlock mutex.
        if (pthread_mutex_unlock(&(jscheduler->mtx_counter))) {
            perror(" ");
            exit(-1);
        }

    }
    // Exit.
    pthread_exit(0);
}

ErrorCode MatchDocument (DocID doc_id, const char * doc_str) {

    // Mutex lock
    if (pthread_mutex_lock(&(jscheduler->mtx_counter_query))) {
        perror(" ");
        exit(-1);
    }                                            
    // If couter is > 0 means that a thread did not finish from its routine so we wait.
    if (jscheduler->counter_query > 0) { 
        // Wait for a signal.   
        if (pthread_cond_wait(&(jscheduler->queries_finished), &(jscheduler->mtx_counter_query))) {
            perror(" ");
            exit(-1);
        }
    }
    
    // Unlock mutex.
    if (pthread_mutex_unlock(&(jscheduler->mtx_counter_query))) {
        perror(" ");
        exit(-1);
    }

    // Create a new job.
    Job job = malloc(sizeof(struct job));

    job->doc_id = doc_id;
    job->doc_str = strdup(doc_str);
    job->errcode = EC_SUCCESS;

    // Submit job to scheduler.
    submit_job(jscheduler, job);


    return EC_SUCCESS;
}

ErrorCode GetNextAvailRes (DocID * p_doc_id, unsigned int * p_num_res, QueryID ** p_query_ids) {

    // Lock mutex.
    if (pthread_mutex_lock(&jscheduler->mtx_document)) {
        perror(" ");
        exit(-1);
    }
    // If there are no documents to get
    if (list_size(doc_list) == 0) {
        // Wait untill a thread signals us.
        if (pthread_cond_wait(&jscheduler->new_doc, &jscheduler->mtx_document)) {
            perror(" ");
            exit(-1);
        }

    }

    // Get document from list.
    Document document = list_remove_first(doc_list);
    

    // Unlock mutex.
    if (pthread_mutex_unlock(&(jscheduler->mtx_document))) {
        perror(" ");
        exit(-1);
    }


    // If no document was found in doc_list return no availavle result.
    if (!document)
        return EC_NO_AVAIL_RES;

    // Assign to the following pointers the document values.
    *p_doc_id = document->doc_id;
    *p_num_res = document->num_res;

    // Crreate a temp array that holds QueryIDs.
    QueryID* tmp = malloc(sizeof(QueryID) * document->num_res);

    // If there are ids in document->query_ids copy document->query_ids to tmp.
    if ((*p_num_res) != 0) {
        for (int i = 0; i < (*p_num_res); i++){
            tmp[i] = document->query_ids[i];            
        }
    }

    // Free document.
    free(document->query_ids);
    free(document);

    // Assign to the pointer the tmp array.
    *p_query_ids = tmp;


    return EC_SUCCESS;
}