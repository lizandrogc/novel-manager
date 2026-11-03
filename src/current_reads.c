#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "int_types.h"
#include "current_reads.h"


void current_reads_init(CurrentReads *reads) {
    reads->head = NULL;
    reads->tail = NULL;
}

static bool current_reads_is_empty(CurrentReads *reads) {
    return reads->head == NULL;
}

static Node *current_reads_create( 
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters, i32 read_chapters) {

    Node *newNode = malloc(sizeof *newNode);

    if (!newNode) return NULL;

    strcpy(newNode->novel.author, author_name);
    strcpy(newNode->novel.title, title_name);
    newNode->novel.id = id;
    newNode->novel.total_chapters = total_chapters;
    newNode->novel.read_chapters = read_chapters;

    if (read_chapters <= 0) {
        newNode->novel.status = NOT_STARTED;
    }
    if (read_chapters > 0 && read_chapters < total_chapters) {
        newNode->novel.status = READING;
    }
    if (read_chapters == total_chapters) {
        newNode->novel.status = COMPLETED;
    }

    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

bool current_reads_insert(CurrentReads *reads, 
    const char author_name[], const char title_name[],
    i32 id, i32 total_chapters, i32 read_chapters) {  
    
    if(strlen(author_name) >= MAX_AUTHOR_LENGTH ||
        strlen(title_name) >= MAX_TITLE_LENGTH) {
        return false;
    }
   
    if (id <= 0) return false;

    if (current_reads_find(reads, id) != NULL) return false;

    if (read_chapters < 0 ||
        read_chapters > total_chapters || 
        total_chapters <= 0) {
        return false;
    } 
    
    Node *newNode = current_reads_create(
        author_name, title_name, id, total_chapters, read_chapters
    );

    if (!newNode) return false;

    if (current_reads_is_empty(reads)) {
        reads->head = newNode;
        reads->tail = newNode;
        return true;
    }

    reads->tail->next = newNode;
    newNode->prev = reads->tail;
    reads->tail = newNode; 
    return true;
}

Node *current_reads_find(CurrentReads *reads, i32 id) {
    if (current_reads_is_empty(reads) == true) return NULL;

    if (id <= 0) return NULL;

    Node *current = reads->head;

    while (current->novel.id != id && current->next != NULL) {
        current = current->next;
    }

    if (current->novel.id != id && current->next == NULL) return NULL;

    return current;
}

bool current_reads_update_progress(
    CurrentReads *reads, i32 id, i32 read_chapters) {

    Node *current = current_reads_find(reads, id);

    if (current == NULL) return false;
    
    if (read_chapters > current->novel.total_chapters ||
        read_chapters < 0) return false;

    current->novel.read_chapters = read_chapters;

    if (current->novel.read_chapters == 0) {
        current->novel.status = NOT_STARTED;
        return true;
    }
    if (current->novel.read_chapters > 0 &&
        current->novel.read_chapters < current->novel.total_chapters) {
        current->novel.status = READING; 
        return true;   
    }
    if (current->novel.read_chapters == current->novel.total_chapters) {
        current->novel.status = COMPLETED;
        return true;
    }

    return false;
}

bool current_reads_remove(CurrentReads *reads, i32 id) {
    Node *current = current_reads_find(reads, id);

    if (current == NULL) return false;

    if (current == reads->head && current == reads->tail) {
        free(reads->head);
        reads->head = NULL;
        reads->tail = NULL;
        return true;
    }

    if (current == reads->head) {
        Node *temp = current->next;
        free(reads->head);
        temp->prev = NULL;
        reads->head = temp;
        return true;  
    }

    if (current == reads->tail) {
        Node *temp = current->prev;
        free(reads->tail);
        temp->next = NULL;
        reads->tail = temp;
        return true;
    }

    Node *next_node = current->next;
    Node *prev_node = current->prev;

    free(current);
    prev_node->next = next_node;
    next_node->prev = prev_node;
    return true;
}