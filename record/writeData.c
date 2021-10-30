#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <sys/stat.h> 
#include <fcntl.h>
#include <unistd.h>
#include "writeData.h"

void writeToDB(int dsize, long int dloc, char *inputBuf,char *dbName)
{
	int fd = open(dbName,O_RDWR | O_CREAT, 0777);
	lseek(fd, dloc, SEEK_SET); 
	int fw = write(fd,inputBuf,dsize);
	printf("%d\n",fw);
	close(fd);	

}

long int appendData(char *newData,char *dbName)
{
/*	int fd = open(dbName,O_RDWR | O_CREAT, 0777);
	long int dataLoc = lseek(fd,0L,SEEK_END);
	
	int len = strlen(newData);
	write(fd,newData,len);
	close(fd);*/
	FILE *fd  = NULL;
	fd = fopen(dbName,"r+");
	if(fd==NULL)
	{
		fd = fopen(dbName,"a+");
	}
	fseek(fd,0,SEEK_END);
	long int sz = ftell(fd);
	fwrite(newData,1,strlen(newData),fd);
	fclose(fd);
	
	
	return sz;
}
void createPollHead(char *dbName,int pollID)
{
	int fd = open(dbName,O_APPEND,0777);
	int fa;
	
	//int pollID = 1;
	fa = write(fd,&pollID,4);
	
	int dataCnt = 0;		//Position of data count is 4
	fa = write(fd, &dataCnt,4);	

	long int pollSize = 0;		//Position of poll size is 8
	fa = write(fd, &pollSize,8);

	long int dataPos[dataSize +1 ];			//Array of data position is at 16
	memset(dataPos,0, 8 * dataSize + 8);
	fa = write(fd,dataPos, 4 * dataSize + 4);
	
	long int headPos = 16 + 8 *dataSize + 8;
	fa = write(fd, &headPos,8);
	
	close(fd);
}

void createPollTail(char *dbName)
{
	int fd = open(dbName, O_RDWR, 0777);
	int fa;
	lseek(fd,-8,SEEK_END);
	
	long int headPos;
	fa = read(fd,&headPos,8);
	lseek(fd, -1 * headPos, SEEK_CUR);
	
	int newID;
	fa = read(fd,&newID,4);
	newID++;

	long int pollSize;
	fa = read(fd,&pollSize,8);
	long int empBufSize = pollSize / 10;
	pollSize += empBufSize;
	
	lseek(fd,-8,SEEK_CUR);
	write(fd,&pollSize,8);
	
	lseek(fd,0,SEEK_END);
	
	char emp[empBufSize];
	memset(emp,0,empBufSize);
	write(fd,emp,empBufSize);
	
	close(fd);
	
	createPollHead(dbName,newID);
}

long int appendToFile(char *dbName,char *newData)
{
	int fd = open(dbName, O_RDWR, 0777);
	lseek(fd,-8,SEEK_END);
	long int headPos;
	read(fd,&headPos,8);
	lseek(fd, -1 * headPos, SEEK_CUR);
	int ID;
	read(fd,&ID, 4);
	
	int dataCnt;
	read(fd,&dataCnt,4);
	
	long int pollSize;
	read(fd,&pollSize,8);
	
	int len = strlen(newData);
	lseek(fd,8*(dataCnt - 1),SEEK_CUR);
	
	long int writeLoc;
	read(fd,&writeLoc,8);
	
	long int nLoc = len + writeLoc + 8;
	write(fd,&nLoc,8);
	
	dataCnt++;
	lseek(fd,dataSize-dataCnt * 8,SEEK_CUR);
	write(fd,newData,len);
	
	pollSize+= (len+4);
	write(fd,&nLoc,8);
	
	lseek(fd,-1 * nLoc + 4,SEEK_CUR);
	write(fd,&dataCnt,4);
	
	write(fd,&pollSize,8);	
	
	long int ret = (long int)ID << 7 + (dataCnt-1);
	return ret;
}



/*char *readData(char *dbName,long int)
{
	
}*/
