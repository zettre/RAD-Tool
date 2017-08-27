#include "TagDataStructure.cpp"
#include <stack>

using namespace std;

class DTOConfiguration
{
private:
friend class DTOGenerator;
friend class GeneratorUtility;
char *authorName;
char *targetFolder;
DTOConfiguration()
{
authorName=NULL;
targetFolder=NULL;
}
};//DTOConfiguration ends





class GeneratorUtility
{
private:
GeneratorUtility()
{
}
static DTOConfiguration *dtoConfiguration;
static DTOConfiguration * getDTOConfiguration()
{
if(dtoConfiguration==NULL)
{
// logic to read contents of dto.cfg and create object
// of DTOConfiguration, assign (authorname and targetfolder)
TagDataStructure *tagDataStructure;
tagDataStructure=new TagDataStructure((char *)"..\\conf\\dto.cfg");
dtoConfiguration=new DTOConfiguration;
Node *rootNode;
rootNode=tagDataStructure->getRootNode();
stack<pair<Node*,int> > nodes;
pair<Node*,int> p;
Node *vNode;
p=make_pair(rootNode,0);
nodes.push(p);
int t,x,i,flag;
while(!nodes.empty())
{
p=nodes.top();
nodes.pop();
vNode=p.first;
i=p.second;
flag=0;
while(1)
{
Node *nn;
if(flag==1)
{
x=0;
flag=0;
}
else
{
x=i;
}
while(x<vNode->getChildCount())
{
nn=vNode->getChild(x);
if(nn->isTag())
{

if(strcmp(nn->getContent(),(char*)"<author>")==0)
{
dtoConfiguration->authorName=nn->getAttributeValue((char*)"name");
}
if(strcmp(nn->getContent(),(char*)"<target>")==0)
{
dtoConfiguration->targetFolder=nn->getAttributeValue((char*)"name");
}
p=make_pair(vNode,x+1);
nodes.push(p);
vNode=nn;
flag=1;
break;
}
else
{

}
x++;
}
if(flag==0)
{
break;
}
}
}
}
return dtoConfiguration;
}
friend class DTOGenerator;
};//GeneratorUtility ends


DTOConfiguration * GeneratorUtility::dtoConfiguration=NULL;


class Interface
{
char * packageName;
char ** packagesToImport;
int packagesToImportCount;
char * interfaceName;
char ** extendedInterfaces;
int extendedInterfacesCount;
char ** properties;
char ** dataTypes;
int propertiesCount;
public:
void setPackageName(char * packageName)
{
this->packageName=packageName;
}
char * getPackageName()
{
return this->packageName;
}
void setPackagesToImport(char ** packagesToImport,int packagesToImportCount)
{
this->packagesToImport=packagesToImport;
this->packagesToImportCount=packagesToImportCount;
}
char ** getPackagesToImport(int * packagesToImportCount)
{
*packagesToImportCount=this->packagesToImportCount;
return this->packagesToImport;
}
void setInterfaceName(char * interfaceName)
{
this->interfaceName=interfaceName;
}
char * getInterfaceName()
{
return this->interfaceName;
}
void setExtendedInterfaces(char ** extendedInterfaces,int extendedInterfacesCount)
{
this->extendedInterfaces=extendedInterfaces;
this->extendedInterfacesCount=extendedInterfacesCount;
}
char ** getExtendedInterfaces(int * extendedInterfacesCount)
{
*extendedInterfacesCount=this->extendedInterfacesCount;
return this->extendedInterfaces;
}
void setProperties(char ** properties,char** dataTypes,int propertiesCount)
{
this->properties=properties;
this->dataTypes=dataTypes;
this->propertiesCount=propertiesCount;
}
char ** getProperties(char*** dataTypes,int * propertiesCount)
{
*propertiesCount=this->propertiesCount;
*dataTypes=this->dataTypes;
return this->properties;
}
};//Interface ends


