#include <stdio.h>
#define CTABLE_IMPL
#include "CTable.h"

int main() {
  Table *table = table_init("example table");
  printf("table name : %s\n", table->TableName);
  if (add_column(table, "Id", INTEGER, PRIMARY_KEY, 1, NULL) != 0) {
    printf("unable to add column\n");
    goto delete;
  }
  printf("no of columns in table %zu\n", table->NoOfColumms);
  if (add_row(table) != 0) {
    printf("unable to add row\n");
    goto delete;
  }
delete:
  if (table_delete(&table) != 0) {
    puts("unable to delete table");
  }
  return 0;
}
