/* main.h */

#ifndef MAIN__H
#define MAIN__H

#include "types.h"

#define OUTPUT_FINAL_NAME "List.txt"
#define DELIMITER '|'
#define TEMPORAL_FILE_NAME "temporal list."

#define MAX_INPUT_ARG 3
#define CMD_ARG_INVENTORY_FILE 1
#define CMD_ARG_COMPARISON_FILE 2
#define MAX_STR_DESCRIPTION 15

status_t validate_arguments (int argc, char *argv[]);
status_t print_error (status_t status);
status_t open_file(FILE **inventory_file, FILE **deletion_file, FILE **temp_file, char *argv_inventory, char *argv_deletion, char *file_temp_name);
status_t close_files (FILE *inventory_file, FILE *deletion_file, FILE *temp_file);
status_t eliminate (FILE *inventory_file, FILE *deletion_file, FILE *output_file, char delimiter);
status_t process_addition (FILE *inventory_file, FILE *addition_file, FILE *output_file, char delimiter);

#endif
