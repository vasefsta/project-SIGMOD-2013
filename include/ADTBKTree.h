#pragma once

#include "core.h"
#include "common_types.h"
#include "ADTEntryList.h"

typedef struct bktree* BKTree;

typedef struct bknode* BKNode;

BKTree bk_create(MatchType type);

ErrorCode bk_insert(BKTree bktree, Pointer value);

Pointer bk_node_value(BKNode node);

void bk_destroy(BKTree bktree, DestroyFunc destroy);

int bk_find (BKTree bktree, Pointer value, EntryList entrylist, int thresholds);

int hamming_distance(Pointer value1, Pointer value2);

int edit_distance(Pointer value1, Pointer value2);