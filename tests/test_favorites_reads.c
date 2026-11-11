#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

#include "favorites_reads.h"


int main(void) {
    // Init favorites
    FavoritesReads favorites;
    favorites_reads_init(&favorites);

    assert(favorites.length == 0);

    // Find empty favorites
    i32 find_empty = favorites_reads_find(&favorites, 1);

    assert(find_empty == -1);
    assert(favorites_reads_find(&favorites, 999) == -1);

    // Insert first novel
    bool first_insert = favorites_reads_insert(&favorites, 10);

    assert(first_insert == true);
    assert(favorites.length == 1);
    assert(favorites.ids[0] == 10);

    // Find first novel
    i32 find_first = favorites_reads_find(&favorites, 10);

    assert(find_first == 0);

    // Insert second novel
    bool second_insert = favorites_reads_insert(&favorites, 20);

    assert(second_insert == true);
    assert(favorites.length == 2);

    assert(favorites.ids[0] == 10);
    assert(favorites.ids[1] == 20);

    // Insert third novel
    bool third_insert = favorites_reads_insert(&favorites, 30);

    assert(third_insert == true);
    assert(favorites.length == 3);

    assert(favorites.ids[0] == 10);
    assert(favorites.ids[1] == 20);
    assert(favorites.ids[2] == 30);

    // Find existing novel
    i32 find_id_ten = favorites_reads_find(&favorites, 10);
    i32 find_id_twenty = favorites_reads_find(&favorites, 20);
    i32 find_id_thirty = favorites_reads_find(&favorites, 30);

    assert(find_id_ten == 0);
    assert(find_id_twenty == 1);
    assert(find_id_thirty == 2);

    // Find invalid ids
    i32 find_zero = favorites_reads_find(&favorites, 0);
    i32 find_negative = favorites_reads_find(&favorites, -1);
    i32 find_non_existent = favorites_reads_find(&favorites, 999);

    assert(find_zero == -1);
    assert(find_negative == -1);
    assert(find_non_existent == -1);

    // Insert invalid ids
    bool insert_zero = favorites_reads_insert(&favorites, 0);
    bool insert_negative = favorites_reads_insert(&favorites, -1);

    assert(insert_zero == false);
    assert(insert_negative == false);

    // Insert duplicate ids
    bool duplicate_first = favorites_reads_insert(&favorites, 10);
    bool duplicate_middle = favorites_reads_insert(&favorites, 20);
    bool duplicate_last = favorites_reads_insert(&favorites, 30);

    assert(duplicate_first == false);
    assert(duplicate_middle == false);
    assert(duplicate_last == false);

    // Check integrity after fail insertion
    assert(favorites.length == 3);

    assert(favorites.ids[0] == 10);
    assert(favorites.ids[1] == 20);
    assert(favorites.ids[2] == 30);

    // Insert after fail insertion
    bool fourth_insert = favorites_reads_insert(&favorites, 40);

    assert(fourth_insert == true);
    assert(favorites.length == 4);

    assert(favorites.ids[0] == 10);
    assert(favorites.ids[1] == 20);
    assert(favorites.ids[2] == 30);
    assert(favorites.ids[3] == 40);

    assert(favorites_reads_find(&favorites, 40) == 3);

    // Init new favorites for capacity test
    FavoritesReads fullFavorites;
    favorites_reads_init(&fullFavorites);

    assert(fullFavorites.length == 0);

    // Fill favorites to max capacity
    for (size_t i = 0; i < MAX_FAVORITES; i++) {
        i32 id = (i32)i + 1;

        bool inserted = favorites_reads_insert(&fullFavorites, id);

        assert(inserted == true);
        assert(fullFavorites.length == i + 1);
        assert(fullFavorites.ids[i] == id);
    }

    // Check maximum capacity
    assert(fullFavorites.length == MAX_FAVORITES);

    assert(fullFavorites.ids[0] == 1);
    assert(fullFavorites.ids[MAX_FAVORITES - 1] == MAX_FAVORITES);

    // Find first, middle and last ids
    i32 find_full_first = favorites_reads_find(&fullFavorites, 1);
    i32 find_full_middle = favorites_reads_find(&fullFavorites, 500);
    i32 find_full_last = favorites_reads_find(&fullFavorites, 1000);

    assert(find_full_first == 0);
    assert(find_full_middle == 499);
    assert(find_full_last == 999);

    // Insert into full favorites
    bool insert_full = favorites_reads_insert(&fullFavorites, 1001);

    assert(insert_full == false);
    assert(fullFavorites.length == MAX_FAVORITES);

    // Check integrity after full insertion
    for (size_t i = 0; i < MAX_FAVORITES; i++) {
        assert(fullFavorites.ids[i] == (i32)i + 1);
    }

    assert(favorites_reads_find(&fullFavorites, 1001) == -1);

    // Reinitialize full favorites
    favorites_reads_init(&fullFavorites);

    assert(fullFavorites.length == 0);
    assert(favorites_reads_find(&fullFavorites, 1) == -1);

    // Insert after reinitialization
    bool insert_after_init = favorites_reads_insert(&fullFavorites, 50);

    assert(insert_after_init == true);
    assert(fullFavorites.length == 1);
    assert(fullFavorites.ids[0] == 50);
    assert(favorites_reads_find(&fullFavorites, 50) == 0);

    printf("All favorites_reads tests passed!\n");

    return 0;
}