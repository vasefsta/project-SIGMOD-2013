#include "ADTBKTree.h"
#include "ADTLinkedList.h"

struct bknode {
    Pointer value;
    Listnode children;
};

struct bktree {
    BKNode root;
    CompareFunc compare;
    DestroyFunc destroy;
};

