#include <stdio.h>
#include <stdbool.h> 

#include "storage.h"
#include "int_types.h"
#include "novel_types.h"


static bool storage_write_novel(
    FILE *file, const Novel *novel) {

    if (fprintf(file, "id=%d\n", novel->id) < 0) return false;
    if (fprintf(file, "title=%s\n", novel->title) < 0) return false;
    if (fprintf(file, "author=%s\n", novel->author) < 0) return false;
    if (fprintf(file, "total_chapters=%d\n", novel->total_chapters) < 0) return false;
    if (fprintf(file, "read_chapters=%d\n", novel->read_chapters) < 0) return false;
    
    const char *status_text;

    switch (novel->status) {
        case NOT_STARTED:
            status_text = "NOT_STARTED";
            break;
        
        case READING:
            status_text = "READING";
            break;

        case COMPLETED:
            status_text = "COMPLETED";
            break;

        default:
            return false;
    }

    if (fprintf(file, "status=%s\n", status_text) < 0) return false;

    if (fprintf(file, "---\n") < 0) return false;

    return true;
}