#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define COMMAND(a) strcmp(command, a) == 0

void exit_with_error(const char *message, char show_usage);
