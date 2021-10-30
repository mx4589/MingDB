#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <mcheck.h>

#include<dirent.h>

#include "Bplus.h"




int binsearch(char **keys,int len,char *k)
{
	int mid = len/2, pre =-1,l=0,r = len;
	while(l<=r && mid <len)
	{
		if(pre == mid)
			break;
		pre = mid;
		//		printf("=====%s\n",keys[0]);
		int cmp = strcmp(k,keys[mid]);
		if(cmp < 0)
		{
			r = mid;
			mid = (l +r)/2;
		}
		else if(cmp > 0)
		{
			l = mid+1;
			mid = (l+r)/2;
		}
		else
		{
			mid;
			break;
		}
	}
	return mid;
}

int binsearch2(char keys[][55],int len,char *k)
{
	int mid = len/2, pre =-1,l=0,r = len;
	while(l<=r && mid <len)
	{
		if(pre == mid)
			break;
		pre = mid;
		//		printf("=====%s\n",keys[0]);
		int cmp = strcmp(k,keys[mid]);
		if(cmp < 0)
		{
			r = mid;
			mid = (l +r)/2;
		}
		else if(cmp > 0)
		{
			l = mid+1;
			mid = (l+r)/2;
		}
		else
		{
			break;
		}
	}
	return mid;
}


struct DkLeaf *readOneLeaf(char *filename,long int fileID)
{
	FILE *inFile;
	char IDstr[10],leafName[150];

	sprintf(IDstr,"%ld",fileID);
	strcpy(leafName,filename);
	strcat(leafName,"_leaf");
	strcat(leafName,IDstr);
	//char *leafName2 = strcat(IndexDir,leafName);
	inFile = fopen(leafName,"r+");
	struct DkLeaf *inData = (struct DkLeaf *)malloc(sizeof(struct DkLeaf));
	//printf("Read Leaf %s begin\n",leafName);
	fread(inData,sizeof(struct DkLeaf),1,inFile);
	//	printf("Read Leaf end\n");
	fclose(inFile);
	return inData;
}
struct DkNode *readOneNode(char *filename, long int nodeLoc)
{
	FILE *inFile;
	//filename = strcat(IndexDir,filename);
	inFile = fopen(filename, "r+");
	int fs = fseek(inFile, nodeLoc, SEEK_SET);
	struct DkNode *inData = (struct DkNode *)malloc(sizeof(struct DkNode));
	fs = fread(inData, sizeof(struct DkNode), 1, inFile);
	fclose(inFile);
	/*if(inData->norder > 256)
	{
		printf("exit at %ld\n",nodeLoc);
		exit(0);
	}*/
	return inData;
}

void writeOneLeaf(char *filename,long int fileID,struct DkLeaf *outdata)
{
	

	//printf("	4\n");
	FILE *outFile;
	char IDstr[10],leafName[150];

	//printf("0    %s\n",leafName);
	sprintf(IDstr,"%ld",fileID);
	strcpy(leafName,filename);
	strcat(leafName,"_leaf");
	strcat(leafName,IDstr);

	//char *leafName2 = strcat(IndexDir,leafName);
	outFile = fopen(leafName,"w+");
	//printf("1    %s\n",leafName);
	fwrite(outdata,sizeof(struct DkLeaf),1,outFile);
	fclose(outFile);
}
void writeOneNode(char *filename, struct DkNode *outData,long int nodeLoc)
{
	if(nodeLoc==0)
	{
		printf("Fail Pos\n");
		exit(0);
	}
	//filename = strcat(IndexDir,filename);
	FILE *outfile;
	outfile = fopen(filename, "r+");
	int fs = fseek(outfile, nodeLoc,SEEK_SET);
	fwrite(outData,sizeof(struct DkNode),1,outfile);
	fclose(outfile);
}



long int creatFileNode(char *filename)
{
	//filename = strcat(IndexDir,filename);
	struct DkNode *newNode = (struct DkNode *)malloc(sizeof(struct DkNode));
	FILE *fp = fopen(filename,"r+");
	fseek(fp, 0L, SEEK_END);
	long int sz = ftell(fp);
	//printf("%ld\n",sz);
	fclose(fp);
	newNode->norder = 0;
	writeOneNode(filename, newNode, sz);
	free(newNode);
	return sz;
}

long int createFileLeaf(char *filename)
{
	//printf("Enter createLeaf\n");
	char preleaf[100],num[10];
	strcpy(preleaf,filename);
	strcat(preleaf,"_leaf");
	int prelen = strlen(preleaf);
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	bool e[999999];
	memset(e,0,sizeof(bool)*999999);
	if(d)
	{
		while ((dir = readdir(d)) != NULL)
		{
			if(strncmp(preleaf,dir->d_name,prelen)==0)
			{
				strcpy(num, dir->d_name + prelen);
				e[atoi(num)] = true;
			}

		}
		closedir(d);
	}
	//printf("55555511199999999999\n");

	long int j;
	for(j=0; j<999999; j++)
	{
		if(!e[j])
		{
			struct DkLeaf *newleaf = (struct DkLeaf *)malloc(sizeof(struct DkLeaf));
			writeOneLeaf(filename,j,newleaf);
			free(newleaf);
			break;
		}
	}
	//printf("Exit createLeaf\n");
	return j;
}

