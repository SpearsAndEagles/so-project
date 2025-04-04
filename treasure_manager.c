#include "logging.h"
#include "hunts.h"
#include "treasures.h"

#define COMMAND(a) strcmp(command, a) == 0

void exit_with_error(const char *message)
{
    if (message)
    {
        printf("%s", message);
    }
    printf("Usage:\n");
    printf("  treasure_manager add <hunt_id>\n");
    printf("  treasure_manager list <hunt_id>\n");
    printf("  treasure_manager view <hunt_id> <treasure_id>\n");
    printf("  treasure_manager remove_treasure <hunt_id> <treasure_id>\n");
    printf("  treasure_manager remove_hunt <hunt_id>\n");
    exit(-1);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        exit_with_error("No command provided");
        exit(-1);
    }

    char *command = argv[1];
    char *hunt_id = argv[2];

    if (COMMAND("add"))
    {
        create_hunt(hunt_id);
    }
    else if (COMMAND("list"))
    {
        list_treasures(hunt_id);
    }
    else if (COMMAND("view"))
    {
        if (argc < 4)
        {
            exit_with_error("No treasure_id provided");
        }
        int treasure_id = atoi(argv[3]);
        if (!treasure_id)
        {
            exit_with_error("Invalid treasure_id provided");
        }
        view_treasure(hunt_id, treasure_id);
    }
    else if (COMMAND("remove_treasure"))
    {
        if (argc < 4)
        {
            exit_with_error("No treasure_id provided");
        }
        int treasure_id = atoi(argv[3]);
        if (!treasure_id)
        {
            exit_with_error("Invalid treasure_id provided");
        }
        remove_treasure(hunt_id, treasure_id);
    }
    else if (COMMAND("remove_hunt"))
    {
        remove_hunt(hunt_id);
    }
    else
    {
        printf("\"%10s\" is not a treasure_manager command", argv[1]);
        exit_with_error(0);
        exit(-1);
    }
}