class Class
{
char * packageName;
char ** packagesToImport;
int packagesToImportCount;
char * className;
char * interfaceName;
char ** properties;
char ** dataTypes;
int propertiesCount;
char * compareValue;
char * equalsValue;
public:
void setPackageName(char * packageName)
{
this->packageName=packageName;
}
char * getPackageName()
{
return this->packageName;
}
void setPackagesToImport(char ** packagesToImport,int packagesToImportCount)
{
this->packagesToImport=packagesToImport;
this->packagesToImportCount=packagesToImportCount;
}
char ** getPackagesToImport(int * packagesToImportCount)
{
*packagesToImportCount=this->packagesToImportCount;
return this->packagesToImport;
}
void setClassName(char * className)
{
this->className=className;
}
char * getClassName()
{
return this->className;
}
void setInterfaceName(char * interfaceName)
{
this->interfaceName=interfaceName;
}
char * getInterfaceName()
{
return this->interfaceName;
}
void setProperties(char ** properties,char** dataTypes,int propertiesCount)
{
this->properties=properties;
this->dataTypes=dataTypes;
this->propertiesCount=propertiesCount;
}
char ** getProperties(char*** dataTypes,int * propertiesCount)
{
*propertiesCount=this->propertiesCount;
*dataTypes=this->dataTypes;
return this->properties;
}
void setCompareValue(char * compareValue)
{
this->compareValue=compareValue;
}
char * getCompareValue()
{
return this->compareValue;
}
void setEqualsValue(char * equalsValue)
{
this->equalsValue=equalsValue;
}
char * getEqualsValue()
{
return this->equalsValue;
}

};//Class ends