struct key_loc *insertLeaf(char *filename, long int fileID,char *newkey,long int newloc)
{
	//printf("insertLeaf %s\n",filename);
	struct DkLeaf *leaf = readOneLeaf(filename,fileID);
	int k,j;
	char **indexArr= (char **)malloc(sizeof(char *) * leaf->num);
	for(j=0;j < leaf->num; j++)
	{
		indexArr[j] = &(leaf->datum[j].key[0]);
	}
	if(leaf->num < BUCKET_SIZE)
	{
		//printf("4444444444\n");
		//k = binsearch(indexArr, leaf->num, newkey);
		for(k=0; k < leaf->num; k++)
		{	
			int cmp = strcmp(newkey, indexArr[k]);
			if(cmp < 0)
				break;
		}
		/*if((k >0 &&strcmp(newkey,indexArr[k-1])<0) || (k<leaf->num-1 && strcmp(newkey,indexArr[k+1]) >0))
		{
			printf("Failed Order of Leaf\n");
			exit(0);
		}*/
		for(j= leaf->num; j>k;j--)
		{
			strcpy(leaf->datum[j].key,leaf->datum[j-1].key);
			leaf->datum[j].loc = leaf->datum[j-1].loc;
		}
		strcpy(leaf->datum[j].key,newkey);
		leaf->datum[j].loc = newloc;
		leaf->num++;
		writeOneLeaf(filename, fileID, leaf);
		free(leaf);
		free(indexArr);
		return NULL;
	}
	else
	{	
		//printf("000999999999\n");
		long int nfid = createFileLeaf(filename);
		struct DkLeaf *nleaf = (struct DkLeaf *)malloc(sizeof(struct DkLeaf));

		//k = binsearch(indexArr, leaf->num, newkey);
		for(k=0; k < leaf->num; k++)
		{	
			int cmp = strcmp(newkey, indexArr[k]);
			if(cmp < 0)
				break;
		}
		/*if((k >0 &&strcmp(newkey,indexArr[k-1])<0) || (k<leaf->num-1 && strcmp(newkey,indexArr[k+1]) >0))
		{
			printf("Failed Order of Leaf\n");
			exit(0);
		}*/
		if(k <= BUCKET_SIZE/2)
		{
			nleaf->num = BUCKET_SIZE/2;
			for(j=BUCKET_SIZE-1; j>=BUCKET_SIZE/2; j--)
			{	
				strcpy(nleaf->datum[j - BUCKET_SIZE/2].key, leaf->datum[j].key);
				nleaf->datum[j - BUCKET_SIZE/2].loc = leaf->datum[j].loc;	
			}
			writeOneLeaf(filename, nfid, nleaf);

			//printf("11999999999999\n");
			leaf->num = BUCKET_SIZE/2 + 1;
			for(j = BUCKET_SIZE/2; j>k;j--)
			{
				strcpy(leaf->datum[j].key, leaf->datum[j-1].key);
				leaf->datum[j].loc = leaf->datum[j-1].loc;
			}
			strcpy(leaf->datum[j].key,newkey);
			leaf->datum[j].loc = newloc;
			writeOneLeaf(filename, fileID, leaf);

		/*	printf("22999999999999\n");
			if(strcmp(leaf->datum[BUCKET_SIZE/2-1].key,nleaf->datum[0].key)>=0)
			{
				printf("Splite Leaf failed\n");
				exit(0);
			}*/
			/*int df;
			for(df=1;df<nleaf->num;df++)
			{
				if(strcmp(nleaf->datum[df-1].key,nleaf->datum[df].key)>=0)
				{
					printf("nleaf order failed\n");
					exit(0);
				}
			}
			for(df=1;df<leaf->num;df++)
			{
				if(strcmp(leaf->datum[df-1].key,leaf->datum[df].key)>=0)
				{
					printf("leaf order failed\n");
					exit(0);
				}
			}*/

		}
		else
		{
	//		printf("111999999999999\n");
			leaf->num = BUCKET_SIZE/2 ;
			writeOneLeaf(filename, fileID, leaf);
		//	printf("%d\n",k);
			nleaf->num = BUCKET_SIZE/2 + 1;
			for(j=BUCKET_SIZE/2; j<k;j++)
			{
				strcpy(nleaf->datum[j - BUCKET_SIZE/2].key, leaf->datum[j].key);
				nleaf->datum[j - BUCKET_SIZE/2].loc = leaf->datum[j].loc;
			}
	//		printf("111999999999999\n");
			strcpy(nleaf->datum[j-BUCKET_SIZE/2].key,newkey);
			nleaf->datum[j - BUCKET_SIZE/2].loc = newloc;
			for(j=j+1; j<=BUCKET_SIZE ;j++)
			{	
				strcpy(nleaf->datum[j-BUCKET_SIZE/2].key, leaf->datum[j - 1].key);
				nleaf->datum[j-BUCKET_SIZE/2].loc = leaf->datum[j - 1].loc;
			}
	//		printf("111999999999999\n");
			writeOneLeaf(filename, nfid, nleaf);
			/*if(strcmp(leaf->datum[BUCKET_SIZE/2-1].key,nleaf->datum[0].key)>=0)
			{
				printf("Splite Leaf failed\n");
				exit(0);
			}*/
			int df;
			/*for(df=1;df<nleaf->num;df++)
			{
				if(strcmp(nleaf->datum[df-1].key,nleaf->datum[df].key)>=0)
				{
					printf("nleaf order failed\n");
					exit(0);
				}
			}
			for(df=1;df<leaf->num;df++)
			{
				if(strcmp(leaf->datum[df-1].key,leaf->datum[df].key)>=0)
				{
					printf("leaf order failed\n");
					exit(0);
				}
			}*/

		}
		struct key_loc *leafRet = (struct key_loc *)malloc(sizeof(struct key_loc));
		//strcpy(leafRet->key,leaf->datum[leaf->num-1].key);
		strcpy(leafRet->key,leaf->datum[leaf->num-1].key);
		leafRet->loc = nfid;
		leafRet->nleaf = true;
		//printf("33999999999999\n");
		free(leaf);
		free(nleaf);
		free(indexArr);
		return leafRet;

	}
	free(indexArr);
	return NULL;

}

