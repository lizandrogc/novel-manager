#include <stdlib.h>
#include <string.h>

#include "next_reads.h"


static bool next_reads_empty(NextReads *reads) {
    return reads->head == NULL;
}

void next_reads_init(NextReads *reads) {
    reads->head = NULL;
    reads->tail = NULL;
}

static NextNode *next_reads_create(
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters) {

    NextNode *newNode = malloc(sizeof *newNode);
    
    if (newNode == NULL) return NULL;
    
    strcpy(newNode->novel.author, author_name);
    strcpy(newNode->novel.title, title_name);
    newNode->novel.id = id;
    newNode->novel.total_chapters = total_chapters;
    newNode->novel.read_chapters = 0;
    newNode->novel.status = NOT_STARTED;
    newNode->next = NULL;

    return newNode;
}

bool next_reads_insert(NextReads *reads,
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters) {
    
    if (id <= 0) return false;

    if (total_chapters <= 0) return false;

    if (next_reads_find(reads, id) != NULL) return false;

    if (strlen(author_name) >= MAX_AUTHOR_LENGTH ||
        strlen(title_name) >= MAX_TITLE_LENGTH
    )   return false;

    NextNode *newNode = next_reads_create(
        author_name, title_name, id, total_chapters
    );

    if (!newNode) return false;

    if (reads->head == NULL) {
        reads->head = newNode;
        reads->tail = newNode;
        return true;
    }

    NextNode *prev = reads->tail;
    prev->next = newNode;
    reads->tail = newNode;

    return true;
}

NextNode *next_reads_find(NextReads *reads, i32 id) {
    if (next_reads_empty(reads) == true) return NULL;

    if (id <= 0) return NULL;

    NextNode *current = reads->head;

    while (current != NULL && current->novel.id != id) {
        current = current->next;
    }

    if (current == NULL || current->novel.id != id) return NULL;

    return current;
}