#include "treasures.h"
#include "helpers.h"

short ask_and_create_treasure(Treasure *treasure)
{
    short next = 0;
    printf("Enter username: ");
    scanf("%50s", treasure->username);
    printf("Enter latitude: ");
    while (scanf("%f", &treasure->latitude) != 1)
    {
        printf(""
               "Invalid input. Please enter a valid latitude: \n");
    }
    printf("Enter longitude: ");
    while (scanf("%f", &treasure->longitude) != 1)
    {
        printf(""
               "Invalid input. Please enter a valid longitude: \n");
    }
    printf("Enter clue: ");
    scanf("%256s", treasure->clue);
    printf("Enter value: ");
    while (scanf("%d", &treasure->value) != 1)
    {
        printf(""
               "Invalid input. Please enter a valid value: \n");
    }
    printf("Do you want to continue? (1 = yes, 0 = no): ");
    scanf("%hd", &next);
    return next;
}

int add_treasure(const char *hunt_id, Treasure *treasure)
{
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "./%s/%s", hunt_id, TREASURE_FILE);
    printf("Adding treasure to file: %s\n", filePath);
    int fd = open(filePath, O_RDWR | O_CREAT, 0644);
    if (fd < 0)
    {
        perror("open treasure file");
        return -1;
    }

    if (lseek(fd, 0, SEEK_END) < 0)
    {
        perror("lseek");
        close(fd);
        return -1;
    }
    if (write(fd, treasure, sizeof(Treasure)) != sizeof(Treasure))
    {
        perror("write treasure record");
        close(fd);
        return -1;
    }
    close(fd);
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