/* main.c Programa de Altas */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main (int argc, char *argv[])
{
	status_t status;
	FILE *inventory_file, *addition_file, *output_file;

	if ((status = validate_arguments(argc, argv)) != OK)
	{
		print_error(status);
		return status;
	}

	if ((status = open_file(&inventory_file, &addition_file, &output_file, argv[CMD_ARG_INVENTORY_FILE], 
		argv[CMD_ARG_COMPARISON_FILE], TEMPORAL_FILE_NAME)) != OK)
	{
		print_error(status);
		return status;
	}

	if ((status = process_addition(inventory_file, addition_file, output_file, DELIMITER)) != OK)
	{
		fclose(inventory_file);
		fclose(addition_file);
		fclose(output_file);
		print_error(status);
		return status;
	}

	if (remove(argv[CMD_ARG_INVENTORY_FILE]) != 0)
 		return ERROR_CAN_NOT_REMOVE_INVENTORY_FILE;

 	if (remove(argv[CMD_ARG_COMPARISON_FILE]) != 0)
 		return ERROR_CAN_NOT_REMOVE_COMPARISON_FILE;

	if(rename(TEMPORAL_FILE_NAME, OUTPUT_FINAL_NAME) != 0)
	{
		close_files(inventory_file, addition_file, output_file);
 		return ERROR_CANT_RENAME_TEMP_FILE;
	}

	if ((status = close_files(inventory_file, addition_file, output_file)) != OK)
	{
		print_error(status);
		return status;
	}

	return OK;
}

status_t validate_arguments (int argc, char *argv[])
{
	if(argv == NULL)
		return ERROR_NULL_POINTER;

	if(argc != MAX_INPUT_ARG)
		return ERROR_INPUT_ARGUMENTS;

	return OK;
}
