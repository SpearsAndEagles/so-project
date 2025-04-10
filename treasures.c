#include "treasures.h"

void ask_for_treasure_info(Treasure *treasure)
{
    printf("Enter username: ");
    scanf("%s", treasure->username);
    printf("Enter latitude: ");
    scanf("%f", &treasure->latitude);
    printf("Enter longitude: ");
    scanf("%f", &treasure->longitude);
    printf("Enter clue: ");
    scanf("%s", treasure->clue);
    printf("Enter value: ");
    scanf("%d", &treasure->value);
}

Treasure *create_treasure()
{
    Treasure *treasure = malloc(sizeof(Treasure));
    ask_for_treasure_info(treasure);
    if (!treasure)
        return 0;
    return treasure;
}

int add_treasure(const char *hunt_id, Treasure *treasure)
{
    return 0;
}
int view_treasure(const char *hunt_id, int treasure_id)
{
    return 0;
}
int remove_treasure(const char *hunt_id, int treasure_id)
{
    return 0;
}