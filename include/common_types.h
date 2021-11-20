/*
 *  Authors: Vasiliki Efstathiou Nikos Eftychiou
 *  File   : common_types.h     
*/

#pragma once

#include "core.h"

typedef char* String;

typedef void* Pointer;

typedef int (*CompareFunc)(Pointer a, Pointer b);

typedef void (*DestroyFunc)(Pointer value);

struct query {
    String words;                       //Save query as string
    unsigned short length;              //number of words of query
    QueryID queryID;
};

typedef struct query* Query;

