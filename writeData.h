#ifndef _WRITEDATA_
#define _WRITEDATA_

#define dataSize 128
#define repSize  1048576

void writeToDB(int dsize, long int loc, char *inputBuf,char *dbName);

void createPollHead(char *dbName,int pollID);

void createPollTail(char *dbName);


long int appendData(char *newData,char *dbName);
long int appendToFile(char *dbName,char *newData);
char *readData(char *dbName, long int dloc,long int dlen);

#endif
