#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>

typedef struct node{
  char name[64];
  char type;
  struct node *child, *sibling, *parent;
}NODE;

NODE *root, *cwd, *start;
char command[16], pathname[64];

char gpath[128];
char *name[64];
int n;

char dname[64], bname[64];

//             0       1       2     3     4        5    6       7    8
char *cmd[]={"mkdir","rmdir","cd", "ls", "quit","save","pwd","creat","rm", 0};

int findCmd(char *command)
{
  int i=0;
  while(cmd[i]){
    if(strcmp(command, cmd[i])==0)
      return i;
    i++;
  }
  return -1;
}

NODE *search_child(NODE *parent, char *name)
{
  NODE *p;
  printf("search for %s in parent DIR\n", name);
  p=parent->child;
  if(p==0)
    return 0;
  while(p){
    if(strcmp(p->name, name)==0)
      return p;
    p=p->sibling;
  }
  return 0;
}

int insert_child(NODE *parent, NODE *q)
{
  NODE *p;
  printf("insert NODE %s into parent child list\n", q->name);
  p=parent->child;
  if(p==0)
    parent->child=q;
  else{
    while(p->sibling)
      p=p->sibling;
    p->sibling=q;
  }
  q->parent=parent;
  q->child=0;
  q->sibling=0;
}
void cd(char *name)
{
  NODE *p, *q;
  if(strcmp(name,"/../..")==0){
    start=root;
    cwd=root;
    printf("current directory root ok\n");
    printf("-------------------------\n");
    return;
  }
  int i=0;
  if(name[0]=='/')
    start=root;
  else
    start=cwd;
  for(i=0;i<strlen(name);i++){
    int j=0;
    char address[10]="";
    while(name[i]!='/' && i<strlen(name)){
      address[j]=name[i];
      j++;
      i++;
      p=search_child(start, address);
    }
  if(p){
    start=p;
    cwd=p;
    printf("current directory %s ok\n", name);
    printf("-------------------------\n");
  }
  else{
    printf("invalid path %s ok\n", name);
    printf("-------------------------\n");
  }
}
}

int mkdir(char *name)
{
  NODE *p, *q;
  int i=0;
  printf("mkdir: name=%s\n", name);

  if(!strcmp(name, "/") || !strcmp(name, ".") || !strcmp(name, "..")){
    printf("can't mkdir with %s\n", name);
  }

  if(name[0]=='/')
    start=root;
  else
    start=cwd;

   if((name[0]=='/') && (name[1]!=NULL)){
    printf("invalid path\n", name);
    printf("mkdir failed\n");
    return -1;
  }
  
 
  printf("check whether %s already exists\n", name);
  p=search_child(start, name);
  if(p){
    printf("name %s already exists, mkdir FAILED\n", name);
    return -1;
  }
  printf("----------------------------\n");
  printf("ready to mkdir %s\n", name);
  q = (NODE *)malloc(sizeof(NODE));
  q->type = 'D';
  strcpy(q->name, name);
  insert_child(start, q);
   
  printf("mkdir %s ok\n", name);
  printf("----------------------------\n");
  
  return 1;
}
int creat(char *name)
{
  NODE *p, *q;
  printf("file: name=%s\n", name);

  if(!strcmp(name, "/") || !strcmp(name, ".") || !strcmp(name, "..")){
    printf("can't create file with %s \n", name);
    return;
  }

  if(name[0]=="/")
    start=root;
  else
    start=cwd;
  
  p=search_child(start,name);
  if(p){
    printf("name %s alredy exists\n", name);
    return -1;
  }
  printf("--------------------------\n");
  printf("ready to create file %s\n");
  q=(NODE *)malloc(sizeof(NODE));
  q->type='F';
  strcpy(q->name, name);
  insert_child(start, q);
  printf("file created %s ok\n", name);
  printf("-----------------\n");

  return 1;
}
int rmdir(char *name)
{
  NODE *p, *curr;
  char nullStr[20] = {"\0"};
  if(name[0]=='/')
    start=root;
  else
    start=cwd;

  printf("check whether %s  exists\n", name);
  curr=search_child(start, name);
  if (curr){
    printf("name %s to be rmdir Found\n ", name);
    if(curr->child==NULL &&  curr->sibling==NULL){
      free(curr);
      strcpy(curr->name, nullStr);
      curr->type=0;
      printf("rmdir ok\n");
      printf("------------------------\n");
    }
      /*** p=curr->parent;
      q=p->child;
      if(strcmp(q->name,name)==0){
	  q->parent=NULL;
	  p->child=NULL;***/
    else{
      printf("node not empty rmdir failed\n");
      return -1;
    }
  }		 
  else{
    printf("name %s to be removed not Found, rmdir failed\n", name);
    return -1;
  }
}

