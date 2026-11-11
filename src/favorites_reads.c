#include "favorites_reads.h"


void favorites_reads_init(FavoritesReads *favorites) {
    favorites->length = 0;
}

static bool favorites_reads_full(const FavoritesReads *favorites) {
    return favorites->length == MAX_FAVORITES;
}

i32 favorites_reads_find(const FavoritesReads *favorites, i32 id) {
    if (favorites->length == 0) return -1;

    if (id <= 0) return -1;

    for (size_t i = 0; i < favorites->length; i++) {
        if (favorites->ids[i] == id) return (i32)i;
    }

    return -1;
}

bool favorites_reads_insert(FavoritesReads *favorites, i32 id) {
    if (favorites_reads_full(favorites) == true) return false;

    if (id <= 0) return false;

    if (favorites_reads_find(favorites, id) != -1) return false;

    favorites->ids[favorites->length] = id;
    favorites->length++;
    return true;
}

bool favorites_reads_remove(FavoritesReads *favorites, i32 id) {
    if (id <= 0) return false;

    i32 idx = favorites_reads_find(favorites, id);

    if (idx == -1) return false;

    for (size_t i = idx; i < favorites->length - 1; i++) {
        favorites->ids[i] = favorites->ids[i + 1];
    }
    
    favorites->length--;
    return true;
}