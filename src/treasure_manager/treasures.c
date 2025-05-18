#include "treasures.h"
#include "../helpers.h"
#include "logging.h"

short ask_and_create_treasure(Treasure *treasure)
{
    short next = 0;
    printf("Enter username: ");
    scanf("%50s", treasure->username);
    printf("Enter latitude: ");
    while (scanf("%f", &treasure->latitude) != 1)
    {
        printf(
            "Invalid input. Please enter a valid latitude: \n");
    }
    printf("Enter longitude: ");
    while (scanf("%f", &treasure->longitude) != 1)
    {
        printf(""
               "Invalid input. Please enter a valid longitude: \n");
    }
    printf("Enter clue: ");
    discard_stdin_line();
    fgets(treasure->clue, sizeof(treasure->clue), stdin);
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

    char details[256];
    snprintf(details, sizeof(details), "Added treasure with ID %d", treasure->id);
    log_operation(hunt_id, "ADD", details);

    return 0;
}

void print_treasure(Treasure *treasure)
{
    if (!treasure)
        return;
    printf("Treasure Details:\n");
    printf("ID: %d\n", treasure->id);
    printf("User: %s\n", treasure->username);
    printf("Latitude: %.4f\n", treasure->latitude);
    printf("Longitude: %.4f\n", treasure->longitude);
    printf("Clue: %s\n", treasure->clue);
    printf("Value: %d\n", treasure->value);
}

int view_treasure(const char *hunt_id, int treasure_id)
{
    char treasure_path[256];
    snprintf(treasure_path, sizeof(treasure_path), "./%s/%s", hunt_id, TREASURE_FILE);
    int fd = open(treasure_path, O_RDONLY);
    if (fd < 0)
    {
        perror("open treasure file");
        return -1;
    }
    Treasure treasure;
    int found = 0;
    while (read(fd, &treasure, sizeof(Treasure)) == sizeof(Treasure))
    {
        if (treasure.id == treasure_id)
        {
            print_treasure(&treasure);
            found = 1;
            break;
        }
    }
    close(fd);
    if (!found)
    {
        printf("Treasure with ID %d not found\n", treasure_id);
        return -1;
    }
    return 0;
}
int remove_treasure(const char *hunt_id, int treasure_id)
{
    char treasure_path[256], temp_path[256];
    snprintf(treasure_path, sizeof(treasure_path), "./%s/%s", hunt_id, TREASURE_FILE);
    snprintf(temp_path, sizeof(temp_path), "./%s/temp.dat", hunt_id);
    int fd = open(treasure_path, O_RDONLY);
    if (fd < 0)
    {
        perror("open treasure file");
        return -1;
    }
    int temp_fd = open(temp_path, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (temp_fd < 0)
    {
        perror("open temp file");
        close(fd);
        return -1;
    }
    Treasure treasure;
    int removed = 0;
    while (read(fd, &treasure, sizeof(Treasure)) == sizeof(Treasure))
    {
        if (treasure.id == treasure_id)
        {
            removed = 1;
            continue;
        }
        if (write(temp_fd, &treasure, sizeof(Treasure)) != sizeof(Treasure))
        {
            perror("write to temp file");
            close(fd);
            close(temp_fd);
            return -1;
        }
    }
    close(fd);
    close(temp_fd);
    if (!removed)
    {
        printf("Treasure with ID %d not found\n", treasure_id);
        remove(temp_path);
        return -1;
    }
    if (rename(temp_path, treasure_path) < 0)
    {
        perror("rename temp file");
        return -1;
    }
    char details[256];
    snprintf(details, sizeof(details), "Removed treasure with ID %d", treasure_id);
    log_operation(hunt_id, "REMOVE_TREASURE", details);
    return 0;
}
