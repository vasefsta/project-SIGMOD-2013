#pragma once

#include "common_types.h"
#include "ADTLinkedList.h"

struct entry{
    String word;
    Listnode payload;
};

typedef struct entry* Entry;

typedef Listnode Entrylist;
