#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>

#include "completed_reads.h"
#include "current_reads.h"
#include "favorites_reads.h"
#include "next_reads.h"


bool storage_save(
    const char *path,
    const CurrentReads *current,
    const CompletedReads *completed,
    const NextReads *next,
    const FavoritesReads *favorites
);

bool storage_load(
    const char *path,
    CurrentReads *current,
    CompletedReads *completed,
    NextReads *next,
    FavoritesReads *favorites
);

#endif