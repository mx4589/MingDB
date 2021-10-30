#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "record.h"

int main()
{
	int recn;
	char buffer[409600];
	struct record *recs;
	scanf("%d",&recn);
	recs = (struct record *)malloc(sizeof(struct record)*recn);
	fgets(buffer,409600,stdin);
	int i;
	for(i=0;i<recn;i++)
	{
		fgets(recs[i].key,50,stdin);
		fgets(buffer,100,stdin);
		recs[i].offset = atoi(buffer);
		recs[i].del = true;
	}
	writeRecAll("yt_db0_rec.txt",recs,recn);
		
	struct record *recs2;
	recs2 = readRecAll("yt_db0_rec.txt");
	for(i=0;i<recn;i++)
	{
		printf("%s",recs2[i].key);
	}
	return 0;
}
