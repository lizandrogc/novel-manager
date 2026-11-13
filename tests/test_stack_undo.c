#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "stack_undo.h"

static void check(bool condition) {
    assert(condition);

    if (!condition) {
        fprintf(stderr, "Test failed!\n");
        exit(EXIT_FAILURE);
    }
}


int main(void) {
    UndoStack stack;
    undo_stack_init(&stack);

    // Empty stack
    check(stack.top == NULL);
    check(undo_stack_peek(&stack) == NULL);

    // Invalid insertion into empty stack
    check(undo_stack_push(&stack, 0, 100) == false);
    check(undo_stack_push(&stack, -1, 100) == false);
    check(undo_stack_push(&stack, 1, -1) == false);

    check(stack.top == NULL);

    // First valid insertion
    check(undo_stack_push(&stack, 1, 0) == true);

    const UndoNode *first = undo_stack_peek(&stack);

    check(first != NULL);
    check(first == stack.top);
    check(first->id == 1);
    check(first->prev_read_chapters == 0);
    check(first->next == NULL);

    // Second insertion
    check(undo_stack_push(&stack, 2, 50) == true);

    const UndoNode *second = undo_stack_peek(&stack);

    check(second != NULL);
    check(second->id == 2);
    check(second->prev_read_chapters == 50);
    check(second->next == first);
    check(first->next == NULL);

    // Same novel can have multiple history
    check(undo_stack_push(&stack, 1, 100) == true);

    const UndoNode *third = undo_stack_peek(&stack);

    check(third != NULL);
    check(third->id == 1);
    check(third->prev_read_chapters == 100);
    check(third->next == second);
    check(second->next == first);

    // Invalid insertions preserve the stack
    check(undo_stack_push(&stack, 0, 200) == false);
    check(undo_stack_push(&stack, 3, -10) == false);

    check(stack.top == third);
    check(third->next == second);
    check(second->next == first);

    // Peek not modify the stack
    const UndoNode *peeked = undo_stack_peek(&stack);

    check(peeked == third);
    check(stack.top == third);

    // Temporary cleanup 
    while (stack.top != NULL) {
        UndoNode *current = stack.top;
        stack.top = current->next;
        free(current);
    }

    check(stack.top == NULL);
    check(undo_stack_peek(&stack) == NULL);

    printf("All stack_undo tests passed!\n");

    return 0;
}