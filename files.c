/* files.c */

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#define INIT_CHOP 20
#define CHOP_SIZE 20

status_t open_file(FILE **inventory_file, FILE **deletion_file, FILE **temp_file, char *argv_inventory, char *argv_deletion, char *file_temp_name)
{
	if (inventory_file == NULL || deletion_file == NULL || temp_file == NULL || argv_inventory == NULL || argv_deletion == NULL)
		return ERROR_NULL_POINTER;

	if ((*inventory_file = fopen(argv_inventory, "rt")) == NULL)
		return ERROR_OPEN_INVENTORY_FILE;

	if ((*deletion_file = fopen(argv_deletion, "rt")) == NULL)
	{
		fclose(*inventory_file);
		return ERROR_OPEN_COMPARISON_FILE;
	}

	if ((*temp_file = fopen(file_temp_name, "wt")) == NULL)
	{
		fclose(*inventory_file);
		fclose(*deletion_file);
		return ERROR_OPEN_OUTPUT_FILE;
	}

	return OK;
}

status_t read_text_line (FILE *fi, char **string, bool_t *eof)
{
	char *aux;
	int i;
	size_t alloc_size, used_size;

	if (eof == NULL || fi == NULL || string == NULL)
		return ERROR_NULL_POINTER;

	if ((*string = (char *)malloc(INIT_CHOP * sizeof(char))) == NULL)
		return ERROR_OUT_MEMORY;

	alloc_size = INIT_CHOP;
	used_size = 0;

	while ((i = fgetc(fi)) != EOF && i != '\n')
	{
		if (used_size == alloc_size - 1)
		{
			if ((aux = realloc(*string, (alloc_size + CHOP_SIZE) * sizeof(char))) == NULL)
			{
				free (string);
				return ERROR_OUT_MEMORY;
			}
			alloc_size += CHOP_SIZE;
			*string = aux; 
		}
		(*string)[used_size ++] = i;
	}
	(*string)[used_size] = '\0';
	*eof = ((i == EOF) ? TRUE : FALSE);

	return OK;
}

status_t close_files (FILE *inventory_file, FILE *deletion_file, FILE *temp_file)
{
	if (inventory_file == NULL || deletion_file == NULL || temp_file == NULL)
		return ERROR_NULL_POINTER;

	if (fclose(inventory_file) == EOF)
	{
		fclose(deletion_file);
		fclose(temp_file);
		return ERROR_CLOSE_FILE;
	}

	if (fclose(deletion_file) == EOF)
	{
		fclose(temp_file);
		return ERROR_CLOSE_FILE;
	}

	if (fclose(temp_file) == EOF)
		return ERROR_CLOSE_FILE;

	return OK;
}

status_t export_product_as_csv (const product_t *product, char delimiter, FILE *output_file)
{
	if (fprintf(output_file, "%lu%c%s%c%f\n", product -> id, delimiter, 
		product -> description, delimiter, product -> price) < 0)
		return ERROR_EXPORT_PRODUCT_AS_CSV;

	return OK;
}
