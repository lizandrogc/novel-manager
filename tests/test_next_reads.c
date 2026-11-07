#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "next_reads.h"


int main(void) {

    // Init queue
    NextReads reads;
    next_reads_init(&reads);

    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Find empty queue
    NextNode *nodeFind = next_reads_find(&reads, 1);

    assert(nodeFind == NULL);
    assert(next_reads_find(&reads, 999) == NULL);

    // Insert first novel
    bool first_insert = next_reads_insert(&reads,
        "Er Gen", "A Will Eternal", 1, 1319
    );

    assert(first_insert == true);

    NextNode *firstNode = reads.head;

    assert(reads.head != NULL);
    assert(reads.head == reads.tail);
    assert(firstNode == reads.head);
    assert(firstNode->next == NULL);

    assert(firstNode->novel.id == 1);
    assert(firstNode->novel.total_chapters == 1319);
    assert(firstNode->novel.read_chapters == 0);
    assert(firstNode->novel.status == NOT_STARTED);

    assert(strcmp(firstNode->novel.author, "Er Gen") == 0);
    assert(strcmp(firstNode->novel.title, "A Will Eternal") == 0);

    // Find the first novel
    NextNode *firstNodeFind = next_reads_find(&reads, 1);

    assert(firstNodeFind != NULL);
    assert(firstNodeFind == firstNode);

    // Insert second novel
    bool second_insert = next_reads_insert(&reads,
        "Er Gen", "Renegade Immortal", 2, 2094
    );

    assert(second_insert == true);

    NextNode *secondNode = firstNode->next;

    assert(reads.head == firstNode);
    assert(reads.tail == secondNode);
    assert(firstNode->next == secondNode);
    assert(secondNode->next == NULL);

    assert(secondNode->novel.id == 2);
    assert(secondNode->novel.total_chapters == 2094);
    assert(secondNode->novel.read_chapters == 0);
    assert(secondNode->novel.status == NOT_STARTED);

    // Insert third novel
    bool third_insert = next_reads_insert(&reads,
        "Cuttlefish That Loves Diving",
        "Lord of the Mysteries", 3, 1432
    );

    assert(third_insert == true);

    NextNode *thirdNode = secondNode->next;

    assert(reads.head == firstNode);
    assert(reads.tail == thirdNode);

    assert(firstNode->next == secondNode);
    assert(secondNode->next == thirdNode);
    assert(thirdNode->next == NULL);

    assert(thirdNode->novel.id == 3);
    assert(thirdNode->novel.total_chapters == 1432);
    assert(thirdNode->novel.read_chapters == 0);
    assert(thirdNode->novel.status == NOT_STARTED);

    // Find existing novels
    NextNode *find_id_one = next_reads_find(&reads, 1);
    NextNode *find_id_two = next_reads_find(&reads, 2);
    NextNode *find_id_three = next_reads_find(&reads, 3);

    assert(find_id_one == firstNode);
    assert(find_id_two == secondNode);
    assert(find_id_three == thirdNode);

    // Test invalid id input
    NextNode *find_non_existent = next_reads_find(&reads, 999);
    NextNode *find_negative = next_reads_find(&reads, -1);
    NextNode *find_zero = next_reads_find(&reads, 0);

    assert(find_non_existent == NULL);
    assert(find_negative == NULL);
    assert(find_zero == NULL);

    // Forward traversal
    assert(reads.head->novel.id == 1);
    assert(reads.head->next->novel.id == 2);
    assert(reads.head->next->next->novel.id == 3);
    assert(reads.head->next->next->next == NULL);

    // Insertion validation
    bool first_invalid_insertion = next_reads_insert(&reads,
        "Test", "Invalid Novel", 0, 100
    );

    assert(first_invalid_insertion == false);

    bool second_invalid_insertion = next_reads_insert(&reads,
        "Test", "Invalid Novel", -1, 100
    );

    assert(second_invalid_insertion == false);

    bool third_invalid_insertion = next_reads_insert(&reads,
        "Test", "Invalid Novel", 4, 0
    );

    assert(third_invalid_insertion == false);

    bool fourth_invalid_insertion = next_reads_insert(&reads,
        "Test", "Invalid Novel", 4, -1
    );

    assert(fourth_invalid_insertion == false);

    // Test duplicate id
    bool fifth_invalid_insertion = next_reads_insert(&reads,
        "Er Gen", "Pursuit of the Truth", 2, 1000
    );

    assert(fifth_invalid_insertion == false);

    // Test strings length
    char big_title[MAX_TITLE_LENGTH + 1];

    memset(big_title, 'L', MAX_TITLE_LENGTH);
    big_title[MAX_TITLE_LENGTH] = '\0';

    bool sixth_invalid_insertion = next_reads_insert(&reads,
        "Test", big_title, 4, 100
    );

    assert(sixth_invalid_insertion == false);

    char big_author[MAX_AUTHOR_LENGTH + 1];

    memset(big_author, 'A', MAX_AUTHOR_LENGTH);
    big_author[MAX_AUTHOR_LENGTH] = '\0';

    bool seventh_invalid_insertion = next_reads_insert(&reads,
        big_author, "Test", 4, 100
    );

    assert(seventh_invalid_insertion == false);

    // Check integrity after fail insertions
    assert(reads.head == firstNode);
    assert(reads.tail == thirdNode);

    assert(firstNode->next == secondNode);
    assert(secondNode->next == thirdNode);
    assert(thirdNode->next == NULL);

    assert(firstNode->novel.id == 1);
    assert(secondNode->novel.id == 2);
    assert(thirdNode->novel.id == 3);

    assert(firstNode->novel.read_chapters == 0);
    assert(secondNode->novel.read_chapters == 0);
    assert(thirdNode->novel.read_chapters == 0);

    assert(firstNode->novel.status == NOT_STARTED);
    assert(secondNode->novel.status == NOT_STARTED);
    assert(thirdNode->novel.status == NOT_STARTED);

    assert(next_reads_find(&reads, 4) == NULL);

    // Temporary cleanup while next_reads_free is not implemented
    free(firstNode);
    free(secondNode);
    free(thirdNode);

    printf("All next_reads tests passed!\n");

    return 0;
}