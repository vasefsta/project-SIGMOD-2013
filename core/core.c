#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ADTIndex.h"
#include "misc.h"
#include "ADTMap.h"
#include "ADTBKTree.h"
#include "core.h"

struct document {
	DocID doc_id;
	unsigned int num_res;
	QueryID* query_ids;
};

typedef struct document* Document;

Index Index_Exact;
Index Index_Edit;
Index Index_Hamming;

Map Map_Queries;

List doc_list;


int compare_entries(Entry e1, Entry e2) {
    return strcmp(e1->word, e2->word);
}

int compare_doc(Document doc1, Document doc2) {
    return doc1->doc_id - doc2->doc_id;
}

int comp_spec(Special s1, Special s2){
    return (s1->query->queryID) - (s2->query->queryID);
}

int hash_entries(Entry entry) {
    return hash_string(entry->word);
}

int hash_queries(Query query) {
    return query->queryID;
}

int hash_spec(Special s){
    return s->query->queryID;
}

Map get_map_queries() {
    return Map_Queries;
}

Index get_index_exact() {
    return Index_Exact;
}

Index get_index_edit() {
    return Index_Edit;
}

Index get_index_hamming() {
    return Index_Hamming;
}

void destroy_entry_only(Entry entry) {
    list_destroy(entry->payload, NULL);
    free(entry);
}


void destroy_entry(Entry entry) {
    list_destroy(entry->payload, NULL);
    free(entry->word);
    free(entry);
}

void destroy_entry_full(Entry entry) {
    list_destroy(entry->payload, (DestroyFunc) free);
    free(entry->word);
    free(entry);
}

void destroy_query(Query query) {
    free(query->words);
    free(query);
}

int compare_ids(const void *a, const void *b) {
    return ( *(int*)a - *(int*)b );
}

int compare_queries(Query q1, Query q2) {
    return q1->queryID - q2->queryID;
}

void destroy_document(Document doc){
    free(doc->query_ids);
    free(doc);
}


ErrorCode InitializeIndex() {

    Map_Queries = map_create((CompareFunc)compare_queries, 200);
    if(Map_Queries == NULL)
        return EC_NO_AVAIL_RES;

    map_set_hash_function(Map_Queries, (HashFunc) hash_queries);

    Index_Exact = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entries, 500);
    if(Index_Exact == NULL)
        return EC_NO_AVAIL_RES;

    map_set_hash_function(index_index(Index_Exact), (HashFunc) hash_entries);

/////////////////////////////////////////////////////////////////////////////////////

    Index_Edit = create_index(MT_EDIT_DIST, (CompareFunc)compare_entries, 0);
    if(Index_Edit == NULL)
        return EC_NO_AVAIL_RES;

/////////////////////////////////////////////////////////////////////////////////////

    Index_Hamming = create_index(MT_HAMMING_DIST, (CompareFunc)compare_entries, 0);
    if(Index_Hamming == NULL)
        return EC_NO_AVAIL_RES;

    doc_list = list_create((CompareFunc)compare_doc);
    if (doc_list == NULL)
        return EC_NO_AVAIL_RES;

    return EC_SUCCESS;
}

ErrorCode DestroyIndex() {
    destroy_entry_index(Index_Exact,(DestroyFunc) destroy_entry);

    destroy_entry_index(Index_Edit,(DestroyFunc) destroy_entry);

    destroy_entry_index(Index_Hamming,(DestroyFunc) destroy_entry);

    list_destroy(doc_list, (DestroyFunc) destroy_document);

    map_destroy(Map_Queries, (DestroyFunc)destroy_query);

    return EC_SUCCESS;
}


int Insert_Query(Query query) {
    
    String *Array = Seperate_sentence(query);
    for(int i = 0; i < query->length; i++) {
        Entry newentry = create_entry(Array[i], (CompareFunc) compare_queries);
        list_insert(newentry->payload, query);

        if (query->match_type == MT_EXACT_MATCH) {
            Entry entry = map_find(index_index(Index_Exact), newentry);
            if(!entry) {
                map_insert(index_index(Index_Exact), newentry);
            } else {
                destroy_entry(newentry);
                list_insert(entry->payload, query);
            }

        } else if (query->match_type == MT_EDIT_DIST) {
            bk_insert(index_index(Index_Edit), newentry);

        } else if (query->match_type == MT_HAMMING_DIST) {
            bk_insert(index_index(Index_Hamming), newentry);
        } else {
            return EC_NO_AVAIL_RES;
        }
    }

    free(Array);
    return EC_SUCCESS;
}


ErrorCode StartQuery(QueryID query_id, const char* query_str, MatchType match_type, unsigned int match_dist) {
    int count = 0;
    String string = strdup(query_str);
    String token = strtok(string, " \t\n");

    while (token != NULL ) {                             // Count number of words
        count++;
        token = strtok(NULL, "  \t\n");   
    }

    free(string);

    Query query = malloc(sizeof(*query));
	query->queryID=query_id;
    query->words = strdup(query_str);
	query->match_type=match_type;
	query->match_dist=match_dist;
    query->length = count;

    map_insert(Map_Queries, query);

    Insert_Query(query);


    return EC_SUCCESS;
}

