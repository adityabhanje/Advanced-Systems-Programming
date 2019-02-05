#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct profile {
   char  * usid;
   char  action;
   char  * topic;
   int   sc;
}prof;



int main()
{
    FILE * file;
    long fsize;
    file = fopen("./input.txt","r");
    if(file != NULL){

        //get file size
        fseek(file,0,SEEK_END);
        fsize = ftell(file);
        rewind(file);

        // print
        char * s;
        int res=0;
        s = malloc(fsize);
        fgets(s,fsize,file);
        //puts(s);
	for(int i=0;i<strlen(s); i++)
        {
             if (s[i]=='(')
             {
              res++;
             }
        }
        //printf("%d\n",res);
        prof users;
        int z=0;
        for(int i=0;i<strlen(s); i++)
        {
             if (s[i]=='(')
             {
              char uid[4];
              int x=0;
              int y=0;
              char act;
              int score=0;
              char top[15];
              for(int j=(i+1);j<(i+5);j++)
              {
                 uid[x]=s[j];
                 x++;
                 
              }
              //printf("usid: %s\n",uid);
              act=s[i+6];
              for(int k=(i+8);k<(i+23);k++)
              {
                 top[y]=s[k];
                 y++;
              }
              //printf("action: %c\n",act);
              //printf("topic :%s\n",top);
              if(act=='P')
              {
                 score=50;
              }
              if(act=='L')
              {
                 score=20;
              }
              if(act=='D')
              {
                 score=-10;
              }
              if(act=='C')
              {
                 score=30;
              }
              if(act=='S')
              {
                 score=40;
              }
              //printf("Score :%d\n",score);
              users.usid = malloc(sizeof(char) * (strlen(uid) + 1));
              strcpy(users.usid,uid);
              users.action=act;
              users.topic = malloc(sizeof(char) * (strlen(top) + 1));
              strcpy(users.topic,top);
              users.sc=score;
              printf("(%s,%s,%d)\n",users.usid,users.topic,users.sc);
              
                      
              }
              
        
        
    }
    /*for(int a=0;a<res;a++)
    {
      printf("User id :%s\n",users[a].usid);
      printf("Action: %c\n",users[a].action);
      printf("Topic: %s\n",users[a].topic);
    }*/

   }
    else{
        printf("open failure\n");
    }
printf("\n");
fclose(file);

    return 0;
}
