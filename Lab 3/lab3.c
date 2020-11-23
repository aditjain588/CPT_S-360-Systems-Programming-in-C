#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#define MAX 128

char gdir[MAX];    // gdir[ ] stores dir strings
char *dir[64];
int  ndir;

char gpath[MAX];   // gpath[ ] stores token strings
char *name[64];
int  ntoken;
int tk;

int main(int argc, char *argv[], char *env[])
{
  int  i, r,f;
  int  pid, status;
  char *s, cmd[64], line[MAX];

  printf("************* Welcome to Mysh **************\n");
  i = 0;
  int j=0;
  char *token;
  while (env[i]){
    printf("env[%d] = %s\n", i, env[i]);
    
    // Looking for PATH=
    if (strncmp(env[i], "PATH=", 5)==0){
      printf("show PATH: %s\n", env[i]);

      printf("decompose PATH into dir strings in gdir[ ]\n");
      strcpy(gdir, &env[i][5]);
      
      // printf(gdir);
      token=strtok(gdir,":");
      dir[0]=token;   
       j=1;
      while(token!=NULL){
	token=strtok(NULL,":");
	dir[j]=token;
	j=j+1;
	}
      j=0;
       while(dir[j]!=NULL){
	printf("%s", dir[j]);
	printf(" ");
	j=j+1;
	ndir=ndir+1;
	}
       printf("\n");
     
      break;
    }
    i++;
  }

  
  printf("*********** Mysh processing loop **********\n");
  char *ntoken;
  char *filename;
  int tk_input=0;
  int p=0,co=0;
  while(1){
     printf("Mysh % : ");

     fgets(line, 128, stdin);
     line[strlen(line)-1] = 0;      // fgets() has \n at end

     if (line[0]==0)
       continue;
     printf("line = %s\n", line);// print line to see what you got

     int lenght = strlen(line);
       char *head = strtok(line, "|");
       char *tail = NULL;
       
       if(strlen(head) < lenght){
	 // printf("In if blcok \n");
	 tail = line + strlen(head) +1;
       }
       else{
	 //printf("IN else block\n");
	 tail = line + lenght;
       }
     
      ntoken=strtok(line," ");
      name[0]=ntoken;   
       j=1;
      while(ntoken!=NULL){
	ntoken=strtok(NULL," ");
	name[j]=ntoken;
	j=j+1;
	}
       j=0;
       while(name[j]){
	 // printf(" in here\n");
	 printf("name[j]= %s j=%d\n", name[j],j);
	printf(" ");
	tk=0;
	if(strcmp(name[j],">")==0 || strcmp(name[j],">>")==0 ){
	  tk=tk+1;
	  // strcpy(name[j],'\0');
	  name[j]=0;
	  }
	else if(strcmp(name[j],"<")==0){
	  tk_input=tk_input+1;
	  name[j]=0;
	}
	else if(strcmp(name[j],"|")==0){
	  p=p+1;
	} 
	j=j+1;
	
	}
       
       
       // printf("head contains %s\n", head);
       // printf("Tail contains %s\n",tail);
	     			 
       if(strcmp(name[0],"cd")==0){
	 printf("Command found \n");
	 if(name[1]!=NULL){
	 chdir(name[1]);
	 printf("%s Directory changed ok\n", name[1]);
	 // return;
	 } 
       }
       else if(strcmp(name[0], "pwd")==0){
	 printf("%s\n", get_current_dir_name());
	 //return;
       }
       else if(strcmp(name[0], "exit")==0){
	   exit(1);
	 }
   
     pid = fork();   // fork a child sh

     if (pid){
        printf("parent sh %d waits\n", getpid());
        pid = wait(&status);
        printf("child sh %d died : exit status = %04x\n", pid, status);
        continue;
     }
     else{
        printf("child sh %d begins\n", getpid());
        for (i=0; i<ndir; i++){
	  strcpy(cmd, dir[i]); strcat(cmd, "/"); strcat(cmd, name[0]);
	  
	  printf("i=%d cmd=%s\n", i, cmd);
	  if(tk>0){
	    printf("Redirection detected\n");
	    tk=0;
	    printf("tk isss tk=%d\n", tk);
	    close(1);
	    int fd=open("outfile", O_WRONLY|O_CREAT, 0644);
	    r=execve(cmd, name, env);
	  
	  }
	   else if(tk_input>0){
	     printf("Redirection detected\n");
	     tk_input=0;
	     printf("tk_input isss tk_input=%d\n", tk_input);
	     int fd = open("outfile", O_RDONLY);
	     close(0);
	     dup(fd);
	     r = execve(cmd, name, env);
	   }
	  
	  else{
	    r = execve(cmd, name, env);
	  }
	}	   
	printf("cmd %s not found, child sh exit \n", name[0]);
	exit(123);   // die with value 123
     }
  }
  
}