class DTOGenerator
{
DTOConfiguration *dtoConfiguration;
int numberOfFiles;
char **files;
TagDataStructure **dataStructures;
char ** getListOfDTOFiles()
{
system("dir ..\\data\\*.dto /b > ..\\data\\dto.data");
StringBufferCollection * sbc;
StringBufferNode *sbn;
sbc=convertFileToStringBufferCollection((char *)"..\\data\\dto.data");
char ** str;
str=new char*[sbc->stringBufferNodeCount];
int i=0;
sbn=sbc->start;
while(sbn!=NULL)
{
str[i]=new char[sbn->sb->characterNodeCount+1];
str[i]=sbn->sb->toString();
sbn=sbn->next;
i++;
}
numberOfFiles=i;
return str;
}
void populateDataStructures()
{
if(numberOfFiles==0) return;
dataStructures=new TagDataStructure *[numberOfFiles];
int x=0;
while(x<numberOfFiles)
{
dataStructures[x]=new TagDataStructure(files[x]);
x++;
}
}


Interface** getInterfaces()
{
// logic to traverse the data structure and extract info about
// interface and its properties and methods, put it neatly in an
// object of class Interface and then return base address of an
// array of type Interface *
Interface ** interfaces=new Interface *[numberOfFiles];
int pi=0;
while(pi<numberOfFiles)
{
interfaces[pi]=new Interface;
Node * node;
node=dataStructures[pi]->getRootNode();

node=node->getChild(0);
int x=0;
Node* nn;
while(x<node->getChildCount())
{
nn=node->getChild(x);
if(nn->isTag())
{
	if(strcmp(nn->getContent(),(char*)"<interface-package>")==0)
	{
	interfaces[pi]->setPackageName(nn->getTagValue());
	}
	if(strcmp(nn->getContent(),(char*)"<interface-imports>")==0)
	{
		char** pack;
		int pn;
		if(nn->hasChildren())
		{
			int e=0;
			Node* en;
			pn=nn->getChildCount();
			pack=(char**)malloc(sizeof(char*)*pn);
			while(e<pn)
			{
				en=nn->getChild(e);
				if(en->isTag())
				{
					if(strcmp(en->getContent(),(char*)"<interface-import>")==0)
					{
						pack[e]=en->getTagValue();
					}
				}
				e++;
			}		
		}
	interfaces[pi]->setPackagesToImport(pack,pn);
	}
	if(strcmp(nn->getContent(),(char*)"<interface-name>")==0)
	{
		interfaces[pi]->setInterfaceName(nn->getTagValue());
	}
	if(strcmp(nn->getContent(),(char*)"<interface-extends>")==0)
	{
		char** pack;
		int pn;
		if(nn->hasChildren())
		{
			int e=0;
			Node* en;	
			pn=nn->getChildCount();
			pack=(char**)malloc(sizeof(char*)*pn);
			while(e<pn)
			{
				en=nn->getChild(e);
				if(en->isTag())
				{
					if(strcmp(en->getContent(),(char*)"<interface-extend>")==0)
					{
						pack[e]=en->getTagValue();
					}
				}
				e++;
			}		
		}
	interfaces[pi]->setExtendedInterfaces(pack,pn);
	}
	if(strcmp(nn->getContent(),(char*)"<properties>")==0)
	{
		char** pack;
		char** dack;
		int pn;
		if(nn->hasChildren())
		{
			int e=0;
			Node* en;	
			pn=nn->getChildCount();
			pack=(char**)malloc(sizeof(char*)*pn);
			dack=(char**)malloc(sizeof(char*)*pn);
			while(e<pn)
			{
				en=nn->getChild(e);
				if(en->isTag())
				{
					Node* eb;
					if(strcmp(en->getContent(),(char*)"<property>")==0)
					{
						if(en->hasChildren())
						{
							int c=0;
							while(c<en->getChildCount())
							{
								eb=en->getChild(c);
								if(strcmp(eb->getContent(),(char*)"<name>")==0)
								{
									pack[e]=eb->getTagValue();
								}
								if(strcmp(eb->getContent(),(char*)"<data-type>")==0)
								{
									dack[e]=eb->getTagValue();
								}
								c++;
							}
						}

					}
				}
				e++;
			}
		}
		interfaces[pi]->setProperties(pack,dack,pn);
	}
}
x++;
}
pi++;
}
return interfaces;
}


void createInterfaces()
{
Interface** interfaces=getInterfaces();
int i=0,size,j;
char ** ar;
char** br;
while(i<numberOfFiles)
{
	char * fileName=concatenate(dtoConfiguration->targetFolder,interfaces[i]->getInterfaceName());
	fileName=concatenate(fileName,(char*)".java");
	FILE* f=fopen(fileName,"w");
	fputs("package ",f);
	fputs(interfaces[i]->getPackageName(),f);
	fputs(";\n",f);
	j=0;
	ar=interfaces[i]->getPackagesToImport(&size);
	while(j<size)
	{
		fputs("import ",f);
		fputs(ar[j],f);
		fputs(";\n",f);
		j++;
	}
	fputs("public interface ",f);
	fputs(interfaces[i]->getInterfaceName(),f);
	fputs(" extends ",f);
	j=0;
	ar=interfaces[i]->getExtendedInterfaces(&size);
	while(j<size)
	{
		if(strcmp(ar[j],(char*)"Comparable")==0)
		{
			fputs(ar[j],f);
			fputc('<',f);
			fputs(interfaces[i]->getInterfaceName(),f);
			fputc('>',f);			
		}
		else
		{
			fputs(ar[j],f);			
		}
		if(j!=size-1)
		{
			fputc(',',f);
		}
		j++;
	}
	fputs("\n{\n",f);
	ar=interfaces[i]->getProperties(&br,&size);
	j=0;
	char c;
	char *s;
	while(j<size)
	{
		fputs("public void set",f);
		c=toupper(ar[j][0]);
		fputc(c,f);
		s=substring(ar[j],1,strlen(ar[j])-1);
		fputs(s,f);
		fputc('(',f);
		fputs(br[j],f);
		fputc(' ',f);
		fputs(ar[j],f);
		fputs(");\n",f);
		fputs("public ",f);
		fputs(br[j],f);
		fputs(" get",f);
		fputc(c,f);
		fputs(s,f);
		fputs("();\n",f);
		j++;
	}
	fputc('}',f);
	fclose(f);
	i++;
}

}

Class** getClasses()
{
Class ** classes=new Class *[numberOfFiles];
int pi=0;
while(pi<numberOfFiles)
{
classes[pi]=new Class;
Node * node;
node=dataStructures[pi]->getRootNode();

node=node->getChild(0);
int x=0;
Node* nn;
while(x<node->getChildCount())
{
nn=node->getChild(x);
if(nn->isTag())
{
	if(strcmp(nn->getContent(),(char*)"<interface-name>")==0)
	{
		classes[pi]->setInterfaceName(nn->getTagValue());
	}
	if(strcmp(nn->getContent(),(char*)"<properties>")==0)
	{
		char** pack;
		char** dack;
		int pn;
		if(nn->hasChildren())
		{
			int e=0;
			Node* en;	
			pn=nn->getChildCount();
			pack=(char**)malloc(sizeof(char*)*pn);
			dack=(char**)malloc(sizeof(char*)*pn);
			while(e<pn)
			{
				en=nn->getChild(e);
				if(en->isTag())
				{
					Node* eb;
					if(strcmp(en->getContent(),(char*)"<property>")==0)
					{
						if(en->hasChildren())
						{
							int c=0;
							while(c<en->getChildCount())
							{
								eb=en->getChild(c);
								if(strcmp(eb->getContent(),(char*)"<name>")==0)
								{
									pack[e]=eb->getTagValue();
								}
								if(strcmp(eb->getContent(),(char*)"<data-type>")==0)
								{
									dack[e]=eb->getTagValue();
								}
								c++;
							}
						}

					}
				}
				e++;
			}
		}
		classes[pi]->setProperties(pack,dack,pn);
	}
}
x++;
}
node=dataStructures[pi]->getRootNode();

node=node->getChild(1);
x=0;
while(x<node->getChildCount())
{
nn=node->getChild(x);
if(nn->isTag())
{
	if(strcmp(nn->getContent(),(char*)"<class-package>")==0)
	{
		classes[pi]->setPackageName(nn->getTagValue());
	}
	if(strcmp(nn->getContent(),(char*)"<class-imports>")==0)
	{
		char** pack;
		int pn;
		if(nn->hasChildren())
		{
			int e=0;
			Node* en;
			pn=nn->getChildCount();
			pack=(char**)malloc(sizeof(char*)*pn);
			while(e<pn)
			{
				en=nn->getChild(e);
				if(en->isTag())
				{
					if(strcmp(en->getContent(),(char*)"<class-import>")==0)
					{
						pack[e]=en->getTagValue();
					}
				}
				e++;
			}		
		}
	classes[pi]->setPackagesToImport(pack,pn);
	}
	if(strcmp(nn->getContent(),(char*)"<class-name>")==0)
	{
		classes[pi]->setClassName(nn->getTagValue());
	}
	if(strcmp(nn->getContent(),(char*)"<equals-value>")==0)
	{
		classes[pi]->setEqualsValue(nn->getTagValue());
	}
	if(strcmp(nn->getContent(),(char*)"<compare-value>")==0)
	{
		classes[pi]->setCompareValue(nn->getTagValue());
	}
}
x++;
}

pi++;
}
return classes;
}
void createImplementations()
{
Class** classes=getClasses();
int i=0,size,j;
char ** ar;
char** br;
while(i<numberOfFiles)
{
	char * fileName=concatenate(dtoConfiguration->targetFolder,classes[i]->getClassName());
	fileName=concatenate(fileName,(char*)".java");
	FILE* f=fopen(fileName,"w");
	fputs("package ",f);
	fputs(classes[i]->getPackageName(),f);
	fputs(";\n",f);
	j=0;
	ar=classes[i]->getPackagesToImport(&size);
	while(j<size)
	{
		fputs("import ",f);
		fputs(ar[j],f);
		fputs(";\n",f);
		j++;
	}
	fputs("public class ",f);
	fputs(classes[i]->getClassName(),f);
	fputs(" implements ",f);
	fputs(classes[i]->getInterfaceName(),f);
	fputs("\n{\n",f);
	ar=classes[i]->getProperties(&br,&size);
	j=0;
	while(j<size)
	{
		fputs("private ",f);
		fputs(br[j],f);
		fputc(' ',f);
		fputs(ar[j],f);
		fputs(";\n",f);
		j++;
	}
	j=0;
	char c;
	char *s;
	while(j<size)
	{
		fputs("public void set",f);
		c=toupper(ar[j][0]);
		fputc(c,f);
		s=substring(ar[j],1,strlen(ar[j])-1);
		fputs(s,f);
		fputc('(',f);
		fputs(br[j],f);
		fputc(' ',f);
		fputs(ar[j],f);
		fputs(")\n{\nthis.",f);
		fputs(ar[j],f);
		fputc('=',f);
		fputs(ar[j],f);
		fputs(";\n}\n",f);
		fputs("public ",f);
		fputs(br[j],f);
		fputs(" get",f);
		fputc(c,f);
		fputs(s,f);
		fputs("()\n{\n",f);
		fputs("return this.",f);
		fputs(ar[j],f);
		fputs(";\n}\n",f);
		j++;
	}
	fputs("public boolean equals(Object object)\n{\nif(!(object instanceof ",f);
	fputs(classes[i]->getClassName(),f);
	fputs(")) return false;\n",f);
	fputs(classes[i]->getInterfaceName(),f);
	fputc(' ',f);
	c=tolower(classes[i]->getInterfaceName()[0]);
	fputc(c,f);
	s=substring(classes[i]->getInterfaceName(),1,strlen(classes[i]->getInterfaceName())-1);
	fputs(s,f);
	fputs("=(",f);
	fputs(classes[i]->getInterfaceName(),f);
	fputs(")object;\nreturn this.",f);
	fputs(classes[i]->getEqualsValue(),f);
	fputs("==",f);
	fputc(c,f);
	fputs(s,f);
	fputs(".get",f);
	c=toupper(classes[i]->getEqualsValue()[0]);
	fputc(c,f);
	s=substring(classes[i]->getEqualsValue(),1,strlen(classes[i]->getEqualsValue())-1);
	fputs(s,f);
	fputs("();\n}\npublic int compareTo(",f);
	fputs(classes[i]->getInterfaceName(),f);
	fputc(' ',f);
	c=tolower(classes[i]->getInterfaceName()[0]);
	fputc(c,f);
	s=substring(classes[i]->getInterfaceName(),1,strlen(classes[i]->getInterfaceName())-1);
	fputs(s,f);
	fputs(")\n{\nreturn this.",f);
	fputs(classes[i]->getCompareValue(),f);
	fputs(".compareTo(",f);
	fputc(c,f);
	fputs(s,f);
	fputs(".get",f);
	c=toupper(classes[i]->getCompareValue()[0]);
	fputc(c,f);
	s=substring(classes[i]->getCompareValue(),1,strlen(classes[i]->getCompareValue())-1);
	fputs(s,f);
	fputs("());\n}\n",f);
	fputc('}',f);
	fclose(f);
	i++;
}
}
public:
DTOGenerator()
{
dtoConfiguration=GeneratorUtility::getDTOConfiguration();
numberOfFiles=0;
dataStructures=NULL;
if(dtoConfiguration!=NULL)
{
files=getListOfDTOFiles();
int x=0;
char* folder=(char*)"..\\data\\";
while(x<numberOfFiles)
{
	files[x]=concatenate(folder,files[x]);
	x++;
}
populateDataStructures();
createInterfaces();
createImplementations();
}
}
~DTOGenerator()
{
int x;
if(dtoConfiguration!=NULL)
{
delete dtoConfiguration;
}
if(files!=NULL)
{
x=0;
while(x<numberOfFiles)
{
if(files[x]!=NULL) delete [] files[x];
x++;
}
delete [] files;
}
}
void traverseAll()
{
int x=0;
while(x<numberOfFiles)
{
//dataStructures[x]->traverseDataStructure();
x++;
}
}
};//DTOGenerator ends


