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

int safe_atoi(const char *str, int *out)
{
    char *endptr;
    long val;

    errno = 0;
    val = strtol(str, &endptr, 10);

    if (endptr == str)
    {
        fprintf(stderr, "Conversion error: no digits found in '%s'\n", str);
        return -1;
    }
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) ||
        (val > INT_MAX || val < INT_MIN))
    {
        fprintf(stderr, "Conversion error: value out of range for int: '%s'\n", str);
        return -1;
    }
    if (*endptr != '\0')
    {
        fprintf(stderr, "Conversion error: extra characters after number: '%s'\n",
                endptr);
        return -1;
    }

    *out = (int)val;
    return 0;
}
