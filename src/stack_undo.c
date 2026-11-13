#include <stdlib.h>
#include "stack_undo.h"

void undo_stack_init(UndoStack *stack) {
    stack->top = NULL;
}

static bool undo_stack_empty(const UndoStack *stack) {
    return stack->top == NULL;
}

const UndoNode *undo_stack_peek(const UndoStack *stack) {
    if (undo_stack_empty(stack) == true) return NULL;
    
    return stack->top;
}

static UndoNode *undo_stack_create(i32 id, i32 chapters) {
    UndoNode *newNode = malloc(sizeof *newNode);

    if (!newNode) return NULL;

    newNode->id = id;
    newNode->prev_read_chapters = chapters;
    newNode->next = NULL;

    return newNode;
}

bool undo_stack_push(UndoStack *stack, i32 id, i32 chapters) {
    if (id <= 0 || chapters < 0) return false;

    UndoNode *newNode = undo_stack_create(id, chapters);

    if (!newNode) return false;

    UndoNode *current = stack->top;

    newNode->next = current;
    stack->top = newNode;
    
    return true;
}