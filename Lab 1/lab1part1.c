#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<ext2fs/ext2_fs.h>
#include<sys/types.h>
#include<unistd.h>

typedef unsigned char u8;
typedef unsigned char u16;
typedef unsigned char u32;

struct partition {
  u8 drive;
  u8 head;
  u8 sector;
  u8 cylinder;
  u8 sys_type;
  u8 end_head;
  u8 end_sector;
  u8 end_cylinder;
  u32 start_sector;
  u32 nr_sectors;
};

char *dev ="vdisk";
int fd;

int read_sector(int fd, int sector, char *buf)
{
  int n;
  lseek(fd, sector*512, SEEK_SET);
  n=read(fd,read,512);
  if(n<=0){
    printf("read failed \n");
    return -1;
  }
  return n;
 }
int main()
{
  int i=1;
  struct partition *p;
  char buf[512];
  fd = open(dev, O_RDONLY);

  p=(struct partition *)(&buf[0x1be]);

   printf("%8d %8d %8x\n", p->start_sector, p->nr_sectors, p->sys_type);

   //    printf("start_sector\t", "sys_type\t","nr_Sectors\t","end_sectors\t");
    while(i<5){
    printf("%d \t" ,p->start_sector);
    printf("%8x \t",p->sys_type);
    printf("%d \t", p->nr_sectors);
    printf("%u \t" ,p->start_sector+p->nr_sectors);
    printf("\n");
    p++;
    i++; }
    printf("p4 start_sector =%d\n", p->start_sector);
    read_sector(fd, p->start_sector ,buf);

    p=(struct partition *)&buf[0x1BE];

    printf("FIRST entry of localMBR\n");
    printf("start_sector=%d, nsectors=%d\n", p->start_sector, p->nr_sectors);

}
/***void reading_partition(struct partitions *p)
{
  struct partitions *n;
  printf("start_sector =%u\n",p->start_sector);
  printf("sys_type=%d\n" ,p->sys_type);
  printf("nr_sectors=%u\n ",p->nr_sectors);
  printf("end_sector=%u\n",p->end_sector);
  }***/
