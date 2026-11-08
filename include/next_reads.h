#ifndef NEXT_READS_H
#define NEXT_READS_H

#include "novel_types.h"
#include <stdbool.h>

typedef struct NextNode {
    Novel novel;
    struct NextNode *next;
} NextNode;

typedef struct NextReads {
    NextNode *head;
    NextNode *tail;
} NextReads;


void next_reads_init(NextReads *reads);

NextNode *next_reads_find(NextReads *reads, i32 id);

bool next_reads_insert(NextReads *reads,
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters);

NextNode *next_reads_consult(NextReads *reads);

bool next_reads_cancel(NextReads *reads, i32 id);

bool next_reads_remove(NextReads *reads);

void next_reads_free(NextReads *reads);

#endif