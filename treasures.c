#include "treasures.h"
#include "helpers.h"

short ask_and_create_treasure(Treasure *treasure)
{
    short next = 0;
    printf("Enter username: ");
    scanf("%s", treasure->username);
    printf("Enter latitude: ");
    while (scanf("%f", &treasure->latitude) != 1)
    {
        printf(""
               "Invalid input. Please enter a valid latitude: ");
    }
    printf("Enter longitude: ");
    while (scanf("%f", &treasure->longitude) != 1)
    {
        printf(""
               "Invalid input. Please enter a valid longitude: ");
    }
    printf("Enter clue: ");
    scanf("%s", treasure->clue);
    printf("Enter value: ");
    while (scanf("%d", &treasure->value) != 1)
    {
        printf(""
               "Invalid input. Please enter a valid value: ");
}
    printf("Do you want to continue? (1 = yes, 0 = no): ");
    scanf("%d", next);
    return next;
}

short create_treasure(Treasure *treasure)
{
    short next = ask_for_treasure_info(treasure);
    if (!treasure)
        return -1;
    return next;
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