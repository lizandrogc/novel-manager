#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
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


    // Temporary cleanup while completed_reads_free is not implemented
    free(firstNode);

    printf("All completed_reads test passed!");

    return 0;
}