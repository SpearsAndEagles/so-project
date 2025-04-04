#ifndef HUNTS_H
#define HUNTS_H

#define TREASURE_FILE "treasures.dat"

int create_hunt(const char *hunt_id);
int remove_hunt(const char *hunt_id);
int list_treasures(const char *hunt_id);

#endif