int leafn = 0;
struct key_loc *addKey(struct DkNode *root,char *filename,char *newkey,long int newloc)
{
	//exit(0);
	//printf("%d\n",root->norder);
	int i,preOrd,latOrd;
	for(i=0; i < root->norder; i++)
	{
		int cmp = strcmp(newkey, root->keys[i]);
		if(cmp < 0)
			break;
	}
	//printf("%d\n",i);
	struct key_loc *sp;
	if(!root->nLeaf)
	{
		struct DkNode *child = readOneNode(filename, root->locs[i]);
		preOrd = child->norder;
		sp = addKey(child, filename,newkey, newloc);
		latOrd = child->norder;
		if(preOrd != latOrd)
		{
			/*if(root->locs[i]==0)
			{
				printf("ddddddddddddddddddddddd\n");
	
				exit(0);
			}*/
			writeOneNode(filename, child, root->locs[i]);
		}
		//printf("55555555\n");
		free(child);
	}
	else
	{
		sp = insertLeaf(filename, root->locs[i],newkey,newloc);
	/*	if(sp!=NULL)
		{
		}*/
	}
	///printf("leafn ====== %d\n",leafn);

	if(sp != NULL)
	{	
		//printf("sp is not NULL and root order is %d\n",root->norder); 
		if(root->norder < BLOCK_SIZE)
		{
			/*int qas;
			for(qas=1;qas<root->norder;qas++)
			{
				if(strcmp(root->keys[qas-1],root->keys[qas])>0)
				{
					printf("%d %d %d Order failed\n%s%s\n",root->norder,qas-1,qas,root->keys[qas-1],root->keys[qas]);
					exit(0);
				}
			}*/
			//printf("1 start\n");
			int j,k;
			for(k=0; k < root->norder; k++)
			{
				int cmp = strcmp(sp->key, root->keys[k]);
				if(cmp < 0)
				break;
			}
			//k =i;
			for(j=root->norder;j>k; j--)
			{
				strcpy(root->keys[j],root->keys[j-1]);
			}
			strcpy(root->keys[k],sp->key);
			for(j=root->norder+1;j>k+1;j--)
			{
				root->locs[j] = root->locs[j-1];
			}
			root->locs[k+1] = sp->loc;
			root->norder++;
			free(sp);
			++leafn;
			//printf("1 end\n");

			/*for(qas=1;qas<root->norder;qas++)
			{
				if(strcmp(root->keys[qas-1],root->keys[qas])>0)
				{
					printf("%d %d %d Order failed\n%s%s\n",root->norder,qas-1,qas,root->keys[qas-1],root->keys[qas]);
					exit(0);
				}
			}*/
			return NULL;
		}
		else if(root->norder == BLOCK_SIZE)
		{
			
			int k = i,j;
			struct key_loc *up = (struct key_loc *)malloc(sizeof(struct key_loc));
			for(k=0; k < root->norder; k++)
			{
				int cmp = strcmp(sp->key, root->keys[k]);
				if(cmp < 0)
				break;
			}

			if(k== BLOCK_SIZE/2)
			{
				//printf("2 start\n");
				struct DkNode *newsib = (struct DkNode *)malloc(sizeof(struct DkNode));
				newsib->norder = BLOCK_SIZE / 2 ;

				strcpy(up->key,sp->key);
				up->loc = creatFileNode(filename);

				root->norder = BLOCK_SIZE/2;
				int j;
				for(j = BLOCK_SIZE/2; j < BLOCK_SIZE;j++)
				{
					strcpy(newsib->keys[j-BLOCK_SIZE/2],root->keys[j]); 
				}
				for(j= BLOCK_SIZE/2+1; j < BLOCK_SIZE+1 ; j++)
				{
					newsib->locs[j - BLOCK_SIZE/2] = root->locs[j];
				}
				newsib->locs[0] = sp->loc;
				newsib->nLeaf = root->nLeaf;
				writeOneNode(filename, newsib, up->loc);
			/*	if(strcmp(root->keys[BLOCK_SIZE/2-1],newsib->keys[0])>0)
				{
					printf("Splite failed\n%s%s\n",root->keys[BLOCK_SIZE/2-1],newsib->keys[0]);
					exit(0);
				}*/
				/*int qas;
				for(qas=1;qas<root->norder;qas++)
				{
					if(strcmp(root->keys[qas-1],root->keys[qas])>0)
					{
						printf("Root %d %d %d Order failed\n%s%s\n",root->norder,qas-1,qas,root->keys[qas-1],root->keys[qas]);
						exit(0);
					}
					if(strcmp(newsib->keys[qas-1],newsib->keys[qas])>0)
					{
						printf("Sib %d %d %d Order failed\n%s%s\n",newsib->norder,qas-1,qas,newsib->keys[qas-1],newsib->keys[qas]);
						exit(0);
					}
				}
				if(strcmp(root->keys[root->norder-1],up->key) >= 0 )
				{
					printf("Left splite failed\n");
					exit(0);
				}
				if(strcmp(newsib->keys[0],up->key) <= 0 )
				{
					printf("Right splite failed\n");
					exit(0);
				}*/
				
				free(newsib); 
				//printf("2 end\n");

			}
			else if(k < BLOCK_SIZE/2)
			{	
				int qas;
				for(qas=1;qas<root->norder;qas++)
				{
					if(strcmp(root->keys[qas-1],root->keys[qas])>0)
					{
						//printf("Order failed\n%s%s\n",root->keys[qas-1],root->keys[qas]);
						exit(0);
					}
				}
				//printf("3 start\n");
				up->loc = creatFileNode(filename);
				struct DkNode *newsib = (struct DkNode *)malloc(sizeof(struct DkNode));
				newsib->norder = BLOCK_SIZE / 2 ;
				for(j = BLOCK_SIZE/2; j<BLOCK_SIZE;j++)
				{
					strcpy(newsib->keys[j-BLOCK_SIZE/2],root->keys[j]);
				}
				for(j = BLOCK_SIZE/2 ;j<=BLOCK_SIZE;j++)
				{
					newsib->locs[j-BLOCK_SIZE/2] = root->locs[j];
				}
				newsib->nLeaf = root->nLeaf;
				writeOneNode(filename, newsib, up->loc);


				
				

				strcpy(up->key,root->keys[BLOCK_SIZE/2 -1]);

				root->norder = BLOCK_SIZE/2;
				for(j=BLOCK_SIZE/2-1; j>k; j--)
				{	
					strcpy(root->keys[j],root->keys[j-1]);
				}
				strcpy(root->keys[k],sp->key);
				for(j = BLOCK_SIZE/2; j>k+1; j--)
				{
					root->locs[j] = root->locs[j-1];
				}
				root->locs[k+1] = sp->loc;
				
				/*if(strcmp(root->keys[BLOCK_SIZE/2-1],newsib->keys[0])>0)
				{
					printf("Splite failed\n%s%s\n",root->keys[BLOCK_SIZE/2-1],newsib->keys[0]);
					exit(0);
				}*/
			//	int qas;
				/*for(qas=1;qas<root->norder;qas++)
				{
					if(strcmp(root->keys[qas-1],root->keys[qas])>0)
					{
						printf("Root %d %d %d Order failed\n%s%s\n",root->norder,qas-1,qas,root->keys[qas-1],root->keys[qas]);
						exit(0);
					}
					if(strcmp(newsib->keys[qas-1],newsib->keys[qas])>0)
					{
						printf("Sib %d %d %d Order failed\n%s%s\n",newsib->norder,qas-1,qas,newsib->keys[qas-1],newsib->keys[qas]);
						exit(0);
					}
				}
				if(strcmp(root->keys[root->norder-1],up->key) >= 0 )
				{
					printf("Left splite failed\n");
					exit(0);
				}
				if(strcmp(newsib->keys[0],up->key) <= 0 )
				{
					printf("Right splite failed\n");
					exit(0);
				}*/
				if(newsib->nLeaf)
				{
					int pl;
					struct DkLeaf *tleaf1,*tleaf2;
					tleaf1 = readOneLeaf(filename,newsib->locs[0]);
					for(pl=1;pl<=newsib->norder;pl++)
					{
						tleaf2 = readOneLeaf(filename,newsib->locs[0]);
						if(strcmp(tleaf1->datum[tleaf1->num-1].key,tleaf2->datum[0].key) >=0)
						{

							//printf("2 Leaf fail order\n");	
						}
						free(tleaf1);
						tleaf1 = tleaf2;
					}
					free(tleaf2);
				}
				if(root->nLeaf)
				{
					int pl;
					struct DkLeaf *tleaf1,*tleaf2;
					tleaf1 = readOneLeaf(filename,root->locs[0]);
					for(pl=1;pl<=newsib->norder;pl++)
					{
						tleaf2 = readOneLeaf(filename,root->locs[0]);
						if(strcmp(tleaf1->datum[tleaf1->num-1].key,tleaf2->datum[0].key) >=0)
						{
							//printf("2 Leaf fail order\n");	
						}
						free(tleaf1);
						tleaf1 = tleaf2;
					}
					free(tleaf2);
				}


				free(newsib);
				//printf("3 end\n");

			}
			else if(k > BLOCK_SIZE/2)
			{
				//printf("4 start\n");
				strcpy(up->key,root->keys[BLOCK_SIZE/2]);
				up->loc = creatFileNode(filename);

				root->norder = BLOCK_SIZE/2;

				struct DkNode *newsib = (struct DkNode *)malloc(sizeof(struct DkNode));

				int j, k=i;
				newsib->norder = BLOCK_SIZE/2;
				for(j=BLOCK_SIZE/2 + 1; j < k; j++)
				{
					strcpy(newsib->keys[j - BLOCK_SIZE/2 -1], root->keys[j]);
				}
				strcpy(newsib->keys[k - BLOCK_SIZE/2-1], sp->key);
				for(j=k+1; j <= BLOCK_SIZE; j++)
				{
					strcpy(newsib->keys[j - BLOCK_SIZE/2-1], root->keys[j-1]);
				}

				for(j=BLOCK_SIZE/2+1; j<=k; j++)
				{
					newsib->locs[j - BLOCK_SIZE/2-1] = root->locs[j];
				}
				newsib->locs[k - BLOCK_SIZE/2] = sp->loc;
				for(j=k+2; j<=BLOCK_SIZE+1; j++)
				{
					newsib->locs[j - BLOCK_SIZE/2 -1] = root->locs[j-1];
				}
				newsib->nLeaf = root->nLeaf;
				
				/*if(strcmp(root->keys[BLOCK_SIZE/2-1],newsib->keys[0])>0)
				{
					printf("Splite failed\n%s%s\n",root->keys[BLOCK_SIZE/2-1],newsib->keys[0]);
					exit(0);
				}*/

				writeOneNode(filename, newsib, up->loc);
				/*int qas;
				for(qas=1;qas<root->norder;qas++)
				{
					if(strcmp(root->keys[qas-1],root->keys[qas])>0)
					{
						printf("Root %d %d %d Order failed\n%s%s\n",root->norder,qas-1,qas,root->keys[qas-1],root->keys[qas]);
						exit(0);
					}
					if(strcmp(newsib->keys[qas-1],newsib->keys[qas])>0)
					{
						printf("Sib %d %d %d Order failed\n%s%s\n",newsib->norder,qas-1,qas,newsib->keys[qas-1],newsib->keys[qas]);
						exit(0);
					}
				}
				if(strcmp(root->keys[root->norder-1],up->key) >= 0 )
				{
					printf("Left splite failed\n");
					exit(0);
				}
				if(strcmp(newsib->keys[0],up->key) <= 0 )
				{
					printf("Right splite failed\n");
					exit(0);
				}*/
				if(newsib->nLeaf)
				{
					int pl;
					struct DkLeaf *tleaf1,*tleaf2;
					tleaf1 = readOneLeaf(filename,newsib->locs[0]);
					for(pl=1;pl<=newsib->norder;pl++)
					{
						tleaf2 = readOneLeaf(filename,newsib->locs[0]);
						if(strcmp(tleaf1->datum[tleaf1->num-1].key,tleaf2->datum[0].key) >=0)
						{
							//printf("2 Leaf fail order\n");	
						}
						free(tleaf1);
						tleaf1 = tleaf2;
					}
					free(tleaf2);
				}
				if(root->nLeaf)
				{
					int pl;
					struct DkLeaf *tleaf1,*tleaf2;
					tleaf1 = readOneLeaf(filename,root->locs[0]);
					for(pl=1;pl<=newsib->norder;pl++)
					{
						tleaf2 = readOneLeaf(filename,root->locs[0]);
						if(strcmp(tleaf1->datum[tleaf1->num-1].key,tleaf2->datum[0].key) >=0)
						{
							//printf("2 Leaf fail order\n");	
						}
						free(tleaf1);
						tleaf1 = tleaf2;
					}
					free(tleaf2);
				}

				free(newsib); 
				//printf("4 end\n");

			}
			//printf("Finish splite\n");
			//root->norder = BLOCK_SIZE/2;
			root->nLeaf = sp->nleaf;
			up->nleaf = false;
			free(sp);
			//printf("Return up success\n");
			return up;
		}
	}
	else
	{
		
		//printf("Return NULL success\n");
		return NULL;
	}

}


