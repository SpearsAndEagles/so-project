#ifndef TREASURES_H
#define TREASURES_H

#define MAX_USERNAME 50
#define MAX_CLUE 256

typedef struct
{
    int id;
    char username[MAX_USERNAME];
    float latitude;
    float longitude;
    char clue[MAX_CLUE];
    int value;
} Treasure;

short ask_and_create_treasure(Treasure *treasure);
int add_treasure(const char *hunt_id, Treasure *treasure);
int view_treasure(const char *hunt_id, int treasure_id);
int remove_treasure(const char *hunt_id, int treasure_id);
void print_treasure(Treasure *treasure);

#endif
