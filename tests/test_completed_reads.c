#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "completed_reads.h"


int main(void) {
    // Init list
    CompletedReads reads;
    completed_reads_init(&reads);

    assert(reads.head == NULL);
    CompletedNode *nodeFind = completed_reads_find(&reads, 1);
    assert(nodeFind == NULL);
    nodeFind = completed_reads_find(&reads, 999);
    assert(nodeFind == NULL);

    // Insert first novel
    bool completed_first_insert = completed_reads_insert(&reads,
        "Er Gen", "I Shall Seal the Heavens", 
        1, 1620, 1620
    );

    CompletedNode *firstNode = reads.head;
    
    assert(completed_first_insert == true);
    assert(reads.head != NULL);
    assert(firstNode == reads.head);
    assert(reads.head->next == NULL);
    assert(firstNode->novel.id == 1);
    assert(firstNode->novel.total_chapters == 1620);
    assert(firstNode->novel.read_chapters == 1620);
    assert(firstNode->novel.status == COMPLETED);

    // Test if find function return the head
    CompletedNode *firstNodeFind = completed_reads_find(&reads, 1);
    assert(firstNodeFind == reads.head);

    // Insert second novel
    bool completed_second_insert = completed_reads_insert(&reads,
        "Er Gen", "A Will Eternal", 
        2, 1319, 1319
    );

    CompletedNode *secondNode = reads.head->next;

    assert(completed_second_insert == true);
    assert(reads.head == firstNode);
    assert(firstNode->next == secondNode);
    assert(secondNode->next == NULL);
    assert(secondNode->novel.id == 2);
    assert(secondNode->novel.total_chapters == 1319);
    assert(secondNode->novel.read_chapters == 1319);
    assert(secondNode->novel.status == COMPLETED);

    // Insert third novel
    bool completed_third_insert = completed_reads_insert(&reads,
        "Er Gen", "Immortal Renegade", 
        3, 2094, 2094
    );

    CompletedNode *thirdNode = secondNode->next;

    assert(completed_third_insert == true);
    assert(reads.head == firstNode);
    assert(firstNode->next == secondNode);
    assert(secondNode->next == thirdNode);
    assert(thirdNode->next == NULL);
    assert(thirdNode->novel.id == 3);
    assert(thirdNode->novel.read_chapters == 2094);
    assert(thirdNode->novel.status == COMPLETED);

    // Test if id can be find
    CompletedNode *find_id_one = completed_reads_find(&reads, 1);
    assert(find_id_one != NULL);
    assert(find_id_one->novel.id == firstNode->novel.id);
    assert(find_id_one->novel.status == firstNode->novel.status);
    assert(find_id_one->novel.read_chapters == firstNode->novel.read_chapters);

    CompletedNode *find_id_two = completed_reads_find(&reads, 2);
    assert(find_id_two != NULL);
    assert(find_id_two->novel.id == secondNode->novel.id);
    assert(find_id_two->novel.status == secondNode->novel.status);
    assert(find_id_two->novel.read_chapters == secondNode->novel.read_chapters);

    CompletedNode *find_id_tree = completed_reads_find(&reads, 3);
    assert(find_id_tree != NULL);
    assert(find_id_tree->novel.id == thirdNode->novel.id);
    assert(find_id_tree->novel.status == thirdNode->novel.status);
    assert(find_id_tree->novel.read_chapters == thirdNode->novel.read_chapters);

    // Test invalid id input
    CompletedNode *find_id_non_existence = completed_reads_find(&reads, 999);
    assert(find_id_non_existence == NULL);

    CompletedNode *find_id_negative = completed_reads_find(&reads, -1);
    assert(find_id_negative == NULL);

    CompletedNode *find_id_zero = completed_reads_find(&reads, 0);
    assert(find_id_zero == NULL);

    // Check data integrity after invalid acess
    assert(firstNode->novel.id == 1);
    assert(secondNode->novel.id == 2);
    assert(thirdNode->novel.id == 3);

    assert(firstNode->novel.read_chapters == 1620);
    assert(secondNode->novel.read_chapters == 1319);
    assert(thirdNode->novel.read_chapters == 2094);

    assert(firstNode->novel.status == COMPLETED);
    assert(secondNode->novel.status == COMPLETED);
    assert(thirdNode->novel.status == COMPLETED);

    // Forward transversal
    assert(reads.head->novel.id == 1);
    assert(reads.head->next->novel.id == 2);
    assert(reads.head->next->next->novel.id == 3);
    assert(reads.head->next->next->next == NULL);

    // Insertion validation
    bool first_invalid_insertion = completed_reads_insert(&reads, 
        "Test", "The Peak Of Life", 4, 999, 998
    );
    assert(first_invalid_insertion == false);

    bool second_invalid_insertion = completed_reads_insert(&reads, 
        "Test", "The Peak Of Life", 4, 1000, 1001
    );
    assert(second_invalid_insertion == false);

    bool third_invalid_insertion = completed_reads_insert(&reads, 
        "Test", "The Peak Of Life", 4, 0, 0
    );
    assert(third_invalid_insertion == false);

    bool four_invalid_insertion = completed_reads_insert(&reads, 
        "Test", "The Peak Of Life", 4, -1, -1
    );
    assert(four_invalid_insertion == false);

    bool five_invalid_insertion = completed_reads_insert(&reads, 
        "Test", "The Peak Of Life", 0, 150, 150
    );
    assert(five_invalid_insertion == false);

    bool six_invalid_insertion = completed_reads_insert(&reads, 
        "Test", "The Peak Of Life", -1, 300, 300
    );
    assert(six_invalid_insertion == false);

    bool seven_invalid_insertion = completed_reads_insert(&reads, 
        "Test", "The Peak Of Life", 1, 767, 767
    );
    assert(seven_invalid_insertion == false);
    
    // Test strings lenght
    char big_title[MAX_TITLE_LENGTH + 1];
    memset(big_title, 'L', MAX_TITLE_LENGTH);
    big_title[MAX_TITLE_LENGTH] = '\0';

    bool eight_invalid_insertion = completed_reads_insert(&reads, 
        "Test", big_title, 5, 100, 100
    );
    assert(eight_invalid_insertion == false);

    char big_author[MAX_AUTHOR_LENGTH + 1];
    memset(big_author, 'A', MAX_AUTHOR_LENGTH);
    big_author[MAX_AUTHOR_LENGTH] = '\0';

    bool nine_invalid_insertion = completed_reads_insert(&reads, 
        big_author, "Test", 6, 199, 199
    );
    assert(nine_invalid_insertion == false);

    bool ten_invalid_insertion = completed_reads_insert(&reads, 
        big_author, big_title , -1, 100, 200
    );
    assert(ten_invalid_insertion == false);

    // Check integrity after fail 
    assert(reads.head == firstNode);
    
    assert(firstNode->novel.id == 1);
    assert(secondNode->novel.id == 2);
    assert(thirdNode->novel.id == 3);

    assert(firstNode->novel.read_chapters == 1620);
    assert(secondNode->novel.read_chapters == 1319);
    assert(thirdNode->novel.read_chapters == 2094);

    assert(firstNode->novel.status == COMPLETED);
    assert(secondNode->novel.status == COMPLETED);
    assert(thirdNode->novel.status == COMPLETED);

    assert(reads.head->novel.id == 1);
    assert(reads.head->next->novel.id == 2);
    assert(reads.head->next->next->novel.id == 3);
    assert(reads.head->next->next->next == NULL);

    assert(firstNode->next == secondNode);
    assert(secondNode->next == thirdNode);
    assert(thirdNode->next == NULL);

    // Temporary cleanup while completed_reads_free is not implemented
    free(firstNode);
    free(secondNode);
    free(thirdNode);

    printf("All completed_reads test passed!");

    return 0;
}