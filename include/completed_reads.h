#ifndef COMPLETED_READS_H
#define COMPLETED_READS_H

#include "novel_types.h"
#include <stdbool.h>

typedef struct CompletedNode {
    Novel novel;
    struct CompletedNode *next;
} CompletedNode;

typedef struct CompletedReads {
    CompletedNode *head;
} CompletedReads;

void completed_reads_init(CompletedReads *reads);

bool completed_reads_insert(CompletedReads *reads,
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters, i32 read_chapters
);

CompletedNode *completed_reads_find(CompletedReads *reads, i32 id);

bool completed_reads_remove(CompletedReads *reads, i32 id);

void completed_reads_free(CompletedReads *reads);

#endif      