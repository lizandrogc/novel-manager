#ifndef NOVEL_TYPES_H
#define NOVEL_TYPES_H


#include "int_types.h"

enum {
    MAX_AUTHOR_LENGTH = 100,
    MAX_TITLE_LENGTH = 100
};

typedef enum {
    NOT_STARTED,
    READING,
    COMPLETED
} ReadingStatus;

typedef struct Novel {
    const char title[MAX_TITLE_LENGTH];
    const char author[MAX_AUTHOR_LENGTH];

    i32 id;
    i32 total_chapters;
    i32 read_chapters;

    ReadingStatus status;
} Novel;

#endif