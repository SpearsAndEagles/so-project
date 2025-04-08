#include "hunts.h"
#include "helpers.h"

int create_hunt(const char *hunt_id)
{
    if (mkdir(hunt_id, 0755) < 0)
    {
        if (errno)
        {
            perror("Cannot create directory");
            return -1;
        }
    }
    char treasure_path[256];
    snprintf(treasure_path, sizeof(treasure_path), "./%s/%s", hunt_id, TREASURE_FILE);
    int fd = open(treasure_path, O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("Cannot create treasure file");
        return -1;
    }
    close(fd);
}
int remove_hunt(const char *hunt_id)
{
    return 0;
}
int list_treasures(const char *hunt_id)
{
    return 0;
}