//traverse using recursion


void traverse(Node *node)
{
/*cout<<"is tag"<<node->isTag()<<endl;
cout<<"content "<<node->getContent()<<endl;
cout<<"child count "<<node->getChildCount()<<endl;
cout<<"has children "<<node->hasChildren()<<endl;
cout<<"attribute count "<<node->getAttributeCount()<<endl;
cout<<"has attributes "<<node->hasAttributes()<<endl;*/
int x;
x=0;
Attribute *attribute;
while(x<node->getAttributeCount())
{
attribute=node->getAttribute(x);
cout<<"Attribute name : "<<attribute->getName()<<endl;
cout<<"Attribute value : "<<attribute->getValue()<<endl;
x++;
}
x=0;
Node *nn;
while(x<node->getChildCount())
{
nn=node->getChild(x);
if(nn->isTag())
{
traverse(nn);
}
else
{
cout<<nn->getContent()<<endl;
}
x++;
}
}




//traverse using stack


void traverseUsingStack(Node *node)
{
cout<<"****************************************************************"<<endl;
stack<pair<Node*,int> > nodes;
pair<Node*,int> p;
Node *vNode;                      //traversing head outside the loop
cout<<node->getContent()<<endl;
int t=0;
Attribute *attribute;
while(t<node->getAttributeCount())
{
attribute=node->getAttribute(t);
cout<<"Attribute name : "<<attribute->getName()<<endl;
cout<<"Attribute value : "<<attribute->getValue()<<endl;
t++;
}
p=make_pair(node,0);
nodes.push(p);
int x,i,flag;
while(!nodes.empty())
{
p=nodes.top();
nodes.pop();
vNode=p.first;
i=p.second;
flag=0;
while(1)
{
Node *nn;
if(flag==1)
{
x=0;
flag=0;
}
else
{
x=i;
}
while(x<vNode->getChildCount())
{
nn=vNode->getChild(x);
if(nn->isTag())
{
cout<<nn->getContent()<<endl;
t=0;
Attribute *attribute;
while(t<nn->getAttributeCount())
{
attribute=nn->getAttribute(t);
cout<<"Attribute name : "<<attribute->getName()<<endl;
cout<<"Attribute value : "<<attribute->getValue()<<endl;
t++;
}

p=make_pair(vNode,x+1);
nodes.push(p);
vNode=nn;
flag=1;
break;
}
else
{
cout<<nn->getContent()<<endl;
}
x++;
}
if(flag==0)
{
break;
}
}
}
}





int main()
{

/*TagDataStructure *tagDataStructure;
tagDataStructure=new TagDataStructure((char *)"dto.cfg");
if(tagDataStructure->isValid())
{
cout<<"File parsed"<<endl;
//tagDataStructure->traverseDataStructure();
Node *rootNode;
rootNode=tagDataStructure->getRootNode();
//traverse(rootNode);
traverseUsingStack(rootNode);
delete tagDataStructure;
}
else
{
cout<<"Cannot parse file"<<endl;
}*/

DTOGenerator *dtoGenerator;
dtoGenerator=new DTOGenerator();
delete dtoGenerator;
return 0;
}