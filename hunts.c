#include "hunts.h"
#include "helpers.h"
#include <ftw.h>

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
    return 0;
}

static int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int ret = remove(fpath);
    if (ret)
        perror(fpath);
    return ret;
}

static int remove_directory_recursive(const char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

int remove_hunt(const char *hunt_id)
{
    if (remove_directory_recursive(hunt_id) < 0)
    {
        fprintf(stderr, "Failed to remove hunt %s\n", hunt_id);
        return -1;
    }
    char link_name[256];
    snprintf(link_name, sizeof(link_name), "logged_hunt-%s", hunt_id);
    unlink(link_name);
    return 0;
}

int list_treasures(const char *hunt_id)
{
    return 0;
}