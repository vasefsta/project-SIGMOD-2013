#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int hash_entries(Entry entry) {
    return hash_string(entry->word);
}

int hash_queries(Query query) {
    return query->queryID;
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

void destroy_entry_only(Entry entry){
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

    Map_Queries = map_create((CompareFunc)compare_queries, 100);
    if(Map_Queries == NULL)
        return EC_NO_AVAIL_RES;

    map_set_hash_function(Map_Queries, (HashFunc) hash_queries);

    Index_Exact = create_index(MT_EXACT_MATCH, (CompareFunc)compare_entries, 100);
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
    puts("1111");
    destroy_entry_index(Index_Exact,(DestroyFunc) destroy_entry);
    puts("222");

    destroy_entry_index(Index_Edit,(DestroyFunc) destroy_entry);
    puts("3333");


    destroy_entry_index(Index_Hamming,(DestroyFunc) destroy_entry);
    puts("4444");

    list_destroy(doc_list, (DestroyFunc) destroy_document);
    puts("5555");

    map_destroy(Map_Queries, (DestroyFunc)destroy_query);
    puts("6666");

    return EC_SUCCESS;
}


int Insert_Query(Query query) {
    
    String *Array = Seperate_sentence(query);

    for(int i = 0; i < query->length; i++) {
        Entry entry = create_entry(Array[i], (CompareFunc) compare_queries);
        list_insert(entry->payload, query);

        if (query->match_type == MT_EXACT_MATCH) {
            Entry node = map_find(index_index(Index_Exact), entry);
            if(!node) {
                map_insert(index_index(Index_Exact), entry);
            } else {
                destroy_entry(entry);
                Entry entry2 = node;
                list_insert(entry2->payload, query);
            }

        } else if (query->match_type == MT_EDIT_DIST) {
            bk_insert(index_index(Index_Edit), entry);

        } else if (query->match_type == MT_HAMMING_DIST) {
            bk_insert(index_index(Index_Hamming), entry);

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
    tmpquery.length = 0;
    tmpquery.match_dist = 0;
    tmpquery.match_type = MT_EXACT_MATCH;
    tmpquery.words = NULL;

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

            if (!entry) 
                return EC_NO_AVAIL_RES;
            
            ErrorCode errcode = list_remove(entry->payload, NULL, query); 

            if (errcode == EC_NO_AVAIL_RES)
                return errcode;           
        
        } else if (query->match_type == MT_EDIT_DIST || query->match_type == MT_HAMMING_DIST) {    
            Entry entry = bk_find_entry((BKTree)index_index(index), words[i]);

            if (!entry) {
                puts("BYYYEEEEEEEEEEE");
                return EC_NO_AVAIL_RES;
            }

            printf("NUM OF QUERIES == %d\n", list_size(entry->payload));
            printf("QUERYIDDDDDDDDDDD = %d  QUERYY WOOOORD = %s\n\n", query_id, entry->word);
            for (ListNode node = list_first(entry->payload); node != NULL; node = list_find_next(node))
                {
                    Query q = list_node_value(node);

                    if (q)
                        printf("query id = %d\t", q->queryID);
                    else    
                        puts("NOTHING");
                }

            puts(" ");
            ErrorCode errcode = list_remove(entry->payload, NULL, query);

            if (errcode == EC_NO_AVAIL_RES) {
                puts("RRRRRRRRRRRRRRRRRR");
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

    EntryList result1 = create_entry_list((CompareFunc)compare_entries);
    EntryList result2 = create_entry_list((CompareFunc)compare_entries);
    EntryList result3 = create_entry_list((CompareFunc)compare_entries);

    int max_thres = 3;

    for (ListNode node = list_first(list_words); node != NULL; node = list_find_next(node)) {
        String doc_word = list_node_value(node);
        lookup_entry_index(Index_Exact, doc_word, max_thres, result1, (CompareFunc)compare_queries); 
        lookup_entry_index(Index_Hamming, doc_word, max_thres, result2, (CompareFunc)compare_queries); 
        lookup_entry_index(Index_Edit, doc_word, max_thres, result3, (CompareFunc)compare_queries);   
    }

    List complete_queries1 = find_complete_queries(result1, (CompareFunc) compare_queries);
    List complete_queries2 = find_complete_queries(result2, (CompareFunc) compare_queries);
    List complete_queries3 = find_complete_queries(result3, (CompareFunc) compare_queries);


    document->num_res = list_size(complete_queries1) + list_size(complete_queries2) + list_size(complete_queries3);

    QueryID *complete_ids = malloc(sizeof(QueryID)*document->num_res);

    int i = 0;

    for(ListNode node = list_first(complete_queries1); node != NULL; node = list_find_next(node) ){
        int *ID = list_node_value(node);
        complete_ids[i] = *ID;
        i++;
    }

    for(ListNode node = list_first(complete_queries2); node != NULL; node = list_find_next(node) ){
        int *ID = list_node_value(node);
        complete_ids[i] = *ID;
        i++;
    }

    for(ListNode node = list_first(complete_queries3); node != NULL; node = list_find_next(node) ){
        int *ID = list_node_value(node);
        complete_ids[i] = *ID;
        i++;
    }

    document->query_ids = complete_ids;

    list_insert(doc_list, document);


    destroy_entry_list(result1, (DestroyFunc) destroy_entry_only);
    destroy_entry_list(result2, (DestroyFunc) destroy_entry_only);
    destroy_entry_list(result3, (DestroyFunc) destroy_entry_only);

    list_destroy(list_words, free);
    list_destroy(complete_queries1, NULL);
    list_destroy(complete_queries2, NULL);
    list_destroy(complete_queries3, NULL);

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

    qsort(tmp, *p_num_res, sizeof(QueryID), (compare_ids));

    free(document->query_ids);
    free(document);

    *p_query_ids = tmp;
    return EC_SUCCESS;
}