#include "cppjieba/include/cppjieba/Jieba.hpp"
#include<string>
#include<cstring>
#include<cstdlib>

#define _BOOL_

#include "writeData.h"
#include "record2.h"
#include "Bplus.h"

using namespace std;

const char* const DICT_PATH = "cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "cppjieba/dict/stop_words.utf8";

struct node{
	long int offset;
	long int recLoc;
	char  *ikey;
	struct node *nxt;
};

struct node *hash[22180];

void insert(char *key,long int recLoc,long int offset)
{
	if((char unsigned)key[0] >= 0x8F && (char unsigned)key[0] <= 0xff && (char unsigned)key[1]>=0x40 &&(char unsigned)key[1]<=0xff)
	{
		int idx = ((char unsigned)key[0] - 0x8F)*192 + ((char unsigned)key[1] - 0x40);
		struct node *ptr;
		ptr = (struct node *)malloc(sizeof(struct node));
		ptr->ikey = strdup(key);
		ptr->offset = offset;
		ptr->recLoc = recLoc;
		ptr->nxt = hash[idx];
		hash[idx] = ptr;
	}
	else
	{
		char key1 = tolower(key[0]), key2 = tolower(key[1]);
		if(key1<='z' && key1 >='a' && key2>='a' &&key2 <='z')
		{
			int idx = (key1 - 'a')*25 + (key2 - 'a')+21504;
			struct node *ptr;
			ptr = (struct node *)malloc(sizeof(struct node));
			ptr->ikey = strdup(key);
			ptr->offset = offset;
			ptr->recLoc = recLoc;
			ptr->nxt = hash[idx];
			hash[idx] = ptr;
		}

	}

}

void init(char *tablename)
{
	int i;
	for(i=0;i<22180;i++)
	{
		hash[i] = NULL;
	}
	FILE *fd = fopen(tablename,"r");
	if(fd==NULL)
		return;
	int len,recLoc,offset;
	while(fread(&len,4,1,fd)==1){
		//fread(&len,4,1,fd);
		//	cout<<"length:"<<len<<endl;
		char *buf = (char *)malloc(len+1);
		fread(buf,1,len,fd);
		buf[len] = '\0';
		//cout<<buf<<endl;
		fread(&recLoc,8,1,fd);
		fread(&offset,8,1,fd);
		insert(buf,recLoc,offset);
		free(buf);
	}
	fclose(fd);
}

void store(char *tablename)
{
	int i;
	FILE *fd = fopen(tablename,"w+");
	for(i=0;i<22180;i++)
	{
		struct node *ptr = hash[i];
		while(ptr!=NULL)
		{
			//printf(	
			int len = strlen(ptr->ikey);
			fwrite(&len,4,1,fd);
			fwrite(ptr->ikey,1,len,fd);
			fwrite(&ptr->recLoc,8,1,fd);	
			fwrite(&ptr->offset,8,1,fd);
			struct node *pre = ptr;
			ptr = ptr->nxt;
			cout<<"xxxyyy"<<endl;
		//	cout<<pre->ikey<<endl;
			if(pre->ikey!=NULL)
				free(pre->ikey);
			cout<<"xxxyyy"<<endl;
			free(pre);
		}
	}
	fclose(fd);
}
struct node *getDoc(char *key)
{
	int idx; 
	char key1 = tolower(key[0]), key2 = tolower(key[1]);
	if((char unsigned)key[0] >= 0x8F && (char unsigned)key[0] <= 0xff && (char unsigned)key[1]>=0x40 &&(char unsigned)key[1]<=0xff)
	{
		idx = ((char unsigned)key[0] - 0x8F)*192 + ((char unsigned)key[1] - 0x40);
	}
	else if(key1<='z' && key1 >='a' && key2>='a' && key2 <='z')
	{
		idx = (key1 - 'a')*25 + (key2 - 'a')+21504;
	}
	else
		return NULL;	
	struct node *ptr = hash[idx];

