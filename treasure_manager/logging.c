#include "logging.h"
#include "helpers.h"

int log_operation(const char *hunt_id, const char *operation, const char *details)
{
    char log_path[256];
    snprintf(log_path, sizeof(log_path), "./%s/%s", hunt_id, LOG_FILE);
    int fd = open(log_path, O_RDWR | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("open log file");
        return -1;
    }
    char log_entry[512];
    snprintf(log_entry, sizeof(log_entry), "%s: %s\n", operation, details);
    if (write(fd, log_entry, strlen(log_entry)) != strlen(log_entry))
    {
        perror("write log entry");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}
int link_log_2root(const char *hunt_id)
{
    char link_name[256], log_path[256];
    snprintf(log_path, sizeof(log_path), "./%s/%s", hunt_id, LOG_FILE);
    snprintf(link_name, sizeof(link_name), "./logged_hunt-%s", hunt_id);
    if (symlink(LOG_FILE, link_name) < 0)
    {
        perror("Failed to create symbolic link");
        return -1;
    }
    return 0;
}