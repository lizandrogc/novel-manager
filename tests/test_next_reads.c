#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
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

    // Consult first novel
    NextNode *consultFirst = next_reads_consult(&reads);

    assert(consultFirst == firstNode);
    assert(reads.head == firstNode);
    assert(reads.tail == thirdNode);
    assert(firstNode->next == secondNode);

    // Init new queue for cancel tests
    NextReads cancelReads;
    next_reads_init(&cancelReads);

    // Consult empty queue
    NextNode *consultEmpty = next_reads_consult(&cancelReads);

    assert(consultEmpty == NULL);

    // Cancel from empty queue
    bool cancel_empty = next_reads_cancel(&cancelReads, 1);

    assert(cancel_empty == false);
    assert(cancelReads.head == NULL);
    assert(cancelReads.tail == NULL);

    // Insert four novels
    bool cancel_insert_first = next_reads_insert(
        &cancelReads, "Er Gen", "Novel A", 10, 100
    );

    bool cancel_insert_second = next_reads_insert(
        &cancelReads, "Er Gen", "Novel B", 20, 200
    );

    bool cancel_insert_third = next_reads_insert(
        &cancelReads, "Er Gen", "Novel C", 30, 300
    );

    bool cancel_insert_fourth = next_reads_insert(
        &cancelReads, "Er Gen", "Novel D", 40, 400
    );

    assert(cancel_insert_first == true);
    assert(cancel_insert_second == true);
    assert(cancel_insert_third == true);
    assert(cancel_insert_fourth == true);

    NextNode *cancelFirst = cancelReads.head;
    NextNode *cancelSecond = cancelFirst->next;
    NextNode *cancelThird = cancelSecond->next;
    NextNode *cancelFourth = cancelReads.tail;

    assert(cancelFirst->next == cancelSecond);
    assert(cancelSecond->next == cancelThird);
    assert(cancelThird->next == cancelFourth);
    assert(cancelFourth->next == NULL);

    // Cancel invalid id
    bool cancel_non_existent = next_reads_cancel(&cancelReads, 999);
    bool cancel_zero = next_reads_cancel(&cancelReads, 0);
    bool cancel_negative = next_reads_cancel(&cancelReads, -1);

    assert(cancel_non_existent == false);
    assert(cancel_zero == false);
    assert(cancel_negative == false);

    // check integrity after failed cancel
    assert(cancelReads.head == cancelFirst);
    assert(cancelReads.tail == cancelFourth);
    assert(cancelFirst->next == cancelSecond);
    assert(cancelSecond->next == cancelThird);
    assert(cancelThird->next == cancelFourth);

    // Cancel mid node
    bool cancel_middle = next_reads_cancel(&cancelReads, 20);

    assert(cancel_middle == true);
    assert(cancelReads.head == cancelFirst);
    assert(cancelReads.tail == cancelFourth);
    assert(cancelFirst->next == cancelThird);
    assert(cancelThird->next == cancelFourth);
    assert(next_reads_find(&cancelReads, 20) == NULL);

    // Cancel tail
    bool cancel_tail = next_reads_cancel(&cancelReads, 40);

    assert(cancel_tail == true);
    assert(cancelReads.head == cancelFirst);
    assert(cancelReads.tail == cancelThird);
    assert(cancelFirst->next == cancelThird);
    assert(cancelThird->next == NULL);
    assert(next_reads_find(&cancelReads, 40) == NULL);

    // Cancel head
    bool cancel_head = next_reads_cancel(&cancelReads, 10);

    assert(cancel_head == true);
    assert(cancelReads.head == cancelThird);
    assert(cancelReads.tail == cancelThird);
    assert(cancelThird->next == NULL);
    assert(next_reads_find(&cancelReads, 10) == NULL);

    // Cancel only node
    bool cancel_only = next_reads_cancel(&cancelReads, 30);

    assert(cancel_only == true);
    assert(cancelReads.head == NULL);
    assert(cancelReads.tail == NULL);

    // Try canceling the same id again
    bool cancel_again = next_reads_cancel(&cancelReads, 30);

    assert(cancel_again == false);

    // reuse empty queue
    bool insert_after_cancel = next_reads_insert(
        &cancelReads, "Er Gen", "A Will Eternal", 50, 1321
    );

    assert(insert_after_cancel == true);
    assert(cancelReads.head == cancelReads.tail);
    assert(cancelReads.head->novel.id == 50);
    assert(cancelReads.head->next == NULL);

    // Cancel last remaining novel
    bool cancel_last = next_reads_cancel(&cancelReads, 50);

    assert(cancel_last == true);
    assert(cancelReads.head == NULL);
    assert(cancelReads.tail == NULL);
    
    // Init new queue for remove tests
    NextReads removeReads;
    next_reads_init(&removeReads);

    // Remove from empty queue
    bool remove_empty = next_reads_remove(&removeReads);

    assert(remove_empty == false);
    assert(removeReads.head == NULL);
    assert(removeReads.tail == NULL);

    // Insert three novels
    bool remove_insert_first = next_reads_insert(
        &removeReads, "Er Gen", "Novel A", 10, 100
    );

    bool remove_insert_second = next_reads_insert(
        &removeReads, "Er Gen", "Novel B", 20, 200
    );

    bool remove_insert_third = next_reads_insert(
        &removeReads, "Er Gen", "Novel C", 30, 300
    );

    assert(remove_insert_first == true);
    assert(remove_insert_second == true);
    assert(remove_insert_third == true);

    NextNode *removeFirst = removeReads.head;
    NextNode *removeSecond = removeFirst->next;
    NextNode *removeThird = removeReads.tail;

    assert(removeFirst->next == removeSecond);
    assert(removeSecond->next == removeThird);
    assert(removeThird->next == NULL);

    // Remove first novel FIFO
    bool remove_first = next_reads_remove(&removeReads);

    assert(remove_first == true);
    assert(removeReads.head == removeSecond);
    assert(removeReads.tail == removeThird);
    assert(removeSecond->next == removeThird);
    assert(next_reads_find(&removeReads, 10) == NULL);

    // Insert after removal
    bool remove_insert_fourth = next_reads_insert(
        &removeReads, "Er Gen", "Novel D", 40, 400
    );

    assert(remove_insert_fourth == true);

    NextNode *removeFourth = removeReads.tail;

    assert(removeReads.head == removeSecond);
    assert(removeSecond->next == removeThird);
    assert(removeThird->next == removeFourth);
    assert(removeFourth->next == NULL);

    // Remove second novel
    bool remove_second = next_reads_remove(&removeReads);

    assert(remove_second == true);
    assert(removeReads.head == removeThird);
    assert(removeReads.tail == removeFourth);
    assert(removeThird->next == removeFourth);
    assert(next_reads_find(&removeReads, 20) == NULL);

    // Remove third novel
    bool remove_third = next_reads_remove(&removeReads);

    assert(remove_third == true);
    assert(removeReads.head == removeFourth);
    assert(removeReads.tail == removeFourth);
    assert(removeFourth->next == NULL);

    // Remove only remaining novel
    bool remove_only = next_reads_remove(&removeReads);

    assert(remove_only == true);
    assert(removeReads.head == NULL);
    assert(removeReads.tail == NULL);

    // Try removing again
    bool remove_again = next_reads_remove(&removeReads);

    assert(remove_again == false);

    // Reuse the empty queue
    bool insert_after_remove = next_reads_insert(
        &removeReads, "Er Gen", "A Will Eternal", 50, 1319
    );

    assert(insert_after_remove == true);
    assert(removeReads.head == removeReads.tail);
    assert(removeReads.head->novel.id == 50);
    assert(removeReads.head->next == NULL);

    bool remove_last = next_reads_remove(&removeReads);

    assert(remove_last == true);
    assert(removeReads.head == NULL);
    assert(removeReads.tail == NULL);

    // Free queue three novels
    next_reads_free(&reads);

    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Free empty queue
    next_reads_free(&reads);

    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Insert only novel after free
    bool free_insert_first = next_reads_insert(
        &reads, "Er Gen", "A Will Eternal", 1, 1319
    );

    assert(free_insert_first == true);
    assert(reads.head == reads.tail);
    assert(reads.head->novel.id == 1);
    assert(reads.head->next == NULL);

    // free queue with only one novel
    next_reads_free(&reads);

    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Insert three novels after free
    bool free_insert_second = next_reads_insert(
        &reads, "Er Gen", "Renegade Immortal", 2, 2094
    );

    bool free_insert_third = next_reads_insert(
        &reads, "Er Gen", "Pursuit of the Truth", 3, 1000
    );

    bool free_insert_fourth = next_reads_insert(
        &reads, "Er Gen", "A Will Eternal", 4, 1319
    );

    assert(free_insert_second == true);
    assert(free_insert_third == true);
    assert(free_insert_fourth == true);

    assert(reads.head->novel.id == 2);
    assert(reads.head->next->novel.id == 3);
    assert(reads.tail->novel.id == 4);
    assert(reads.tail->next == NULL);

    // Free queue again
    next_reads_free(&reads);

    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Reuse queue after multiple frees
    bool free_insert_again = next_reads_insert(
        &reads, "Cocooned Cow", "Martial World", 5, 2255
    );

    assert(free_insert_again == true);
    assert(reads.head == reads.tail);
    assert(reads.head->novel.id == 5);
    assert(reads.head->next == NULL);

    // Final cleanup
    next_reads_free(&reads);

    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    printf("All next_reads tests passed!\n");

    return 0;
}