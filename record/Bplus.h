#ifndef _BPLUS_
#define _BPLUS_

#define BLOCK_SIZE 1024
#define BUCKET_SIZE 1024
#define IndexDir "/media/ming/disk1/MingDB/index/"

#ifndef _BOOL_
#define _BOOL_
typedef enum { false, true } bool;
#endif

struct key_loc{
	char key[55];
	long int loc;
	bool nleaf;
};

struct DkNode{
	bool nLeaf,isRoot;
	char keys[BLOCK_SIZE][55];
	long int locs[BLOCK_SIZE+1];
	int norder; 
};

struct DkLeaf{
	struct key_loc datum[BUCKET_SIZE];
	int num;
	long int pre, nxt;
};

int binsearch(char **keys,int len,char *k);
int binsearch2(char keys[][55],int len,char *k);
struct DkLeaf *readOneLeaf(char *filename,long int fileID);
struct DkNode *readOneNode(char *filename, long int nodeLoc);
void writeOneLeaf(char *filename,long int fileID,struct DkLeaf *outdata);
void writeOneNode(char *filename, struct DkNode *outData,long int nodeLoc);
long int creatFileNode(char *filename);
long int createFileLeaf(char *filename);
struct key_loc *insertLeaf(char *filename, long int fileID,char *newkey,long int newloc);
struct key_loc *addKey(struct DkNode *root,char *filename,char *newkey,long int newloc);
void insertKey(char *tableName,char *newkey,long int newloc);
long int searchRec(char *filename, long int sloc, char *skey);
long int searchKey(char *tableName, char *skey);
void updateLoc(char *tableName,char *key, long int newLoc);

#endif
