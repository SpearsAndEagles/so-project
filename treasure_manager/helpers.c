#include "helpers.h"

void exit_with_error(const char *message, char show_usage)
{
    if (message)
    {
        printf("%s", message);
    }
    if (show_usage)
    {
        printf("Usage:\n");
        printf("  treasure_manager add <hunt_id>\n");
        printf("  treasure_manager list <hunt_id>\n");
        printf("  treasure_manager view <hunt_id> <treasure_id>\n");
        printf("  treasure_manager remove_treasure <hunt_id> <treasure_id>\n");
        printf("  treasure_manager remove_hunt <hunt_id>\n");
    }
    exit(-1);
}