int oo = 0;
int yyyy=0;
void insertKey(char *tableName,char *newkey,long int newloc)
{
	char filename[100];
	strcpy(filename, tableName);
	strcat(filename,"_db_node");
	
	FILE *fp = fopen(filename,"r+");
	if(fp==NULL)
		fp = fopen(filename,"a+");
	fseek(fp,0,SEEK_END);
	long int sz = ftell(fp);
	fseek(fp,0,SEEK_SET);
	long int pos = 8;
	//printf("Insert %s\n",newkey);
	//printf("111111111\n");
	/*if(sz==0)
	  {
	  fwrite(&pos,8,1,fp);
	  fclose(fp);
	  struct DkNode *root = (struct DkNode *)malloc(sizeof(struct DkNode));
	  root->norder = 1;
	  strcpy(root->keys[0],newkey);
	  root->isRoot = true;
	  root->nLeaf = true;
	  struct DkLeaf *leaf = (struct DkLeaf *)malloc(sizeof(struct DkLeaf));
	  leaf->num = 1;
	  strcpy(leaf->datum[0].key,newkey);
	  leaf->datum[0].loc = newloc;
	  long int leafID1 = createFileLeaf(filename);
	  writeOneLeaf(filename, leafID1, leaf);
	  root->locs[0] = leafID1;

	  leaf->num = 0;
	  long int leafID2 = createFileLeaf(filename);
	  writeOneLeaf(filename,	 leafID2, leaf);
	  root->locs[1] = leafID2;

	  free(leaf);
	  writeOneNode(filename,root,8);
	  free(root);
	  }*/
	if(sz==0)
	{
		//printf("333333");
		pos = 0;
		fwrite(&pos,8,1,fp);
		fclose(fp);
		long int leafID = createFileLeaf(filename);
		struct DkLeaf *leaf = (struct DkLeaf *)malloc(sizeof(struct DkLeaf));
		leaf->num=1;
		strcpy(leaf->datum[0].key,newkey);
		leaf->datum[0].loc = newloc;
		writeOneLeaf(filename, leafID, leaf);
		free(leaf);
	}
	else if(sz==8)
	{

		struct key_loc *sp = insertLeaf(filename, 0,newkey,newloc);
		if(sp!=NULL)
		{
			fclose(fp);
			FILE *rfp = fopen(filename,"w+");
			pos = 8;
			fwrite(&pos,8,1,rfp);
			fclose(rfp);

			struct DkNode *root = (struct DkNode *)malloc(sizeof(struct DkNode));
			root->norder = 1;
			root->isRoot = true;
			root->nLeaf = true;
			strcpy(root->keys[0],sp->key);
			root->locs[0] = 0;
			root->locs[1] = sp->loc;
			writeOneNode(filename,root,8);
			free(root);
			free(sp);
		}
		else
		{
			fclose(fp);
		}
		/*printf("765654554\n");
		  FILE *ftest = fopen(filename,"r+");
		  fread(&pos,8,1,ftest);
		  printf("343242-----%ld\n",pos);
		  fclose(ftest);*/

	}
	else
	{
		yyyy++;
		fread(&pos,sizeof(long int),1,fp);
		//printf("pos = %ld loc = %ld\n enter time = %d",pos,ftell(fp),yyyy);
		fclose(fp);
		struct DkNode* root = readOneNode(filename,pos);
		int preOrd = root->norder;
		//printf("%d\n",root->norder);
		/*if(root->norder==257)
		{
			printf("%ld\n",pos);
			exit(0);
		}*/
		struct key_loc *sp = addKey(root,filename, newkey, newloc);
		//printf("ssss2222222\n");

		if(sp!=NULL)
		{
			//printf("2ssss2222222\n");
			struct DkNode *nroot = (struct DkNode *)malloc(sizeof(struct DkNode));
			nroot->norder = 1;
			//printf("%d\n",root->norder);
			//printf("%s\n",sp->key);
			strcpy(nroot->keys[0],sp->key);
			nroot->locs[0] = pos;
			nroot->locs[1] = sp->loc;
			//printf("qqqqqq2222222\n");
			nroot->nLeaf = false;
			nroot->isRoot = true;
			root->isRoot = false;
			writeOneNode(filename, root, pos);
			long int rootLoc = creatFileNode(filename);
			writeOneNode(filename, nroot, rootLoc);

			//printf("qqq2222111222\n");
			FILE *fp = fopen(filename,"r+");
			fseek(fp,0,SEEK_SET);
			fwrite(&rootLoc,8,1,fp);
			fclose(fp);
			free(sp);
			free(nroot);
		}
		else
		{
			//printf("3ssss2222222\n");
			if(preOrd != root->norder)
			{
				writeOneNode(filename, root, pos);
				oo = root->norder;		

			}
		//	printf("oo is %d %d\n",oo,root->norder);
		}
		//printf("order is %d\n",root->norder);
		free(root);		

	}
}

