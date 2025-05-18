#include "../helpers.h"
#include "../treasure_manager/treasures.h"

typedef struct
{
    char username[MAX_USERNAME];
    int score;
} UserScore;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <hunt_id>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/%s", argv[1], TREASURE_FILE);

    FILE *f = fopen(path, "rb");
    if (!f)
        return 0;

    UserScore *users = NULL;
    size_t count = 0, cap = 0;

    Treasure t;
    while (fread(&t, sizeof(t), 1, f) == 1)
    {
        size_t i;
        for (i = 0; i < count; i++)
        {
            if (strcmp(users[i].username, t.username) == 0)
            {
                users[i].score += t.value;
                break;
            }
        }
        if (i == count)
        {
            if (count == cap)
            {
                cap = cap ? cap * 2 : 4;
                UserScore *tmp = realloc(users, cap * sizeof(*users));
                if (!tmp)
                {
                    perror("realloc");
                    free(users);
                    fclose(f);
                    return 1;
                }
                users = tmp;
            }
            strncpy(users[count].username, t.username, MAX_USERNAME);
            users[count].username[MAX_USERNAME - 1] = '\0';
            users[count].score = t.value;
            count++;
        }
    }
    fclose(f);

    for (size_t i = 0; i < count; i++)
        printf("%s: %d\n", users[i].username, users[i].score);

    free(users);
    return 0;
}