ErrorCode EndQuery(QueryID query_id) {
    struct query tmpquery;

    tmpquery.queryID = query_id;

    Query query = map_find(Map_Queries, &tmpquery);
    if (!query)
        return EC_NO_AVAIL_RES;

    String* words = Seperate_sentence(query);
    Index index;
    
    if (query->match_type == MT_EXACT_MATCH)
        index = Index_Exact;
    else if (query->match_type == MT_EDIT_DIST)
        index = Index_Edit;
    if (query->match_type == MT_HAMMING_DIST)
        index = Index_Hamming;
    
    for (int i = 0; i < query->length; i++) {
        struct entry tmpentry;

        tmpentry.word = words[i];
        tmpentry.payload = NULL;

        if (query->match_type == MT_EXACT_MATCH) {
            Entry entry = map_find((Map)index_index(index), &tmpentry);
            
            if (!entry){
                for (int i = 0; i < query->length; i++)
                    free(words[i]);

                free(words);
                return EC_NO_AVAIL_RES;
            } 
            
            ErrorCode errcode = list_remove(entry->payload, NULL, query); 

            if (errcode == EC_NO_AVAIL_RES){
                for (int i = 0; i < query->length; i++)
                    free(words[i]);
                
                free(words);
                return errcode;           
            }
        
        } else if (query->match_type == MT_EDIT_DIST || query->match_type == MT_HAMMING_DIST) {    
            Entry entry = NULL;
            entry = bk_find_entry((BKTree)index_index(index), words[i], entry);

            if (!entry){
                for (int i = 0; i < query->length; i++)
                    free(words[i]);

                free(words);
                return EC_NO_AVAIL_RES;
            }
            ErrorCode errcode = list_remove(entry->payload, NULL, query);

            if (errcode == EC_NO_AVAIL_RES) {
                for (int i = 0; i < query->length; i++)
                    free(words[i]);

                free(words);
                return errcode;           
            }
        }
    }

    for (int i = 0; i < query->length; i++)
        free(words[i]);

    free(words);

    return EC_SUCCESS;
}

ErrorCode MatchDocument (DocID doc_id, const char * doc_str) {
    Document document = malloc(sizeof(*document));

    document->doc_id = doc_id;
    document->num_res = 0;
    document->query_ids = NULL;

    String doc_str1 = strdup(doc_str);
    List list_words = deduplicated_words_map(doc_str1);

    Map map_result = map_create((CompareFunc)comp_spec, 1000);
    map_set_hash_function(map_result, (HashFunc) hash_spec);

    List complete_queries = list_create((CompareFunc)compare_ids);

    int max_thres = 3;

     clock_t start, end;
     double cpu_time_used;

    for (ListNode node = list_first(list_words); node != NULL; node = list_find_next(node)) {
        String doc_word = list_node_value(node);
        // start = clock();
        lookup_entry_index(Index_Exact, doc_word, max_thres, map_result, complete_queries, (CompareFunc)compare_queries); 
        // end = clock();
        // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("Time needed by hash is %lf\n", cpu_time_used);
        
        // start = clock();
        lookup_entry_index(Index_Hamming, doc_word, max_thres, map_result, complete_queries, (CompareFunc)compare_queries); 
        // end = clock();
        // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("Time needed by hamming is %lf\n", cpu_time_used);
        
        // start = clock();
        lookup_entry_index(Index_Edit, doc_word, max_thres, map_result, complete_queries, (CompareFunc)compare_queries);   
        // end = clock();
        // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        // printf("Time needed by edit is %lf\n", cpu_time_used);
    }   

    document->num_res = list_size(complete_queries);
    
    document->query_ids = malloc(sizeof(QueryID)*document->num_res);
    
    int i = 0;

    for(ListNode node = list_first(complete_queries); node != NULL; node = list_find_next(node) ){
        QueryID* queryid = list_node_value(node);
        document->query_ids[i] = *queryid;
        i++;
    }

  //  start = clock();
    qsort(document->query_ids, document->num_res, sizeof(QueryID), (compare_ids));
    // end = clock();
    // cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("Time needed by qsort is %lf\n", cpu_time_used);

    // puts("");
    // puts("");

    list_insert(doc_list, document);

    list_destroy(list_words, free);
    list_destroy(complete_queries, NULL);

    map_destroy(map_result, free);
    free(doc_str1);


    return EC_SUCCESS;
}

ErrorCode GetNextAvailRes (DocID * p_doc_id, unsigned int * p_num_res, QueryID ** p_query_ids) {
    Document document = list_remove_first(doc_list);


    if (!document)
        return EC_NO_AVAIL_RES;

    *p_doc_id = document->doc_id;
    *p_num_res = document->num_res;
    QueryID* tmp = malloc(sizeof(QueryID) * document->num_res);

    if ((*p_num_res) != 0) {

        for (int i = 0; i < (*p_num_res); i++){
            tmp[i] = document->query_ids[i];            
        }
    }


    free(document->query_ids);
    free(document);

    *p_query_ids = tmp;
    return EC_SUCCESS;
}