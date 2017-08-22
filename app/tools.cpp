#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<iostream>

#define false 0
#define true 1

using namespace std;

class CharacterNode
{
public:
char c;
CharacterNode *next;
CharacterNode()
{
next=NULL;
}
};

class StringBuffer
{
public:
CharacterNode *start,*end;
int characterNodeCount;
StringBuffer()
{
start=end=NULL;
characterNodeCount=0;
}
~StringBuffer()
{
clear();
}
void append(char c)
{
CharacterNode *t;
t=new CharacterNode;
t->c=c;
if(start==NULL)
{
start=end=t;
}
else
{
end->next=t;
end=t;
}
characterNodeCount++;
}
void clear()
{
CharacterNode *t;
while(start!=NULL)
{
t=start;
start=start->next;
delete t;
}
end=NULL;
characterNodeCount=0;
}
char * toString()
{
char *s;
if(start==NULL)
{
s=new char[1];
s[0]='\0';
}
else
{
s=new char[characterNodeCount+1];
int i;
i=0;
CharacterNode *t;
t=start;
while(t!=NULL)
{
s[i]=t->c;
t=t->next;
i++;
}
s[i]='\0';
}
return s;
}
};

class StringBufferNode
{
public:
StringBuffer *sb;
StringBufferNode *next,*previous;
StringBufferNode()
{
next=previous=NULL;
sb=NULL;
}
~StringBufferNode()
{
delete sb;
}
};

class StringBufferCollection
{
public:
StringBufferNode *start,*end;
int stringBufferNodeCount;
StringBufferCollection()
{
start=end=NULL;
stringBufferNodeCount=0;
}
~StringBufferCollection()
{
clear();
}
void clear()
{
StringBufferNode *t;
while(start!=NULL)
{
t=start;
start=start->next;
delete t;
}
end=NULL;
stringBufferNodeCount=0;
}
void add(StringBuffer *sb)
{
StringBufferNode *t;
t=new StringBufferNode;
t->sb=sb;
if(start==NULL)
{
start=end=t;
}
else
{
end->next=t;
t->previous=end;
end=t;
}
stringBufferNodeCount++;
}
StringBuffer * get(int index)
{
if(index<0 || index>=stringBufferNodeCount) 
{
return NULL;
}
StringBufferNode *t;
int i=0;
t=start;
while(i<index)
{
t=t->next;
i++;
}
return t->sb;
}
};

int indexOf(char *s,char *f,int startFromIndex=0)
{
if(startFromIndex<0) return -1;
int i=0+startFromIndex;
int slength=strlen(s);
int flength=strlen(f);
int endIndex=slength-flength;
while(i<=endIndex)
{
if(strncmp(s+i,f,flength)==0) return i;
i++;
}
return -1;
}

int countOccurrences(char *s,char *f)
{
int count=0;
int startFromIndex=0;
while(1)
{
startFromIndex=indexOf(s,f,startFromIndex);
if(startFromIndex==-1) break;
count++;
startFromIndex++;
}
return count;
}

void findAndReplace(char *s,char *f,char *r)
{
int si=0;
int flength=strlen(f);
int rlength=strlen(r);
char *tmp=new char[strlen(s)+1];
while(1)
{
si=indexOf(s,f,si);
if(si==-1) break;
if(flength!=rlength)
{
strcpy(tmp,s+si+flength);
s[si]='\0';
strcat(s,r);
strcat(s,tmp);
}
else
{
strncpy(s+si,r,rlength); // strncpy does not place \0
}
si=si+rlength;
}
delete [] tmp;
}

char **split(char *str,char *separator,int *numberOfSplits)
{
*numberOfSplits=countOccurrences(str,separator)+1;
char **splits=new char *[*numberOfSplits];
int leftIndex,rightIndex,separatorLength;
separatorLength=strlen(separator);
int strLength=strlen(str);
int x;
leftIndex=0;
int req;
int len;
x=0;
while(1)
{
rightIndex=indexOf(str,separator,leftIndex);
if(rightIndex==-1) break;
req=rightIndex-leftIndex+1;
len=req-1;
splits[x]=new char[req];
strncpy(splits[x],str+leftIndex,len);
splits[x][len]='\0';
leftIndex=rightIndex+separatorLength;
x++;
}
req=strLength-leftIndex+1;
len=req-1;
splits[x]=new char[req];
strcpy(splits[x],str+leftIndex);
int countOfNonEmptyStrings=0;
x=0;
while(x<*numberOfSplits)
{
if(splits[x][0]!='\0') countOfNonEmptyStrings++;
x++;
}
if(countOfNonEmptyStrings<*numberOfSplits)
{
int e,f;
char **tsplits=new char *[countOfNonEmptyStrings];
for(e=0,f=0;e<*numberOfSplits;e++)
{
if(splits[e][0]!='\0')
{
tsplits[f]=splits[e];
f++;
}
else
{
delete [] splits[e];
}
}
delete [] splits;
splits=tsplits;
}
*numberOfSplits=countOfNonEmptyStrings;
return splits;
}

void allTrim(char *str)
{
int l;
int r;
r=strlen(str)-1;
while(str[r]==' ' && r>=0)
{
r--;
}
str[r+1]='\0';
l=0;
while(str[l]==' ')
{
l++;
}
if(l>0)
{
strcpy(str,str+l);
}
while(countOccurrences(str,(char *)"  ")>0)
{
findAndReplace(str,(char *)"  ",(char *)" ");
}
}

void rightTrim(char *str)
{
int r;
r=strlen(str)-1;
while(str[r]==' ' && r>=0)
{
r--;
}
str[r+1]='\0';
}
void leftTrim(char *str)
{
int l;
l=0;
while(str[l]==' ')
{
l++;
}
if(l>0)
{
strcpy(str,str+l);
}
}

StringBufferCollection * convertFileToStringBufferCollection(char * str)
{
FILE *f;
f=fopen(str,"r");
if(f==NULL)
{
cout<<"File not found"<<endl;
return NULL;
}
char ch;
StringBuffer *sb;
sb=new StringBuffer;
StringBufferCollection *sbc;
sbc=new StringBufferCollection;
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch=='\r') continue;
if(ch=='\n')
{
StringBufferNode * sbn;
sbn=new StringBufferNode;
sbn->sb=sb;
if(sbc->start==NULL)
{
sbc->start=sbc->end=sbn;
}
else
{
sbc->end->next=sbn;
sbn->previous=sbc->end;
sbc->end=sbn;
}
sb=new StringBuffer;
}
else sb->append(ch);
}
return sbc;
}