int ls()
{
  NODE *p = cwd->child;
  printf("cwd contents = \n ");
  while(p){
    printf("%c  %s \n ", p->type, p->name);
    p=p->sibling;
  }
  printf("\n");
}
int rpwd(NODE *node)
{
  if(strcmp(node->name, "/")){
    rpwd(node->parent);
    printf("%s/", node->name);
  }
  else printf("%s", node->name);
}
int pwd()
{
  printf("current directory is \n");
  rpwd(cwd);
}

int rm(char *name)
{
  NODE *p, *curr;
  char nullStr[20] = {"\0"};
  if(name[0]=="/")
    start=root;
  else
    start=cwd;

  printf("check whether %s exists\n", name);
  curr=search_child(start, name);
  if(curr){
    printf("name %s to be rm found\n", name);
    free(curr);
    strcpy(curr->name, nullStr);
    curr-> type=0;
    printf("rm ok\n");
    printf("----------------------\n");
  }
  else{
    printf("name %s to be removed not found, rm failed\n", name);
    printf("----------------------\n");
    return -1;
  }
}

int saveTree(NODE *starting, FILE *pFile){
  if(starting!=NULL){
    NODE *temp;
    fprintf(pFile,"%c\t",starting->type);
    char *pathList[16];
    int c=0;
    temp=starting;
    while(temp!=root){
      pathList[c]=strcat(temp->name,"/");
      temp=temp->parent;
      c++;
    }
    c--;
    while(c>=0){
      fprintf(pFile,"%s",pathList[c]);
      c--;
    }
    fprintf(pFile,"\n");
    if(starting -> sibling !=NULL){
      saveTree(starting -> sibling,pFile);
    }
    if(starting->child!=NULL){
      saveTree(starting->child, pFile);
    }
  }
  else{
    printf("empty tree");
  }
  return 0;
}

int save(){
  FILE *pFile;
  pFile=fopen("input.txt","w");

  if(pFile==NULL){
    printf("file cannot be opned\n");
    return 0;
  }
  else{
    printf("file opened\n");
  }
  if(root->child !=NULL){
    saveTree(root->child,pFile);
  }
  fclose(pFile);
  printf("file closed\n");
  return 0;
  }

int quit()
{
  printf("program exist\n");
  exit(0);
}

int initialize()
{
  root=(NODE *)malloc(sizeof(NODE));
  strcpy(root->name, "/");
  root->parent=root;
  root->sibling=0;
  root->child=0;
  root->type='D';
  cwd=root;
  printf("Root initialized OK\n");
}

int main()
{
  int index;
  char line[128];

  initialize();

  printf("NOTE: commands = [mkdir|creat|rmdir|rm|cd|save|pwd|ls|quit]\n");

  while(1){
    printf("Enter command line : ");
    fgets(line, 128, stdin);
    line[strlen(line)-1]=0;

    sscanf(line, "%s %s", command, pathname);
    printf("command=%s pathname=%s\n", command, pathname);

    if(command[0]==0)
      continue;

    index = findCmd(command);

    switch (index){
    case 0: mkdir(pathname); break;
    case 1: rmdir(pathname);  break;
    case 2: cd(pathname);  break;
    case 3: ls(); break;
    case 4: quit(); break;
    case 5: save();break;
    case 6: pwd(); break;
    case 7: creat(pathname); break;
    case 8: rm(pathname); break;
    }
  }
}
     

