#include "tools.cpp"
#include<stack>
class AttributeNode
{
public:
char *name;
char *value;
AttributeNode *next,*previous;
AttributeNode()
{
next=previous=NULL;
name=value=NULL;
}
~AttributeNode()
{
if(name!=NULL) delete [] name;
name=NULL;
if(value!=NULL) delete [] value;
value=NULL;
}
};
class TagNode
{
public:
char *content;
int isTag;
TagNode *parent;
TagNode *next,*previous;
TagNode *start;
TagNode *end;
int childCount;
AttributeNode *attributeStart,*attributeEnd;
int attributesCount;
TagNode()
{
parent=start=end=NULL;
previous=next=NULL;
childCount=0;
content=NULL;
attributeStart=attributeEnd=NULL;
attributesCount=0;
}
~TagNode()
{
TagNode *t;
while(start!=NULL)
{
t=start;
start=start->next;
delete t;
}
end=NULL;
childCount=0;
if(content!=NULL)
{
delete [] content;
}
AttributeNode *at;
while(attributeStart!=NULL)
{
at=attributeStart;
attributeStart=attributeStart->next;
delete at;
}
attributeEnd=NULL;
attributesCount=0;
}
};
class Attribute
{
private:
AttributeNode *attributeNode;
friend class TagDataStructure;
friend class Node;
public:
char * getName()
{
if(attributeNode->name==NULL) return attributeNode->name;
char *m=new char[strlen(attributeNode->name)+1];
strcpy(m,attributeNode->name);
return m;
}
char * getValue()
{
if(attributeNode->value==NULL) return attributeNode->value;
char *m=new char[strlen(attributeNode->value)+1];
strcpy(m,attributeNode->value);
return m;
}
};
class Node
{
private:
TagNode *tagNode;
friend class TagDataStructure;
public:
Node()
{
this->tagNode=NULL;
}
bool isTag()
{
return this->tagNode->isTag;
}
char * getContent()
{
if(this->tagNode->content==NULL) return this->tagNode->content;
char *m=new char[strlen(this->tagNode->content)+1];
strcpy(m,tagNode->content);
return m;
}
int getChildCount()
{
return this->tagNode->childCount;
}
bool hasChildren()
{
return this->tagNode->childCount>0;
}
char * getTagValue()
{
TagNode * t;
t=tagNode;
t=t->start;
return t->content;
}
char * getAttributeValue(char * name)
{
char * value=NULL;
int t=0;
Attribute *attribute;
while(t<getAttributeCount())
{
attribute=getAttribute(t);
if(strcmp(attribute->getName(),name)==0)
{
value=attribute->getValue();
}
t++;
}
return value;
}
Attribute * getAttribute(int i)
{
if(i<0 || i>=this->tagNode->attributesCount) return NULL;
int j=0;
AttributeNode *t;
t=this->tagNode->attributeStart;
while(j<i)
{
t=t->next;
j++;
}
Attribute *a;
a=new Attribute;
a->attributeNode=t;
return a;
}
Node * getChild(int i)
{
if(i<0 || i>=this->tagNode->childCount) return NULL;
Node *node;
TagNode *t=this->tagNode->start;
int m;
for(m=0;m<i;m++)
{
t=t->next;
}
node=new Node;
node->tagNode=t;
return node;
}
int getAttributeCount()
{
return this->tagNode->attributesCount;
}
bool hasAttributes()
{
return this->tagNode->attributesCount>0;
}
};
class TagDataStructureUtility
{
friend class TagDataStructure;
private:
TagDataStructureUtility()
{
}
static int isSlashAtCorrectPosition(char *str)
{
int i=countOccurrences(str,(char *)"/");
if(i==0) return true;
if(i>1) return false;
if(str[1]=='/') return true;
if(str[strlen(str)-2]=='/') return true;
return false;
}
static int isEndTag(char *str)
{
return str[1]=='/';
}
static int isStartAndEndTag(char *str)
{
return str[strlen(str)-2]=='/';
}
static char * getEndTag(char *startTag)
{
char *str=new char[strlen(startTag)+1];
str[0]='<';
str[1]='/';
str[2]='\0';
strcat(str,startTag+1);
return str;
}
static void trimTag(char *str)
{
while(countOccurrences(str,(char *)"< ")>0)
{
findAndReplace(str,(char *)"< ",(char *)"<");
}
while(countOccurrences(str,(char *)" >")>0)
{
findAndReplace(str,(char *)" >",(char *)">");
}
while(countOccurrences(str,(char *)" /")>0)
{
findAndReplace(str,(char *)" /",(char *)"/");
}
if(isEndTag(str))
{
while(countOccurrences(str,(char *)"/ ")>0)
{
findAndReplace(str,(char *)"/ ",(char *)"/");
}
}
else
{
while(countOccurrences(str,(char *)"  ")>0)
{
findAndReplace(str,(char *)"  ",(char *)" ");
}
}
}
static char ** splitNameValuePair(char *str)
{
/*48-57 but not at index 0
65 - 90 anywhere
97 - 122 anywhere
36 for $ anywhere
95 for _ anywhere*/
int x,y,z;
x=0;
while(str[x]!='\0' && str[x]!='=')
{
x++;
}
if(str[x]=='\0')
{
return NULL;
}
if(str[x+1]=='\0')
{
return NULL;
}
char **nv=new char *[2];
nv[0]=new char[x+1];
nv[1]=new char[strlen(str)-x+1];
strncpy(nv[0],str,x);
nv[0][x]='\0';
strcpy(nv[1],str+x+1);
allTrim(nv[0]);
x=strlen(nv[0]);
y=0;
while(y<x)
{
if(!((nv[0][y]>=65 && nv[0][y]<=90) || (nv[0][y]>=97 && nv[0][y]<=122) || nv[0][y]==36 || nv[0][y]==95 || (x>0 && nv[0][y]>=48 && nv[0][y]<=57)))
{
delete [] nv[0];
delete [] nv[1];
delete [] nv;
return NULL;
}
y++;
}
allTrim(nv[1]);
x=strlen(nv[1]);
// 39 for ' and 34 for "
if(nv[1][0]!=39  && nv[1][0]!=34)
{
delete [] nv[0];
delete [] nv[1];
delete [] nv;
return NULL;
}
if(nv[1][x-1]!=39  && nv[1][x-1]!=34)
{
delete [] nv[0];
delete [] nv[1];
delete [] nv;
return NULL;
}
if(nv[1][0]!=nv[1][x-1])
{
delete [] nv[0];
delete [] nv[1];
delete [] nv;
return NULL;
}
int na=(nv[1][0]==39)?39:34;
y=1;
while(y<x-1)
{
if(nv[1][y]==na)
{
delete [] nv[0];
delete [] nv[1];
delete [] nv;
return NULL;
}
y++;
}
char *name=new char[strlen(nv[0])+1];
char *value=new char[strlen(nv[1])+1];
strcpy(name,nv[0]);
strcpy(value,nv[1]);
delete [] nv[0];
delete [] nv[1];
nv[0]=name;
nv[1]=value;
return nv;
}
};
class TagDataStructure
{
private:
TagNode *start;
TagNode *end;
TagNode *current;
bool validStructure;
char *fileName;
int parseFile()
{
AttributeNode *at,*astart,*aend;
char **nv;
char **attributes;
int m,u;
int i;
FILE *f;
f=fopen(fileName,"r");
if(f==NULL)
{
cout<<fileName<<" does not exist"<<endl;
return false;
}
int lessThanHandled=1;
char ch;
int numberOfSplits;
int lineNumber;
int characterNumber;
lineNumber=1;
characterNumber=0;
int isPartOfTag=0;
StringBuffer sb;
char *str,*str2;
while(1)
{
ch=fgetc(f);
if(feof(f)) break;
if(ch=='\r') continue;
if(ch=='\n')
{
lineNumber++;
characterNumber=0;
continue;
}
characterNumber++;
if(ch=='<')
{
if(!lessThanHandled)
{
cout<<"Expected >, found < at "<<lineNumber<<","<<characterNumber<<endl;
return false;
}
lessThanHandled=false;
isPartOfTag=1;
if(sb.characterNodeCount>0)
{
TagNode *t;
t=new TagNode;
t->content=sb.toString();
t->parent=current;
sb.clear();
t->isTag=false;
// logic to debug starts
if(current!=NULL)
{
//cout<<"Append under "<<current->content<<endl;
//cout<<t->content<<endl;
}
else
{
//cout<<"root element"<<current->content<<endl;
//cout<<t->content<<endl;
}
//logic to debug ends
if(current->start==NULL)
{
current->start=current->end=t;
}
else
{
current->end->next=t;
t->previous=end;
current->end=t;
}
current->childCount++;
}
sb.append('<');
continue;
}
if(ch=='>')
{
if(lessThanHandled)
{
cout<<"Expected <, found > at "<<lineNumber<<","<<characterNumber<<endl;
return false;
}
lessThanHandled=true;
isPartOfTag=0;
sb.append('>');
str=sb.toString();
TagDataStructureUtility::trimTag(str);
sb.clear();
if(!TagDataStructureUtility::isSlashAtCorrectPosition(str))
{
cout<<"Incorrect format of tag "<<str<<" at "<<lineNumber<<","<<characterNumber<<endl;
return false;
}
if(TagDataStructureUtility::isStartAndEndTag(str))
{
i=strlen(str);
str[i-2]='>';
str[i-1]='\0';
i--;
if(i==2)
{
cout<<"Invalid start tag <> at "<<lineNumber<<","<<characterNumber-1<<endl;
delete [] str;
return false;
}
astart=aend=NULL;
numberOfSplits=0;
if(countOccurrences(str,(char *)" ")>0)
{
// contains attributes
attributes=split(str,(char *)" ",&numberOfSplits);
//cout<<numberOfSplits<<endl;
strcpy(str,attributes[0]);
m=strlen(str);
str[m]='>';
str[m+1]='\0';
//cout<<str<<endl;
m=strlen(attributes[numberOfSplits-1]);
if(attributes[numberOfSplits-1][m-2]=='/')
{
attributes[numberOfSplits-1][m-2]='\0';
}
else
{
attributes[numberOfSplits-1][m-1]='\0';
}
m=1;
while(m<numberOfSplits)
{
nv=TagDataStructureUtility::splitNameValuePair((char *)attributes[m]);
if(nv==NULL)
{
cout<<"Attributes of "<<attributes[0]<<"> are invalid"<<endl;
return false;
}
at=new AttributeNode;
at->name=nv[0];
at->value=new char[strlen(nv[1])-2+1];
strcpy(at->value,nv[1]+1);
at->value[strlen(at->value)-1]='\0';
delete [] attributes[m];
delete [] nv[1];
if(astart==NULL)
{
astart=aend=at;
}
else
{
aend->next=at;
at->previous=aend;
aend=at;
}
m++;
}
}
TagNode *t;
t=new TagNode;
t->content=str;
t->isTag=true;
t->parent=current;
t->attributeStart=astart;
t->attributeEnd=aend;
t->attributesCount=numberOfSplits-1;
if(start==NULL)
{
start=end=t;
}
else
{
if(current->start==NULL)
{
current->start=current->end=t;
}
else
{
current->end->next=t;
t->previous=end;
current->end=t;
}
current->childCount++;
}
// current=t;   // because an empty tag got created
continue;
}
if(TagDataStructureUtility::isEndTag(str))
{
if(strlen(str)==3)
{
cout<<"Invalid end tag </> at "<<lineNumber<<","<<characterNumber-2<<endl;
delete [] str;
return false;
}
str2=TagDataStructureUtility::getEndTag(current->content);
if(stricmp(str,str2)!=0)
{
printf("%s, contains malformed tags, end tag for %s missing\n",fileName,current->content);
delete [] str2;
delete [] str;
return 0;
}
delete [] str2;
delete [] str;
current=current->parent;
}
else
{
if(strlen(str)==2)
{
cout<<"Invalid start tag <> at "<<lineNumber<<","<<characterNumber-1<<endl;
delete [] str;
return false;
}
astart=aend=NULL;
numberOfSplits=0;
if(countOccurrences(str,(char *)" ")>0)
{
// contains attributes
attributes=split(str,(char *)" ",&numberOfSplits);
//cout<<numberOfSplits<<endl;
strcpy(str,attributes[0]);
m=strlen(str);
str[m]='>';
str[m+1]='\0';
//cout<<str<<endl;
m=strlen(attributes[numberOfSplits-1]);
if(attributes[numberOfSplits-1][m-2]=='/')
{
attributes[numberOfSplits-1][m-2]='\0';
}
else
{
attributes[numberOfSplits-1][m-1]='\0';
}
m=1;
while(m<numberOfSplits)
{
nv=split(attributes[m],(char *)"=",&u);
// need to apply validations
at=new AttributeNode;
at->name=nv[0];
at->value=new char[strlen(nv[1])-2+1];
strcpy(at->value,nv[1]+1);
at->value[strlen(at->value)-1]='\0';
delete [] attributes[m];
delete [] nv[1];
if(astart==NULL)
{
astart=aend=at;
}
else
{
aend->next=at;
at->previous=aend;
aend=at;
}
m++;
}
} 
TagNode *t;
t=new TagNode;
t->content=str;
t->isTag=true;
t->parent=current;
t->attributeStart=astart;
t->attributeEnd=aend;
t->attributesCount=numberOfSplits-1;
if(start==NULL)
{
start=end=t;
}
else
{
if(current->start==NULL)
{
current->start=current->end=t;
}
else
{
current->end->next=t;
t->previous=end;
current->end=t;
}
current->childCount++;
}
current=t;
}
continue;
}
sb.append(ch);
}
}
public:
TagDataStructure(char *fileName)
{
this->fileName=fileName;
start=end=current=NULL;
validStructure=parseFile();
}
~TagDataStructure()
{
if(start!=NULL)
{
delete start;
start=end=current=NULL;
}
}
int isValid()
{
return validStructure;
}

Node * getRootNode()
{
Node *node;
node=new Node;
node->tagNode=this->start;
return node;
}


// for testing only
void traverseDataStructure()
{
int i;
cout<<"******Traversing the data structure*******"<<endl;
int numberOfTabs=0;
char *str;
int x;
TagNode *t,*j;
AttributeNode *at;
j=NULL;
t=start;
while(1)
{
if(t==NULL)
{
if(j==NULL) break;
str=TagDataStructureUtility::getEndTag(j->content);
numberOfTabs--;
for(i=1;i<=numberOfTabs;i++)
{
cout<<"\t";
}
cout<<str<<endl;
delete [] str;
t=j;
j=j->parent;
t=t->next;
if(t==NULL) continue;
}
for(i=1;i<=numberOfTabs;i++)
{
cout<<"\t";
}
cout<<t->content<<endl;
if(t->isTag)
{
numberOfTabs++;
at=t->attributeStart;
if(at!=NULL)
{
cout<<"Number of attribute : "<<t->attributesCount<<endl;
}
while(at!=NULL)
{
cout<<"("<<at->name<<","<<at->value<<")  ";
at=at->next;
if(at==NULL)
{
cout<<endl;
}
}
j=t;
t=t->start;
}
else
{
t=t->next;
}
}
}
}; // Class TagDataStructure ends





