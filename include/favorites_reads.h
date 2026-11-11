#ifndef FAVORITES_READS_H
#define FAVORITES_READS_H

#define MAX_FAVORITES 1000

#include <stdbool.h>
#include <stddef.h>
#include "int_types.h"

typedef struct FavoritesReads {
    i32 ids[MAX_FAVORITES];
    size_t length;
} FavoritesReads;


void favorites_reads_init(FavoritesReads *favorites);

bool favorites_reads_insert(FavoritesReads *favorites, i32 id);

i32 favorites_reads_find(const FavoritesReads *favorites, i32 id);

bool favorites_reads_remove(FavoritesReads *favorites, i32 id);

void favorites_reads_clear(FavoritesReads *favorites);


#endif