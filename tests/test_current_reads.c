#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "current_reads.h"

int main(void) {
    CurrentReads reads;
    current_reads_init(&reads);

    // Empty list
    assert(reads.head == NULL);
    assert(reads.tail == NULL);
    assert(current_reads_find(&reads, 1) == NULL);

    // Insert first novel
    bool insert_first = current_reads_insert(
        &reads, "Er Gen", "A Will Eternal", 1, 1321, 0
    );

    assert(insert_first == true);

    Node *first = reads.head;

    assert(reads.head == reads.tail);
    assert(first->next == NULL);
    assert(first->prev == NULL);
    assert(first->novel.id == 1);
    assert(first->novel.status == NOT_STARTED);

    // Insert second novel
    bool insert_second = current_reads_insert(
        &reads, "Er Gen", "Renegade Immortal", 2, 2094, 100
    );

    assert(insert_second == true);

    Node *second = reads.tail;

    assert(reads.head == first);
    assert(reads.tail == second);
    assert(first->next == second);
    assert(second->prev == first);
    assert(second->next == NULL);
    assert(second->novel.status == READING);

    // Insert third novel
    bool insert_third = current_reads_insert(
        &reads, "Er Gen", "Pursuit of the Truth", 3, 1000, 1000
    );

    assert(insert_third == true);

    Node *third = reads.tail;

    assert(second->next == third);
    assert(third->prev == second);
    assert(third->next == NULL);
    assert(third->novel.status == COMPLETED);

    // Forward traversal
    assert(reads.head->novel.id == 1);
    assert(reads.head->next->novel.id == 2);
    assert(reads.head->next->next->novel.id == 3);

    // Backward traversal
    assert(reads.tail->novel.id == 3);
    assert(reads.tail->prev->novel.id == 2);
    assert(reads.tail->prev->prev->novel.id == 1);

    // Invalid chapter counts
    bool invalid_chapters_above = current_reads_insert(
        &reads, "Test", "Invalid", 4, 100, 150
    );

    assert(invalid_chapters_above == false);

    bool invalid_chapters_negative = current_reads_insert(
        &reads, "Test", "Invalid", 4, 100, -1
    );

    assert(invalid_chapters_negative == false);

    bool invalid_total_chapters = current_reads_insert(
        &reads, "Test", "Invalid", 4, 0, 0
    );

    assert(invalid_total_chapters == false);

    // Title exceeds maximum length
    char long_title[MAX_TITLE_LENGTH + 1];

    memset(long_title, 'A', MAX_TITLE_LENGTH);
    long_title[MAX_TITLE_LENGTH] = '\0';

    bool invalid_title_length = current_reads_insert(
        &reads, "Test", long_title, 4, 100, 0
    );

    assert(invalid_title_length == false);

    // Failed insertions must preserve the list
    assert(reads.head == first);
    assert(reads.tail == third);
    assert(first->next == second);
    assert(second->next == third);

    // Test existing novel
    assert(current_reads_find(&reads, 1) != NULL);
    assert(current_reads_find(&reads, 2) != NULL);
    assert(current_reads_find(&reads, 3) != NULL);

    // Test id validation
    assert(current_reads_find(&reads, -1) == NULL);
    assert(current_reads_find(&reads, 999) == NULL);
    assert(current_reads_find(&reads, 0) == NULL);

    // Test duplicate id
    Node *oldHead = reads.head;
    Node *oldTail = reads.tail;

    bool duplicate_id = current_reads_insert(
        &reads, "Er Gen", "I Shall Seal The Heavens", 2, 1620, 67
    );

    assert(duplicate_id == false);

    // Test is has not node changes
    assert(oldHead == reads.head);
    assert(oldTail == reads.tail);

    assert(first->prev == NULL);
    assert(first->next == second);
    assert(second->prev == first);
    assert(second->next == third);
    assert(third->prev == second);
    assert(third->next == NULL);

    // Test upadate progress (chapters)
    bool upadate_read_chapters = (current_reads_update_progress(
        &reads, 2, 105
    ));

    assert(upadate_read_chapters == true);
    assert(second->novel.read_chapters == 105);

    (current_reads_update_progress(&reads, 2, 0));
    assert(second->novel.status == NOT_STARTED);

    (current_reads_update_progress(&reads, 2, 1500));
    assert(second->novel.status == READING);

    (current_reads_update_progress(&reads, 2, 2094));
    assert(second->novel.status == COMPLETED);

    // Original informations
    i32 oldReadChapters_second = second->novel.read_chapters;
    ReadingStatus oldStatus_second = second->novel.status;

    i32 oldReadChapters_first = first->novel.read_chapters;
    ReadingStatus oldStatus_first = first->novel.status;

    i32 oldReadChapters_third = third->novel.read_chapters;
    ReadingStatus oldStatus_third = third->novel.status;

    // Test upadate progress (validation)
    bool upadate_non_existent_id = (current_reads_update_progress(
        &reads, 999, 67
    ));

    assert(upadate_non_existent_id == false);

    assert(reads.head != NULL); 

    bool negative_progress = (current_reads_update_progress(
        &reads, 2, -1
    ));

    assert(negative_progress == false);

    bool overflow_max_chapters = (current_reads_update_progress(
        &reads, 2, 2100
    ));
    
    assert(overflow_max_chapters == false);
    
    // Data integrity
    assert(oldStatus_second == second->novel.status);
    assert(oldReadChapters_second == second->novel.read_chapters);

    assert(oldStatus_first == first->novel.status);
    assert(oldReadChapters_first == first->novel.read_chapters);

    assert(oldStatus_third == third->novel.status);
    assert(oldReadChapters_third == third->novel.read_chapters);

    // Trasition states
    (current_reads_update_progress(&reads, 3, 0));
    assert(third->novel.status == NOT_STARTED);

    // Init a new list
    CurrentReads Reads;
    current_reads_init(&Reads);

    // Removing a novel (empty list)
    bool empty_list_removed = current_reads_remove(&Reads, 1);
    assert(empty_list_removed == false);
    assert(Reads.head == NULL);
    assert(Reads.tail == NULL);

    // Insert first novel for the new list
    bool insert_martial_peak = current_reads_insert(
        &Reads, "Momo", "Martial Peak", 1, 6009, 0
    );

    assert(insert_martial_peak == true);

    Node *firstNode = Reads.head;

    assert(Reads.head == Reads.tail);
    assert(firstNode->next == NULL);
    assert(firstNode->prev == NULL);
    assert(firstNode->novel.id == 1);
    assert(firstNode->novel.status == NOT_STARTED);
    assert(firstNode->novel.total_chapters == 6009);   

    // Removing a non-existence id
    bool non_existence_id = current_reads_remove(&Reads, 19);
    assert(non_existence_id == false);
    
    // Test if has no changes after the fail
    assert(Reads.head == Reads.tail);
    assert(firstNode->next == NULL);
    assert(firstNode->prev == NULL);
    assert(firstNode->novel.id == 1);
    assert(firstNode->novel.status == NOT_STARTED);
    assert(firstNode->novel.total_chapters == 6009);

    // Removing invalid id (-1 || 0)
    bool remove_id_zero = current_reads_remove(&Reads, 0);
    assert(remove_id_zero == false);

    bool remove_id_negative = current_reads_remove(&Reads, -1);
    assert(remove_id_negative == false);

    // Removing the only novel existence (new list)
    bool NovelRemoved = current_reads_remove(&Reads, 1);
    assert(NovelRemoved == true);
    assert(Reads.head == NULL);
    assert(Reads.tail == NULL);

    // Removing the head (old list)
    current_reads_remove(&reads, 1);
    assert(reads.head->novel.id == 2);
    assert(reads.head->novel.status == oldStatus_second);
    assert(reads.head->novel.read_chapters == oldReadChapters_second);
    assert(reads.head->prev == NULL);

    // Insert a new novel tail (old list)
    bool insert_against_the_gods = current_reads_insert(
        &reads, "Mars Gravity", "Against the Gods", 4, 2205, 2205
    );

    assert(insert_against_the_gods == true);
    
    Node *four = reads.tail;

    assert(reads.head == second);
    assert(reads.tail == four);
    assert(second->next == third);
    assert(second->prev == NULL);
    assert(four->next == NULL);
    assert(four->novel.status == COMPLETED);

    // Remove the previous new novel inserted
    current_reads_remove(&reads, 4);
    assert(reads.head == second);
    assert(reads.tail == third);
    assert(second->next == third);
    assert(second->prev == NULL);
    assert(third->prev == second);
    assert(third->next == NULL);
    assert(third->novel.status == NOT_STARTED);

    // Insert a new novel then remove the mid (old list)
    bool insert_martial_world = current_reads_insert(
        &reads, "Cocooned Cow", "Martial World", 5, 2255, 0
    );

    assert(insert_martial_world == true);
    
    Node *five = reads.tail;

    assert(reads.head == second);
    assert(reads.tail == five);
    assert(second->next == third);
    assert(second->prev == NULL);
    assert(third->next == five);
    assert(five->prev == third);
    assert(five->next == NULL);
    assert(five->novel.status == NOT_STARTED);

    bool remove_mid = current_reads_remove(&reads, 3);
    assert(remove_mid == true);
    assert(reads.head == second);
    assert(reads.tail == five);
    assert(second->next == five);
    assert(second->prev == NULL);
    assert(five->next == NULL);
    assert(five->prev == second);

    // Duplicate remove
    bool first_remove_true = current_reads_remove(&reads, 5);
    assert(first_remove_true == true);
    bool second_remove_false = current_reads_remove(&reads, 5);
    assert(second_remove_false == false);

    // Check the integrity
    assert(reads.head == reads.tail);
    assert(reads.head->prev == NULL);
    assert(reads.tail->next == NULL);

    // Cleanup memmory allocation (first list)
    current_reads_free(&reads);

    // Try free empty init list 
    current_reads_free(&reads);
    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Insert and free the only one novel
    current_reads_insert(&reads, 
        "TurtleMe", "The Beginning After the End", 
        1, 529, 0
    );

    Node *Newfirst = reads.head;
        
    assert(reads.head == reads.tail);
    assert(Newfirst->next == NULL);
    assert(Newfirst->prev == NULL);
    assert(Newfirst->novel.id == 1);
    assert(Newfirst->novel.total_chapters == 529);
    assert(Newfirst->novel.status == NOT_STARTED);

    current_reads_free(&reads);
    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Insert three novel and free the list
    current_reads_insert(&reads, 
        "TurtleMe", "The Beginning After the End", 
        1, 529, 0
    );
    
    Node *firstNew = reads.head;
    
    assert(reads.head == reads.tail);
    assert(firstNew->next == NULL);
    assert(firstNew->prev == NULL);
    assert(firstNew->novel.id == 1);
    assert(firstNew->novel.total_chapters == 529);
    assert(firstNew->novel.status == NOT_STARTED);

    // second insert novel
    current_reads_insert(&reads, 
        "I Eat Tomatoes", "Coiling Dragon", 
        2, 807, 100
    );
    
    Node *secondNew = reads.tail;
    
    assert(reads.head == firstNew);
    assert(reads.tail == secondNew);
    assert(secondNew->next == NULL);
    assert(secondNew->prev == firstNew);
    assert(firstNew->next == secondNew);
    assert(secondNew->novel.id == 2);
    assert(secondNew->novel.total_chapters == 807);
    assert(secondNew->novel.status == READING);

    // third insert novel
    current_reads_insert(&reads, 
        "Cuttlefish That Loves Diving", 
        "Lord of the Mysteries",            
        3, 1432, 1432
    );
    
    Node *thirdNew = reads.tail;
    
    assert(reads.head == firstNew);
    assert(reads.tail == thirdNew);
    assert(thirdNew->next == NULL);
    assert(thirdNew->prev == secondNew);
    assert(secondNew->next == thirdNew);
    assert(secondNew->prev == firstNew);
    assert(firstNew->next == secondNew);
    assert(firstNew->prev == NULL);
    assert(thirdNew->novel.id == 3);
    assert(thirdNew->novel.total_chapters == 1432);
    assert(thirdNew->novel.status == COMPLETED);

    // Forward traversal
    assert(reads.head->novel.id == 1);           
    assert(reads.head->next->novel.id == 2);
    assert(reads.head->next->next->novel.id == 3);

    // Backward traversal
    assert(reads.tail->novel.id == 3);                
    assert(reads.tail->prev->novel.id == 2);
    assert(reads.tail->prev->prev->novel.id == 1);

    // Free current list
    current_reads_free(&reads);                 
    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Try free the list again 
    current_reads_free(&reads);
    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    // Use the same list to insert after free
    current_reads_insert(&reads, 
        "Er Gen", "Beyond the Timescape", 
        1, 1445, 1000
    );

    Node *newNovel = reads.head;
        
    assert(reads.head == reads.tail);
    assert(newNovel->next == NULL);
    assert(newNovel->prev == NULL);
    assert(newNovel->novel.id == 1);
    assert(newNovel->novel.total_chapters == 1445);
    assert(newNovel->novel.status == READING);

    // Free the new inserted novel
    current_reads_free(&reads);
    assert(reads.head == NULL);
    assert(reads.tail == NULL);

    printf("All current_reads tests passed!\n");

    return 0;
}