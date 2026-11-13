#ifndef STACK_UNDO_H
#define STACK_UNDO_H

#include <stdbool.h>
#include "int_types.h"

typedef struct UndoNode {
    i32 id;
    i32 prev_read_chapters;
    struct UndoNode *next; 
} UndoNode;

typedef struct UndoStack {
    UndoNode *top;
} UndoStack;


void undo_stack_init(UndoStack *stack);

bool undo_stack_push(UndoStack *stack, 
    i32 id, i32 prev_read_chapters);

const UndoNode *undo_stack_peek(const UndoStack *stack);

bool undo_stack_pop(UndoStack *stack);

void undo_stack_free(UndoStack *stack);


#endif