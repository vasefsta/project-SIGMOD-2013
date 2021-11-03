#pragma once

#include "common_types.h"
#include "core.h"
#include "ADTEntryList.h"
#include "ADTMap.h"

typedef struct index* Index;

struct index {
    Pointer index;
    MatchType matchtype;
};

Index create_index(MatchType matchtype, CompareFunc compare,  int size);

ErrorCode build_entry_index(Index index, const EntryList entrylist);

ErrorCode lookup_entry_index(Index index, String word, int threshold, EntryList result);

ErrorCode destroy_entry_index(Index index);