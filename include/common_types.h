#pragma once

typedef char* String;

typedef void* Pointer;

// ####################################################################

struct query {
    String words;                       //Save query as string
    unsigned short length;              //number of words of query
};



typedef struct query* Query;

// ####################################################################

typedef int (*CompareFunc)(Pointer a, Pointer b);

typedef void (*DestroyFunc)(Pointer value);