	while(ptr!=NULL)
	{
		if(strcmp(ptr->ikey,key)==0)
			return ptr;
		ptr = ptr->nxt;
	}
	return NULL;
}

void demo(char *tablename)
{
	for(int i=0;i<22180;i++)
	{
		struct node *ptr = hash[i];
		while(ptr!=NULL)
		{
			cout<<i<<" "<<ptr->ikey<<endl;
			ptr = ptr->nxt;
		}
	}
}

struct node **sret(char *key,int &num)
{
	
	int idx; 
	num = 0;
	char key1 = tolower(key[0]), key2 = tolower(key[1]);
	if((char unsigned)key[0] >= 0x8F && (char unsigned)key[0] <= 0xff && (char unsigned)key[1]>=0x40 &&(char unsigned)key[1]<=0xff)
	{
		idx = ((char unsigned)key[0] - 0x8F)*192 + ((char unsigned)key[1] - 0x40);
	}
	else if(key1<='z' && key1 >='a' && key2>='a' && key2 <='z')
	{
		idx = (key1 - 'a')*25 + (key2 - 'a')+21504;
	}
	else
		return NULL;	
	struct node *qtr = hash[idx];
	
	while(qtr!=NULL)
	{
		if(strcmp(qtr->ikey,key)==0)
			num++;
		qtr = qtr->nxt;
		//cout<<qtr->ikey<<endl;
	}

	struct node **ret = (struct node **)malloc(sizeof(struct node *)*num);
	struct node *ptr = hash[idx];
	//cout<<"qqq"<<endl;
	int i=0;
	while(ptr!=NULL)
	{
		if(strcmp(ptr->ikey,key)==0)
		{
			ret[i++] = ptr;
			//cout<< ret[i-1]->ikey <<endl;
		}
		ptr = ptr->nxt;
	}
	//cout<<"111"<<endl;
	
	return ret;
}

int cmp(const void *a,const void*b)
{
	if((*(struct node **)a)->recLoc==(*(struct node **)b)->recLoc)
		return (*(struct node **)a)->offset - (*(struct node **)b)->offset;
	return (*(struct node **)a)->recLoc - (*(struct node **)b)->recLoc;
}

char *str_replace(char *orig, char *rep, char *with) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count
    if (!with)
        with = "";
    len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

result = (char *)malloc(strlen(orig) + (len_with - len_rep) * count + 1);
    tmp = result;
    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
}
int main(int argc, char** argv) 
{
	char tablename[100];
	strcpy(tablename,argv[1]);
	strcat(tablename,"_text");
	init(tablename);
	/*cppjieba::Jieba jieba(DICT_PATH,
			HMM_PATH,
			USER_DICT_PATH,
			IDF_PATH,
			STOP_WORD_PATH);
	vector<string> words;
	string s;
	string result;*/
	

	//jieba.Cut(argv[2], words, true);

	/*int cnt = words.size();
	int pos=0;
	for(int i=0;i<cnt;i++)
	{
		char *cstr = (char *)malloc(sizeof(char) * (words[i].length() + 1));
		strcpy(cstr, words[i].c_str());
		cout<<words[i]<<endl;

		insert(cstr,atoi(argv[3]),pos);
		pos = pos + strlen(cstr);
		struct node *ptr = getDoc(cstr);
		delete [] cstr;
	}
	//demo(tablename);
	store(tablename);*/
	int num;
	long int pre=-1,cur=0,cn = atoi(argv[3]);
	struct node **arr = sret(argv[2],num);
	qsort(arr,num,sizeof(struct node *),cmp);

	char hl[100];
	strcpy(hl,"<hl>");
	strcat(hl,argv[2]);
	strcat(hl,"</hl>");
	for(int i=0;i<num;i++)
	{
		cout<<i<<endl;
		cur = i;
		if(cur != pre)
		{
			printf("<p>");
			struct record *rec = readRec("yt_rec", arr[i]->recLoc,1);
			//cout<<rec->offset<<endl;
			if(!rec->del)
			{
				char *retd = readData("yt_data",rec->offset,rec->length);
				char *out = str_replace(retd,argv[2],hl);
				printf("%s\n",out);
				free(retd);
				free(out);
			}
			printf("</p>");
			cn--;
		}
		if(!cn)
			break;
		pre = cur;
		//cout<<arr[i]->recLoc<<endl;
	}	
	return 0;
}

