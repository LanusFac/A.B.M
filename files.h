/* files.h */

#ifndef FILES__H
#define FILES__H

#include <stdio.h>
#include "types.h"

status_t open_file(FILE **inventory_file, FILE **deletion_file, FILE **temp_file, char *argv_inventory, char *argv_deletion, char *file_temp_name);
status_t close_files (FILE *inventory_file, FILE *deletion_file, FILE *temp_file);

#endif
