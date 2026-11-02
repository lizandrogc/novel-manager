#ifndef CURRENT_READS_H
#define CURRENT_READS_H

#include "novel_types.h"
#include <stdbool.h>

typedef struct Node {
    Novel novel;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct CurrentReads {
    Node *head;
    Node *tail;
} CurrentReads;

void current_reads_init(CurrentReads *reads);

bool current_reads_insert(CurrentReads *reads,
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters, i32 read_chapters);

bool current_reads_remove(CurrentReads *reads, i32 id);

Node *current_reads_find(CurrentReads *reads, i32 id);

bool current_reads_update_progress(CurrentReads *reads, i32 id, i32 read_chapters);

void current_reads_free(CurrentReads *reads);

#endif    