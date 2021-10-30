
#ifndef _RECORD_
#define _RECORD_

#ifndef _BOOL_
#define _BOOL_
typedef enum { false, true } bool;
#endif

struct record{
	long int offset; 
	char key[100];
	bool del;
	int length;
};
void writeRecAll(char *fileName,struct record *outData, int recn);

struct record *readRecAll(char *fileName);

struct record *readRec(char *fileName,long int recLoc, int recCnt);

void writeRec(char *fileName,struct record *outData, long int recLoc,int recCnt);

long int appendRec(char *filename, struct record *outData, int recCnt);

void deleteRec(char *filename,long int recLoc);
long int fillLoc(char *filename,long int dlen);
#endif
