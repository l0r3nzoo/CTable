#include <stdio.h>
#define CTABLE_IMPL
#include "CTable.h"

int main(){
    Table* table=table_init("example table");
    printf("table name : %s\n",table->TableName);
    add_column(table, "Id", INTEGER, PRIMARY_KEY, 1, NULL);
    printf("no of columns in table %zu\n",table->NoOfColumms);
    add_row(table);
    return 0;
}