long int searchRec(char *filename, long int sloc, char *skey)
{
	struct DkNode *root = readOneNode(filename,sloc);
	//printf("%d\n",root->norder);
	//int k = binsearch2(root->keys,root->norder,skey);
	int k;
	for(k=0; k < root->norder; k++)
	{	
		int cmp = strcmp(skey, root->keys[k]);
		if(cmp <= 0)
			break;
	}
	/*if(k==32 && strcmp(skey,"Zpclaz7UX_I\n")==0)
	{
		k=30;
		int q;
		for(q=0;q<=root->norder;q++)
		{
			printf("%ld\n",root->locs[q]);
		}
	}*/
	//printf("%d th loc of %d order\n",k,root->norder);
	//exit(0);

	//int k=i-1;
	//if(k < root->norder -1 && strcmp(root->keys[k+1],skey)==0)
	//	k++;
	if(root->nLeaf)
	{
		struct DkLeaf *leaf = readOneLeaf(filename,root->locs[k]);
		free(root);
		char **indexArr= (char **)malloc(sizeof(char *) * leaf->num);
		int j;
		for(j=0;j < leaf->num; j++)
		{
			indexArr[j] = &(leaf->datum[j].key[0]);
		}
		//int k = binsearch(indexArr,leaf->num,skey);
		for(k=0; k < leaf->num; k++)
		{
			int cmp = strcmp(skey, indexArr[k]);
			if(cmp <= 0)
				break;
		}
		free(indexArr);
		if(strcmp(skey,leaf->datum[k].key)==0)
		{
			long int ret = leaf->datum[k].loc;
			free(leaf);
			return ret;
		}
		else
		{
			/*printf("%s\n",leaf->datum[k-1].key);
			printf("%d\n",k);*/
			//printf("%d\n",leaf->num);
			//printf("*****************************\n");
			int h = 0;
			for(h=0;h<leaf->num;h++)
			{
				//printf("%s",leaf->datum[h].key);
			}
			//printf("*****************************\n");
			//printf("%d %d\n%sooo%sooo\n",k,leaf->num,skey,leaf->datum[k].key);
			free(leaf);
			return -1;
		}
	}
	else
	{
		long int nloc = root->locs[k];
		int m;
		/*for(m = 0; m< root->norder; m++)
		  {
		  printf("%s\n",root->keys[k]);
		  }
		  printf("======================================\n");*/
		
		free(root);
		//if(k>0)
			//printf("%s",root->keys[k-1]);
		//else
			//printf("@@@@@@@@");
		//if(k < root->norder)
			//printf("%s",root->keys[k]);
		//else
			//printf("&&&&&&&&");
		//printf("%d\n",k);
		return searchRec(filename,nloc,skey);
	}
}

