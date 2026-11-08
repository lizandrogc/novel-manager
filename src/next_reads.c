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

NextNode *next_reads_consult(NextReads *reads) {
    if (next_reads_empty(reads) == true) return NULL;

    return reads->head;
}

bool next_reads_cancel(NextReads *reads, i32 id) {
    NextNode *current = next_reads_find(reads, id);

    if (!current) return false;

    if (current == reads->head && current == reads->tail) {
        free(reads->head);
        reads->head = NULL;
        reads->tail = NULL;
        return true;
    }

    if (current == reads->head) {
        NextNode *next = reads->head->next;
        free(reads->head);
        reads->head = next;
        return true;
    }

    if (current == reads->tail) {
        NextNode *prev = NULL;
        NextNode *aux = reads->head;

        while (aux->next != NULL) {
            prev = aux;
            aux = aux->next;
        }       

        free(reads->tail);
        prev->next = NULL;
        reads->tail = prev;
        return true;
    }

    NextNode *prev_node = NULL;
    NextNode *temp = reads->head;

    while (temp != current) {
        prev_node = temp;
        temp = temp->next;
    }

    NextNode *next_node = temp->next;
    free(temp);
    prev_node->next = next_node;
    return true;
}

bool next_reads_remove(NextReads *reads) {
    if (next_reads_empty(reads) == true) return false;

    if (reads->head == reads->tail) {
        free(reads->head);
        reads->head = NULL;
        reads->tail = NULL;
        return true;
    }

    NextNode *nextNode = reads->head->next;
    free(reads->head);
    reads->head = nextNode;
    return true;
}

void next_reads_free(NextReads *reads) {
    if (next_reads_empty(reads) == true) return;

    NextNode *current = reads->head;

    while (current != NULL) {
        NextNode *temp = current->next;
        free(current);
        current = temp;
    }

    reads->head = NULL;
    reads->tail = NULL;
}