#pragma once

typedef char* String;

typedef void* Pointer;

// ####################################################################

struct query {
    String words;
    unsigned short length; 
};

typedef struct query* Query;

// ####################################################################

typedef int (*CompareFunc)(Pointer a, Pointer b);

typedef void (*DestroyFunc)(Pointer value);