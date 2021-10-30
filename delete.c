#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "record.h"
#include "writeData.h"
#include "Bplus.h"

/*
   argv[1] is table name
   argv[2] is key
 */
int main(int argc,char *argv[])
{
	char database[100],indexbase[100],recbase[100],freebase[100];
	strcpy(database,argv[1]);
	strcat(database,"_data");

	strcpy(indexbase,argv[1]);
	//strcat(indexbase,"_index");

	strcpy(freebase,argv[1]);
	strcat(freebase,"_free");

	strcpy(recbase,argv[1]);
	strcat(recbase,"_rec");
	long int kloc = searchKey(indexbase,argv[2]);
	if(kloc < 0)
		return 0;
	deleteRec(recbase, kloc);


	return 0;
}
