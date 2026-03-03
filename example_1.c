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

  for(int i=0;i<table->NoOfRows;i++){
      for(int j=0;j<table->NoOfColumms;j++){
          int value=120;
          printf("inserting value '%d' for row '%d' and column '%s'\n",value,i+1,table->Columns[j].Name);
          if(insert_row_value(table,&table->Rows[i],table->Columns[j].Name,&value)!=0){
              puts("unable to insert value");
          }
          printf("value of Id at row 1 : %d\n",(int*)table->Rows[i].ColumnValues[j].Value);
      }
  }

delete:
  if (table_delete(&table) != 0) {
    puts("unable to delete table");
  }
  return 0;
}
