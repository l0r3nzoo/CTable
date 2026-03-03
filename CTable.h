#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef CTABLE_H
#define CTABLE_H

#define INITIAL_COL_CAP 10
#define INITIAL_ROW_CAP 10
#define TABLE_NAME_CAP 20
#define COLUMN_NAME_CAP 20

typedef enum { INTEGER, DECIMAL, TEXT, DATE } DataType;

typedef enum {
  PRIMARY_KEY,
  FOREIGN_KEY,
  UNIQUE,
} ConstraintType;

typedef struct {
  char Name[COLUMN_NAME_CAP];
  DataType DataType;
  ConstraintType Constraint;
  int Nullable;
  void *Default;
} Column;

typedef struct {
  Column *Column;
  void *Value;
} Value;

typedef struct {
  Value *ColumnValues;
  size_t NoOfColumns;
} Row;

typedef struct {
  char TableName[TABLE_NAME_CAP];
  Column *Columns;
  size_t ColumnsCap;
  size_t NoOfColumms;
  Row *Rows;
  size_t RowsCap;
  size_t NoOfRows;
} Table;

Table *table_init(char *table_name);
int add_column(Table *self, char *column_name, DataType data_type,
               ConstraintType constrain_type, int nullable,
               void *default_value);
int add_row(Table *self);
int insert_row_value(Row *row, char *column_name, void *value);
int table_truncate(Table *self);
int table_delete(Table **self);

#ifdef CTABLE_IMPL

int validate_value(DataType type, void *value) {
  switch (type) {
  case INTEGER: {
    int *int_ptr = (int *)value;
    if (int_ptr == NULL) {
      return 1;
    } else {
      return 0;
    }
  } break;
  case DECIMAL: {
    float *float_ptr = (float *)value;
    if (float_ptr == NULL) {
      return 1;
    } else {
      return 0;
    }
  } break;
  case TEXT: {
    char *char_ptr = (char *)value;
    if (char_ptr == NULL) {
      return 1;
    } else {
      return 0;
    }
  } break;
  case DATE: {
    return 1;
  } break;
  }
  puts("invalid datatype");
  return 1;
}

Table *table_init(char *table_name) {
  Table *t = (Table *)malloc(sizeof(Table));
  if (t == NULL) {
    return NULL;
  }
  t->NoOfColumms = 0;
  t->NoOfRows = 0;
  t->ColumnsCap = INITIAL_COL_CAP;
  t->RowsCap = INITIAL_ROW_CAP;
  snprintf(t->TableName, TABLE_NAME_CAP, "%s", table_name);
  t->Columns = (Column *)malloc(sizeof(Column) * INITIAL_COL_CAP);
  if (t->Columns == NULL) {
    free(t);
    return NULL;
  }
  t->Rows = (Row *)malloc(sizeof(Row) * INITIAL_ROW_CAP);
  if (t->Rows == NULL) {
    free(t->Columns);
    free(t);
    return NULL;
  }
  return t;
}

int add_column(Table *self, char *column_name, DataType data_type,
               ConstraintType constrain_type, int nullable,
               void *default_value) {
  if (self == NULL) {
    return 1;
  }
  if (self->NoOfColumms == self->ColumnsCap) {
    size_t temp_cap = self->ColumnsCap + INITIAL_COL_CAP;
    Column *temp_allocation =
        (Column *)realloc(self->Columns, sizeof(Column) * temp_cap);
    if (temp_allocation == NULL) {
      perror("unable to create column");
      return 1;
    }
    self->ColumnsCap = temp_cap;
    self->Columns = temp_allocation;
  }
  if (default_value != NULL) {
    if (validate_value(data_type, default_value) == 1) {
      return 1;
    }
  }
  Column *col = (Column *)malloc(sizeof(Column));
  if (col == NULL) {
    perror("unable to create column");
    return 1;
  }
  snprintf(col->Name, COLUMN_NAME_CAP, "%s", column_name);
  col->Nullable = nullable;
  col->Constraint = constrain_type;
  col->DataType = data_type;
  col->Default = default_value;
  memcpy(self->Columns + self->NoOfColumms, col, sizeof(Column));
  free(col);
  self->NoOfColumms++;
  return 0;
}

int add_row(Table *self) {
  if (self == NULL) {
    return 1;
  }
  if (self->NoOfRows == self->RowsCap) {
    size_t temp_cap = self->RowsCap + INITIAL_ROW_CAP;
    Row *temp_allocation = (Row *)realloc(self->Rows, sizeof(Row) * temp_cap);
    if (temp_allocation == NULL) {
      perror("unable to create row");
      return 1;
    }
    self->RowsCap = temp_cap;
    self->Rows = temp_allocation;
  }
  Row *row = (Row *)malloc(sizeof(Row));
  if (row == NULL) {
    perror("unable to create row");
    return 1;
  }
  row->NoOfColumns = 0;
  row->ColumnValues = NULL;
  memcpy(self->Rows + self->NoOfRows, row, sizeof(Row));
  free(row);
  self->NoOfRows++;
  return 0;
}

int column_delete(Column *self) {
  if (self == NULL) {
    // null column passed
    return 1;
  }
  free(self->Default);
  return 0;
}
int row_delete(Row *self) {
  if (self == NULL) {
    return 1;
  }
  for (int i = 0; i < self->NoOfColumns; i++) {
    free(self->ColumnValues[i].Column);
    free(self->ColumnValues[i].Value);
  }
  return 0;
}

int table_delete(Table **selfpointer) {
  Table *selftable = *selfpointer;
  if (selftable == NULL) {
    return 1;
  }
  for (int i = 0; i < selftable->NoOfRows; i++) {
    if (row_delete(selftable->Rows + i) != 0) {
      return 1;
    }
  }
  for (int i = 0; i < selftable->NoOfColumms; i++) {
    if (column_delete(selftable->Columns + i) != 0) {
      return 1;
    }
  }
  free(selftable->Rows);
  free(selftable->Columns);
  free(selftable);

  *selfpointer = NULL;
  return 0;
}

int insert_row_value(Row *row, char *column_name, void *value) {
  if (row == NULL) {
    return 1;
  }
}

#endif

#endif
