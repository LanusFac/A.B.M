/* products.c */

#include <stdio.h>
#include <stdlib.h>
#include "products.h"

int compare_products_by_id (const product_t *inventory_product, const product_t *deletion_product)
{
	return (inventory_product -> id - deletion_product -> id);
}


status_t product_assing_attributes (char * string, product_t *product, char delimiter)
{
	char *aux_line, *aux_str, **str_arr, *temp;
	size_t len;

	if (product == NULL || string == NULL)
		return ERROR_NULL_POINTER;

	if ((str_arr = (char **) malloc(INIT_CHOP_ARR * sizeof(char *))) == NULL)
		return ERROR_OUT_MEMORY;

	for (aux_line = string, len = 0; (aux_str = strtok(aux_line, &delimiter)) != NULL; len++, aux_line = NULL)
	{
		if((str_arr[len] = (char *) malloc(sizeof(char) * (strlen(aux_str) + 1))) == NULL)
		{
			destroy_string(str_arr);

			return  ERROR_OUT_MEMORY;
		}

		strcpy(str_arr[len], aux_str);
	}

	product -> id = strtol(str_arr[ID_CSV_POSSITION], &temp, 10);

	if(*temp != '\0' && *temp != '\n')
		return ERROR_INVALID_DATA;

	if((product -> description = (char*) malloc(sizeof(char) * (strlen(str_arr[DESCRIPTION_CSV_POSSITION]) + 1))) == NULL)
	{
		destroy_string(str_arr);

		return ERROR_OUT_MEMORY;
	}

	strcpy(product -> description, str_arr [DESCRIPTION_CSV_POSSITION]);

	product -> price = strtod(str_arr[PRICE_CSV_POSSITION], &temp);

	if (*temp != '\0' && *temp != '\n')
		return  ERROR_INVALID_DATA;

	destroy_string(str_arr);

	return OK;
}

void destroy_string(char **str_arr)
{
	free(*str_arr);
	*str_arr = NULL;
}

status_t eliminate (FILE *inventory_file, FILE *deletion_file, FILE *output_file, char delimiter)
{
	product_t inventory_product, deletion_product;
	char *inv_str, *del_str;
	bool_t eof_inventory, eof_deletion;
	status_t status;

	if (inventory_file == NULL || deletion_file == NULL || output_file == NULL)
		return ERROR_NULL_POINTER;

	if ((status = read_text_line(inventory_file, &inv_str, &eof_inventory)) != OK)
	{
		free(&inv_str);
		return status;
	}

	if ((status = read_text_line(deletion_file, &del_str, &eof_deletion)) != OK)
	{
		free(&inv_str);
		free(&del_str);
		return status;
	}

	if ((status = product_assing_attributes(inv_str, &inventory_product, delimiter)) != OK)
	{
		free(&inv_str);
		free(&del_str);
		return status;
	}

	if ((status = product_assing_attributes(del_str, &deletion_product, delimiter)) != OK)
	{
		free(&inv_str);
		free(&del_str);
		return status;
	}

	while(eof_inventory == FALSE && eof_deletion == FALSE)
	{
		if (compare_products_by_id(&inventory_product, &deletion_product) == 0)
		{
			if ((status = read_text_line(deletion_file, &del_str, &eof_deletion)) != OK)
			{
				free(&inv_str);
				free(&del_str);
				return status;
			}

			if ((status = read_text_line(inventory_file, &inv_str, &eof_inventory)) != OK)
			{
				free(&inv_str);
				free(&del_str);
				return status;
			}

			if ((status = product_assing_attributes(del_str, &deletion_product, delimiter)) != OK)
			{
				free(&inv_str);
				free(&del_str);
				return status;
			}

			if ((status = product_assing_attributes(inv_str, &inventory_product, delimiter)) != OK)
			{
				free(&inv_str);
				free(&del_str);
				return status;
			}
		}
		
		else if (compare_products_by_id(&inventory_product, &deletion_product) < 0)
		{
			if ((status = export_product_as_csv(&inventory_product, delimiter, output_file)) != OK)
			{
				free(&inv_str);
				free(&del_str);
				return status;
			}

			if ((status = read_text_line(inventory_file, &inv_str, &eof_inventory)) != OK)
			{
				free(&inv_str);
				free(&del_str);
				return status;
			}

			if ((status = product_assing_attributes(inv_str, &inventory_product, delimiter)) != OK)
			{
				free(&inv_str);
				free(&del_str);
				return status;
			}
		}

		else if (compare_products_by_id(&inventory_product, &deletion_product) > 0)
			return ERROR_PRODUCT_DOES_NOT_EXIST;
	}

	while (eof_inventory == FALSE)
	{
		if ((status = export_product_as_csv(&inventory_product, delimiter, output_file)) != OK)
		{
			free(&inv_str);
			free(&del_str);
			return status;
		}

		if ((status = read_text_line(inventory_file, &inv_str, &eof_inventory)) != OK)
		{
			free(&inv_str);
			free(&del_str);
			return status;
		}

		if ((status = product_assing_attributes(inv_str, &inventory_product, delimiter)) != OK)
		{
			free(&inv_str);
			free(&del_str);
			return status;
		}
	}

	return OK;
}