long int searchKey(char *tableName, char *skey)
{

	char filename[50];
	strcpy(filename, tableName);
	strcat(filename,"_db_node");

	FILE *fp = fopen(filename,"r+");
	long int rootLoc;
	fread(&rootLoc,8,1,fp);
	fclose(fp);
	if(rootLoc==0)
	{
		int j;
		struct DkLeaf *leaf = readOneLeaf(filename,0);
		char **indexArr= (char **)malloc(sizeof(char *) * leaf->num);
		for(j=0;j < leaf->num; j++)
		{
			indexArr[j] = &(leaf->datum[j].key[0]);
		}
		//int k = binsearch(indexArr,leaf->num,skey);
		int k;
		for(k=0; k < leaf->num; k++)
		{	
			int cmp = strcmp(skey, indexArr[k]);
			if(cmp <= 0)
				break;
		}
		
		//printf("%d th loc\n",k);
		free(indexArr);
		if(strcmp(skey,leaf->datum[k].key)==0)
		{
			long int ret = leaf->datum[k].loc;
			free(leaf);
			return ret;
		}
		else
		{
			//printf("%d %d\n%s\n%s\n",k,leaf->num,skey,leaf->datum[k-1].key);
			free(leaf);
			return -1;
		}
	}
	else
	{
		return searchRec(filename,rootLoc,skey);
	}
}

