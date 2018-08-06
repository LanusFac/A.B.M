/* products.h */

#ifndef PRODUCTS__H
#define PRODUCTS__H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define ID_CSV_POSSITION 0
#define DESCRIPTION_CSV_POSSITION 1
#define PRICE_CSV_POSSITION 2

#define INIT_CHOP_ARR 20

int compare_products_id (const product_t *inventory_product, const product_t *deletion_product);
status_t product_assing_attributes (char *string, product_t *product, char delimiter);
status_t read_text_line (FILE *fi, char **string, bool_t *eof);
status_t export_product_as_csv (const product_t *product, char delimiter, FILE *output_file);
void destroy_string(char **str_arr);

#endif
