#pragma once

#include "core.h"
#include "common_types.h"

typedef struct bktree* BKTree;

typedef struct bknode* BKNode;

BKTree create_bk_tree(MatchType type);

ErrorCode insert_bknode(BKTree bktree, Pointer value);

void bk_tree_destroy(BKTree bktree, DestroyFunc destroy);

ErrorCode insert(BKNode bknode, BKNode new, CompareFunc compare);

int hamming_distance(Pointer value1, Pointer value2);

int edit_distance(Pointer value1, Pointer value2);