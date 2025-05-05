#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

#define COMMAND(a) strcmp(command, a) == 0

#define TREASURE_FILE "treasures.dat"

void exit_with_error(const char *message, char show_usage);

#endif