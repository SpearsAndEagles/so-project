#ifndef HUNTS_H
#define HUNTS_H

int create_hunt(const char *hunt_id);
int remove_hunt(const char *hunt_id);
int list_treasures(const char *hunt_id);
int list_hunts(void);

#endif
