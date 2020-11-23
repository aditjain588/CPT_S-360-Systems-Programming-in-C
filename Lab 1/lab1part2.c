#include<stdio.h>
#include<stdlib.h>

typedef unsigned int u32;

char *ctable = "0123456789ABCDEF";
int BASE=10;

int rpu(u32 x);
void prints(char *s);
int printu(u32 x);
int printd(int x);
int printx(u32 x);
int printo(u32 x);
void myprintf(char*fmt, ...);
int main(int argc, char * argv[ ], char *env[ ])
{
  printf("argc =%d\n" ,argc);
  myprintf("cha=%c string=%s          dec=%d  hex=%x  oct=%o  neg=%d\n",'A'," this is a test",100,    100,      100,     -100 );   
  return 0;
}

int rpu(u32 x)
{
  //  printf(" in rpu \n");
  char c;
  if(x){
    c=ctable[x % BASE];
    rpu(x/BASE);
    putchar(c);
  }
}
void prints(char *s){
  while(*s!='\0'){
    putchar(*s);
    s++;
  }
}
int printu(u32 x)
{
  //  printf("in printu \n");
  (x==0)?putchar('0'):rpu(x);
  putchar(' ');
}

int printd(int x)
{
  // printf("in print d \n");
  BASE=10;
  if(x<0){
    putchar('-');
    rpu(-1*x);
  }
    else if(x>0){
      rpu(x);
    }
}
int printx(u32 x)
{
  //printf("in printx \n");
  BASE=16;
  rpu(x);
}

int printo(u32 x)
{
  // printf("in print o\n");
  BASE=8;
  rpu(x);
}

void myprintf(char *fmt, ...)
{
  // printf(" in myprint f \n");
  char *cp=fmt;
  int *ip=(int *)&fmt;
  ip++;
  while(*cp!='\0'){
  if(*cp=='%'){
    cp++; 
    switch(*cp)
      {
      case 'c':
	putchar(*ip);
	break;
      
      case 'u':
	printu(*ip);
	break;
      case's':
	prints(*ip);
	break;
      case'd':
	printd(*ip);
	break;
      case'o':
	printo(*ip);
	break;
      case'x':
	printx(*ip);
	break;
             	
	}
    ip++;
  }  
    else{
       putchar(*cp);
     }
  cp++;
  }
}
