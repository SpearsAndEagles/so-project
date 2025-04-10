#ifndef LOGGING_H
#define LOGGING_H

#define LOG_FILE "log.txt"

int log_operation(const char *hunt_id, const char *operation, const char *details);
int link_log_2root(const char *hunt_id);

#endif
