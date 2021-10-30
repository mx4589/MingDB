#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "record.h"
#include "writeData.h"
#include "Bplus.h"

#ifndef mingDB
#define mingDB

void insert(char *command[]);
void delete(char *command[]);
void update(char *command[]);
char *get(char *command[]);

#endif
