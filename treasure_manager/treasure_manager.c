#include "logging.h"
#include "hunts.h"
#include "treasures.h"
#include "../helpers.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        exit_with_error("No command provided\n", 1);
        exit(-1);
    }

    char *command = argv[1];
    char *hunt_id = argv[2];

    if (COMMAND("list_hunts"))
    {
        list_hunts();
        return 0;
    }
    else if (COMMAND("add"))
    {
        create_hunt(hunt_id);
        Treasure *treasure = malloc(sizeof(Treasure));
        short next = 1;
        while (next)
        {
            next = ask_and_create_treasure(treasure);
            if (add_treasure(hunt_id, treasure) < 0)
            {
                exit_with_error("Failed to add treasure", 1);
            }
        }
    }
    else if (COMMAND("list"))
    {
        list_treasures(hunt_id);
    }
    else if (COMMAND("view"))
    {
        if (argc < 4)
        {
            exit_with_error("No treasure_id provided", 1);
        }
        int treasure_id;
        if (safe_atoi(argv[3], &treasure_id))
        {
            exit_with_error("Invalid treasure_id provided", 1);
        }
        view_treasure(hunt_id, treasure_id);
    }
    else if (COMMAND("remove_treasure"))
    {
        if (argc < 4)
        {
            exit_with_error("No treasure_id provided", 1);
        }
        int treasure_id = atoi(argv[3]);
        if (!treasure_id)
        {
            exit_with_error("Invalid treasure_id provided", 1);
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
        exit_with_error(0, 1);
        exit(-1);
    }
}