#pragma once

typedef char* String;

typedef void* Pointer;

// ####################################################################

typedef int (*CompareFunc)(Pointer a, Pointer b);

typedef void (*DestroyFunc)(Pointer value);

// ####################################################################

struct query {
    String words;                       //Save query as string
    unsigned short length;              //number of words of query
};

typedef struct query* Query;

