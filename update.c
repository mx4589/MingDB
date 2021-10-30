#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include "record.h"
#include "writeData.h"
#include "Bplus.h"

/*
   argv[1] is table name
   argv[2] is data
   argv[3] is key
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
	long int kloc = searchKey(indexbase,argv[3]);
	if(kloc < 0)
		return 0;
	deleteRec(recbase, kloc);

	long int len = strlen(argv[2]);
	//printf("%ld\n",len);
	long int updLoc = fillLoc(recbase,len);
	//printf("%ld\n",updLoc);
	struct record inRec;
	if(updLoc == -1)
	{
		long int datloc = appendData(argv[2],database);
		inRec.offset = datloc;
		strcpy(inRec.key,argv[3]);
		inRec.del = false;
		inRec.length = len;
		long int rloc = appendRec(recbase,&inRec,1);
		updateLoc(indexbase,inRec.key,rloc);
	}
	else
	{

		FILE *fd  = NULL;
		fd = fopen(database,"r+");
		fseek(fd,updLoc,SEEK_SET);
		fwrite(argv[2],1,len,fd);
		fclose(fd);
		strcpy(inRec.key,argv[3]);
		inRec.offset = updLoc;
		inRec.del = false;
		inRec.length = len;
		long int tloc = appendRec(recbase,&inRec,1);
		updateLoc(indexbase,inRec.key,tloc);
	}



	return 0;
}