void updateLoc(char *tableName,char *key, long int newLoc)
{
	char filename[50];
	strcpy(filename, tableName);
	strcat(filename,"_db_node");

	FILE *fp = fopen(filename,"r+");
	long int rootLoc;
	fread(&rootLoc,8,1,fp);

	int k;
	if(rootLoc)
	{
		struct DkNode* root = NULL;
		bool isleaf = false;
		do{
			root = readOneNode(filename,rootLoc);
			k = binsearch2(root->keys,root->norder,key);
			//printf("%s\n",k);
			rootLoc = root->locs[k];
			isleaf = root->nLeaf;
			free(root);
		}while(!isleaf);

		struct DkLeaf *leaf = readOneLeaf(filename,rootLoc);

		int j;
		char **indexArr= (char **)malloc(sizeof(char *) * leaf->num);
		for(j=0;j < leaf->num; j++)
		{
			indexArr[j] = &(leaf->datum[j].key[0]);
		}
		j = binsearch(indexArr, leaf->num, key);
		if(j < leaf->num && strcmp(leaf->datum[j].key,key)==0)
		{
			leaf->datum[j].loc = newLoc;
			writeOneLeaf(filename,rootLoc,leaf);
		}
		//printf("%s %s\n",key,leaf->datum[j].key);
		free(leaf);
		free(indexArr);
	}
	else
	{	
		struct DkLeaf *leaf = readOneLeaf(filename,0);
		int j;
		char **indexArr= (char **)malloc(sizeof(char *) * leaf->num);
		for(j=0;j < leaf->num; j++)
		{
			indexArr[j] = &(leaf->datum[j].key[0]);
		}
		j = binsearch(indexArr, leaf->num, key);
		if(strcmp(leaf->datum[j].key,key)==0)
		{
			leaf->datum[j].loc = newLoc;
			writeOneLeaf(filename,0,leaf);
					//printf("%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
		}
		free(leaf);
		free(indexArr);
	}




}

