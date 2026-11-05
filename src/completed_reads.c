#include <stdlib.h>
#include <string.h>

#include "completed_reads.h"

void completed_reads_init(CompletedReads *reads) {
    reads->head = NULL;
}

static bool completed_reads_empty(CompletedReads *reads) {
    return reads->head == NULL;
}

static CompletedNode *completed_reads_create( 
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters, i32 read_chapters) {
    
    CompletedNode *newNode = malloc(sizeof *newNode);

    if (!newNode) return NULL;

    strcpy(newNode->novel.author, author_name);
    strcpy(newNode->novel.title, title_name);
    newNode->novel.id = id;
    newNode->novel.total_chapters = total_chapters;
    newNode->novel.read_chapters = read_chapters;
    newNode->novel.status = COMPLETED;

    newNode->next = NULL;

    return newNode;
}

CompletedNode *completed_reads_find(CompletedReads *reads, i32 id) {
    if (completed_reads_empty(reads) == true) return NULL;

    if (id <= 0) return NULL;
    
    CompletedNode *current = reads->head;

    while (current != NULL && current->novel.id != id) {
        current = current->next;
    }

    if (current == NULL || current->novel.id != id) return NULL;

    return current;
}

bool completed_reads_insert(CompletedReads *reads,
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters, i32 read_chapters) {

    if (read_chapters != total_chapters || total_chapters <= 0) 
        return false;

    if (id <= 0) return false;

    if (strlen(author_name) >= MAX_AUTHOR_LENGTH ||
        strlen(title_name) >= MAX_TITLE_LENGTH
    )   return false;

    if (completed_reads_find(reads, id) != NULL) return false;

    CompletedNode *newNode = completed_reads_create(
        author_name, title_name, id, total_chapters, read_chapters
    );

    if (!newNode) return false;

    if (completed_reads_empty(reads) == true) {
        reads->head = newNode;
        return true;
    }

    CompletedNode *current = reads->head;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = newNode;

    return true;
}

bool completed_reads_remove(CompletedReads *reads, i32 id) {
    CompletedNode *current = completed_reads_find(reads, id);

    if (!current) return false;

    if (current == reads->head && current->next == NULL) {
        free(reads->head);
        reads->head = NULL;
        return true;
    }

    if (current == reads->head) {
        CompletedNode *aux = reads->head->next;
        free(reads->head);
        reads->head = aux;
        return true;
    }

    CompletedNode *temp = reads->head;
    CompletedNode *prev = NULL;

    if (current->next == NULL) {
        while (temp != current) {
            prev = temp;
            temp = temp->next;
        }

        free(current);
        prev->next = NULL;
        return true;
    }

    while (temp != current) {
        prev = temp;
        temp = temp->next;
    }

    CompletedNode *next_node = current->next;

    free(current);
    prev->next = next_node;
    return true;
}