status_t process_addition (FILE *inventory_file, FILE *addition_file, FILE *output_file, char delimiter)
{
	product_t inventory_product, addition_product;
	char *inv_str, *add_str;
	bool_t eof_inventory = FALSE, eof_addition = FALSE;
	status_t status;


	if (inventory_file == NULL || addition_file == NULL || output_file == NULL)
		return ERROR_NULL_POINTER;

	if ((status = read_text_line(inventory_file, &inv_str, &eof_inventory)) != OK)
	{
		free(&inv_str);
		return status;
	}

	if ((status = read_text_line(addition_file, &add_str, &eof_addition)) != OK)
	{
		free(&add_str);
		free(&inv_str);
		return status;
	}

	if ((status = product_assing_attributes(inv_str, &inventory_product, delimiter)) != OK)
	{
		free(&add_str);
		free(&inv_str);
		return status;
	}

	if ((status = product_assing_attributes(add_str, &addition_product, delimiter)) != OK)
	{
		free(&add_str);
		free(&inv_str);	
		return status;
	}

	while(eof_inventory == FALSE && eof_addition == FALSE)
	{
		if (compare_products_by_id (&inventory_product, &addition_product) == 0)
		{
			free(&add_str);
			free(&inv_str);
			return ERROR_DUPLICATED_KEY;
		}

		if (compare_products_by_id (&inventory_product, &addition_product) > 0)
		{
			if ((status = export_product_as_csv (&addition_product, delimiter, output_file)) != OK)
			{
				free(&add_str);
				free(&inv_str);
				return status;
			}

			if ((status = read_text_line(addition_file, &add_str, &eof_addition)) != OK)
			{
				free(&add_str);
				free(&inv_str);
				return status;
			}

			if ((status = product_assing_attributes(add_str, &addition_product, delimiter)) != OK)
			{
				free(&add_str);
				free(&inv_str);
				return status;
			}

		}

		else if (compare_products_by_id (&inventory_product, &addition_product) < 0)
		{
			if ((status = export_product_as_csv (&inventory_product, delimiter, output_file)) != OK)
			{
				free(&add_str);
				free(&inv_str);
				return status;
			}

			if ((status = read_text_line(inventory_file, &inv_str, &eof_inventory)) != OK)
			{
				free(&add_str);
				free(&inv_str);
				return status;
			}

			if ((status = product_assing_attributes(inv_str, &inventory_product, delimiter)) != OK)
			{
				free(&add_str);
				free(&inv_str);
				return status;
			}

		}
	}

	while (eof_inventory == FALSE)
	{
		if ((status = export_product_as_csv (&inventory_product, delimiter, output_file)) != OK)
		{
			free(&add_str);
			free(&inv_str);
			return status;
		}


		if ((status = read_text_line(inventory_file, &inv_str, &eof_inventory)) != OK)
		{
			free(&add_str);
			free(&inv_str);
			return status;
		}

		if ((status = product_assing_attributes(inv_str, &inventory_product, delimiter)) != OK)
		{
			free(&add_str);
			free(&inv_str);
			return status;
		}	
	}

	while (eof_addition == FALSE)
	{

		if ((status = export_product_as_csv (&addition_product, delimiter, output_file)) != OK)
		{
			free(&add_str);
			free(&inv_str);
			return status;
		}

		if ((status = read_text_line(addition_file, &add_str, &eof_addition)) != OK)
		{
			free(&add_str);
			free(&inv_str);
			return status;
		}	

		if ((status = product_assing_attributes(add_str, &addition_product, delimiter)) != OK)
		{
			free(&add_str);
			free(&inv_str);
			return status;
		}	
	}

	return OK;
}