int main2()
{
			cppjieba::Jieba jieba(DICT_PATH,
					HMM_PATH,
					USER_DICT_PATH,
					IDF_PATH,
					STOP_WORD_PATH);
	FILE *ytf = fopen("youtube.txt","r");
	long int y = 0;
	char buffer[409600];
	char writeBuffer[2000000];
	char title[102400];
	char content[409600];
	int len =strlen("@url:https://www.youtube.com/watch?v=");
	strcpy(writeBuffer,"\0");
	long int n =0,lp=0;
	long int maxl = 0;
	init("yt_text");
	//demo("1");
	//return 0;
	for(y=0;; y++)
	{
		if(fgets(buffer,409600,ytf)==NULL)
			break;

		char *ptr = strstr(buffer,"@url:https://www.youtube.com/watch?v=");
		if(strstr(buffer,"@title:")!=NULL)
		{
			strcpy(title,buffer+7);
		}
		if(strstr(buffer,"@content:")!=NULL)
		{
			strcpy(content,buffer+9);
		}

		if(ptr!=NULL && y)
		{
			struct record inRec;
			char *qtr = strstr(ptr+len,"&");
			if(qtr!=NULL)
			{
				strncpy(inRec.key,ptr,qtr-ptr);
				inRec.key[qtr-ptr]='\0';
			}
			else
			{
				strcpy(inRec.key,ptr + len);
			}
			if(maxl < strlen(inRec.key))
			{
				maxl = strlen(inRec.key);
			}
			long int sl  = sizeof(struct record) * n;

			if(sl==-1){
				printf("%ld %s\n",sl,inRec.key);
				exit(1);
				lp++;
			}
			vector<string> words;
			string s;
			string result;

			jieba.Cut(title, words, true);

			int cnt = words.size();
			int pos=0;
			cout<<"q"<<endl;
			for(int i=0;i<cnt;i++)
			{
		
				char *cstr = (char *)malloc(sizeof(char) * (words[i].length() + 1));
				strcpy(cstr, words[i].c_str());
				cout<<words[i]<<endl;
				cstr[words[i].length()] = '\0';
				insert(cstr,sl,pos);
				pos = pos + strlen(cstr);
			//	struct node *ptr = getDoc(cstr);
				cout<<"sssssss"<<endl;
				delete [] cstr;
				cout<<"pppssss"<<endl;
			}
			if(words.size()>0)
				words.clear();
			jieba.Cut(content, words, true);

                        cnt = words.size();
                        pos=0;
                        for(int i=0;i<cnt;i++)
                        {
                                char *cstr = (char *)malloc(sizeof(char) * (words[i].length() + 1));
                                strcpy(cstr, words[i].c_str());
                         //       cout<<"ooo"<<words[i]<<"yyy"<<endl;
				cstr[words[i].length()] = '\0';
				//if(strcmp(
                                insert(cstr,sl,pos);
                                pos = pos + strlen(cstr);
			//	cout<<"sssssss"<<endl;
                                //struct node *ptr = getDoc(cstr);
                                delete [] cstr;
			//	cout<<"pppssss"<<endl;
                        }
			if(words.size()>0)
				words.clear();

			//demo(tablename);


			strcpy(writeBuffer,buffer);

			n++;
			if(n==1500000)
				break;
			continue;
		}
		if(strcmp(buffer,"\n")==0 || strcmp(buffer,"@\n")==0)
			continue;
		strcat(writeBuffer,buffer);
	}
	demo("yt_test");
	store("yt_text");
	fclose(ytf);

	return 0;
}
