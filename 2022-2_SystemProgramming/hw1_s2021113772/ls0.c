#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

int namecmp(const void *a, const void *b)
{
   return strcmp(*(char **)a, *(char **)b);
}

int ls(char *fileName)
{
   DIR *pdir;
   struct dirent *pde;
   struct stat buf;

   int i=0;
   int count =0;
   char *dir_name[255];

   memset(dir_name, '\0', sizeof(dir_name));
   memset(&pde, '\0', sizeof(pde));
   memset(&buf, '\0', sizeof(buf));

   if((pdir = opendir(fileName)) < 0)
   {
      perror("opendir");
      return 1;
   }

    printf("%s:\n",fileName);

   while((pde = readdir(pdir)) != NULL)
   {
      if(!strcmp(pde->d_name, ".") || !strcmp(pde->d_name, ".."))
         continue;
      else{
         dir_name[count] = pde->d_name;
         count = count + 1;
      }
   }
   qsort(dir_name, count, sizeof(char *), namecmp);

   for(i=0; i<count; i++)
      printf("%s\t", dir_name[i]);
   printf("\n");
   for(i=0; i<count; i++)
   {
      char *newDirName = (char *)malloc(sizeof(char) * 100);
      strcpy(newDirName, fileName);
      strcat(newDirName, "/");
      strcat(newDirName, dir_name[i]);
      lstat(newDirName, &buf);
      if(S_ISDIR(buf.st_mode) == 1)
         ls(newDirName);
   }

   closedir(pdir);
}

int main(int argc, char *argv[])
{
   if(strcmp(argv[1],"-R"))
   {
      fprintf(stderr, "!!!need -R option!!!\n");

      return 1;
   }

    ls(".");
   
   return 0;
}