int main1(int argc,char *argv[])
{
	mtrace();
	/*struct DkNode p;
	  p.nLeaf = false;
	  p.norder = 3;
	  strcpy(p.keys[0],"ytreww");
	  strcpy(p.keys[1],"jhgf");
	  strcpy(p.keys[2],"sssxxx");
	  p.locs[0] = 5;
	  p.locs[1]= 9;
	  p.locs[2] = 17;
	  p.locs[3] = 34;
	  writeOneNode("btree.out",&p,0);
	  struct DkNode *q;
	  q = readOneNode("btree.out",0);
	  printf("%s\n",q->keys[1]);*/
	/*printf("%ld %ld\n",creatFileNode("btree.out"),sizeof(struct DkNode));
	  char *test[5]={"b","c","e","f","p"};
	  printf("%s\n",test[1]);
	  char *c = "a";
	  printf("%d\n",binsearch(test,5,c));
	  createFileLeaf("yt");*/

	FILE *ytf = fopen("youtube2017.0000","r");
	long int y = 0;
	char buffer[409600];
	int len =strlen("url:https://www.youtube.com/watch?v=");
	int n = 0, m = 0;
	long int h1 = searchKey("yt","d4FLLNW8fpg\n");
	updateLoc("yt","d4FLLNW8fpg\n",9987);
	long int h2 = searchKey("yt","d4FLLNW8fpg\n");
	//printf("pre: %ld after :%ld\n",h1,h2);
	for(y=0;y < 250000; y++)
	{
		fgets(buffer,409600,ytf);
		char *ptr = strstr(buffer,"url:https://www.youtube.com/watch?v=");
		if(ptr!=NULL)
		{
			n++;
	//				printf("%d--%s",n,ptr+len);

			long int h = searchKey("yt",ptr+len);
			if((h - y*5))
				printf("777777777777===========7777777777777777777777\n");
			//		printf("%d %ld\n",y*5,searchKey("yt",ptr+len));
			//		break;	

			//		insertKey("yt",ptr+len,y * 5);
		}
	}
	fclose(ytf);

	/*	struct DkNode *child = readOneNode("yt_db_node", 0);
		if(child != NULL)
		printf("%d\n",child->norder);*/
	//struct DkNode *troot = readOneNode("yt_db_node",8);
	//int k = binsearch(troot->keys,troot->norder,"");
	/*printf("%d\n",troot->norder);
	  int i;
	  for(i=0;i<troot->norder;i++)
	  {
	  printf("%s %ld\n",troot->keys[i],troot->locs[i]);
	  struct DkLeaf *tleaf = readOneLeaf("yt_db_node",troot->locs[i]);
	  printf("1%s2%s\n",tleaf->datum[1].key,tleaf->datum[2].key);
	  free(tleaf);
	  }*/
	return 0;
}
