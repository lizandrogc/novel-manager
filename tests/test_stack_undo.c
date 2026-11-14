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

    // Pop from multiple nodes
    check(undo_stack_pop(&stack) == true);

    check(stack.top == second);
    check(undo_stack_peek(&stack) == second);
    check(stack.top->id == 2);
    check(stack.top->prev_read_chapters == 50);

    // Pop leaving one node
    check(undo_stack_pop(&stack) == true);

    check(stack.top == first);
    check(stack.top->id == 1);
    check(stack.top->next == NULL);

    // Pop last node
    check(undo_stack_pop(&stack) == true);

    check(stack.top == NULL);
    check(undo_stack_peek(&stack) == NULL);

    // Pop from empty stack
    check(undo_stack_pop(&stack) == false);
    check(stack.top == NULL);

    // Free empty stack
    undo_stack_free(&stack);
    check(stack.top == NULL);

    // Free stack with one node
    check(undo_stack_push(&stack, 3, 75) == true);

    undo_stack_free(&stack);
    check(stack.top == NULL);
    check(undo_stack_peek(&stack) == NULL);

    // Free stack with multiple nodes
    check(undo_stack_push(&stack, 1, 100) == true);
    check(undo_stack_push(&stack, 2, 200) == true);
    check(undo_stack_push(&stack, 3, 300) == true);

    undo_stack_free(&stack);

    check(stack.top == NULL);
    check(undo_stack_peek(&stack) == NULL);

    // Reuse stack after free
    check(undo_stack_push(&stack, 4, 0) == true);

    check(stack.top != NULL);
    check(stack.top->id == 4);
    check(stack.top->prev_read_chapters == 0);
    check(stack.top->next == NULL);

    undo_stack_free(&stack);
    check(stack.top == NULL);

    printf("All stack_undo tests passed!\n");

    return 0;
}