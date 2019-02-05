#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 

#include<stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>


struct tup {
  
  char user[5];
  char field[16];
  int points;
};

struct tup buffer[5000]={0};
struct tup **sharedbuff;
sem_t *full;  
sem_t *empty;
sem_t *latch;
int *count, *objProd, *objCon,*done;
int *sizebuff,*userids;
char* fn;
void reducer(int param);
void mapper();

FILE *fptr;


int main(int argc, char *argv[]) {


int i=0;
count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
*count=0;	
sizebuff= mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
userids= mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
*userids= atoi(argv[2]);
*sizebuff=atoi(argv[1]);

sharedbuff= (struct tup **)mmap(NULL, sizeof(struct tup *) * (*userids), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

*sharedbuff = (struct tup *)mmap(NULL, sizeof(struct tup) * (*userids) * *(sizebuff), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

objProd= (int *)mmap(NULL, sizeof(int)*(*userids), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
done= (int *)mmap(NULL, sizeof(int)*100, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);	

objCon= (int *)mmap(NULL, sizeof(int)*(*userids), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);		

	

full = (sem_t *)mmap(NULL, (*userids) * sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);	

empty = (sem_t *)mmap(NULL, (*userids) * sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);	
	

latch = (sem_t *)mmap(NULL, (*userids) * sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);	

    for(i = 0; i < *userids; i++)
        sharedbuff[i] = (*sharedbuff + (*sizebuff) * i);

   for(i=0; i< *userids; i++)
	{
	objProd[i]=0;
	objCon[i]=0;
	}

pthread_t mapper_thread[1];
pthread_t reducer_threads[*userids];




for(i=0;i< *userids;i++)
{
sem_init(&latch[i],1,1);
sem_init(&full[i],1,0);
sem_init(&empty[i],1,0);
}

switch (fork())
{ 
 case 0:
 mapper();
}

for(i=0;i< *userids;i++)
{
 switch (fork())
  {
    case 0:
 	reducer(i);
  }
 
}

}


void reducer(int param)
{
  int thrdid;
  thrdid=param;
  const char a[4] = "()";
  const char dlim[2] = ",";
  char *token;
  struct tup itup[10000]={0};
  int j = 0, i = 0,k = 0, aval = 0;
  char *user;
  char *field;
  char *points;
  
  int index = 0,bindex=0,bindex_prod=0,iprocess=0;

   while(1)
   {
	
	sem_wait(&latch[thrdid]);
	while((objProd[thrdid]-objCon[thrdid])==0)
	{
	     	
	sem_post(&latch[thrdid]);
	sem_wait(&empty[thrdid]);
	sem_wait(&latch[thrdid]);
	
	}
	while ((objProd[thrdid]-objCon[thrdid])>0) 
	{ 
    user = sharedbuff[thrdid][bindex].user;
    field = sharedbuff[thrdid][bindex].field;
    points = sharedbuff[thrdid][bindex].points;
	for(i=0;i<4;i++)
    itup[bindex_prod].user[i]=sharedbuff[thrdid][bindex].user[i];
    for(i=0;i<15;i++)
    {
    	itup[bindex_prod].field[i]=sharedbuff[thrdid][bindex].field[i];
    }
    itup[bindex_prod].points=sharedbuff[thrdid][bindex].points;

    
    index++;
    bindex++;
    bindex_prod++;  
    objCon[thrdid]++;
    }

    iprocess=index;

		
 	
		
    for(i=0;i<iprocess;i++)
    {
     for(k=i+1;k<iprocess;k++)
     { 
        if((strcmp(itup[i].user, itup[k].user)==0)&& (strcmp(itup[i].field, itup[k].field)==0))
          {   
              aval=itup[i].points+itup[k].points;
              itup[i].points=aval;
              
              for(j=k;j<iprocess-1;j=j+1)
              {   
               itup[j]=itup[j+1];
              }

              iprocess=iprocess-1;
			  bindex_prod--;	  
              k=k-1;
			  index --;
          }    

       
    } 

    }
	printf("\n");

    for (i = 0; i < iprocess; i = i +1 )
    {
      printf("\n%s:\t(%s,%s,%d)\n", itup[i].user, itup[i].user, itup[i].field, itup[i].points);
    }	
		
	objCon[thrdid]=0;
	bindex=0;
	objProd[thrdid]=0;
	if(done[thrdid]==1)
	{
	sem_post(&latch[thrdid]);
	break;
	}


    sem_post(&full[thrdid]); 
    sem_post(&latch[thrdid]);
 
    }
    pthread_exit(NULL);

}

void mapper()
{
    const char a[4] = "()";
    const char dlim[2] = ",";
    char *token;
    FILE *fp;
    int j=0,i,it=0,iter=0,counter=0, gsize=*(sizebuff),bsize=0;   
    char *wtok[500];
    char *eword[500];
    char *user[500],*field[500],*action[500],*uid[400];
    char input[5000];
    struct tup buffer[5000]={0};
    fgets(input, 5000, stdin);
    token = strtok(input, a);
    while( token != NULL ) 
    {
      wtok[j++] = token;
      token = strtok(NULL, a);
    }
    for( i = 0; i <j; i=i+2)
    {
        it=0;
        token = strtok(wtok[i], dlim);
        while( token != NULL )
         {
          eword[it++] = token;
          token = strtok(NULL, dlim);
         } 
	
	      user[counter]=eword[0];
	      field[counter]=eword[2];
	      action[counter]=eword[1];
	
	counter++;
	}
	it=0;
	for(i=0;i<counter;i++)
	{
	 for (j=0; j<i; j++)
	 {
	   if (strcmp(user[j], user[i])==0)
               break;	
	 }
	   if (i == j)
         { 
	 uid[it]=user[i];
	 it++;

	}
	}
	

	for(i=0;i< *userids;i++)
	 {
	bsize=0;
	for(j=0;j<counter;j++)
	 {
         if(strcmp(uid[i], user[j])==0)
	  { 
	    sem_wait(&latch[i]);
	   if(objProd[i]==(*sizebuff))
	   { 
		sem_post(&empty[i]); 
		sem_post(&latch[i]);
		sem_wait(&full[i]);
		sem_wait(&latch[i]);	
		objProd[i]=0;
		bsize=0;

	    }

        if(objProd[i] < gsize)
		{
	     objProd[i]=objProd[i]+1;	
	     for(iter=0;iter<4;iter++)
         sharedbuff[i][bsize].user[iter]=user[j][iter];	  
         for(iter=0;iter<15;iter++)
         sharedbuff[i][bsize].field[iter]=field[j][iter];
	     if(strcmp("P", action[j])==0)
	     sharedbuff[i][bsize].points=50;
    	 if(strcmp("L", action[j])==0)
	     sharedbuff[i][bsize].points=20;
    	if(strcmp("D", action[j])==0)
	    sharedbuff[i][bsize].points=-10;	
     	if(strcmp("C", action[j])==0)
	    sharedbuff[i][bsize].points=30;
      	if(strcmp("S", action[j])==0)
	    sharedbuff[i][bsize].points=40;

        bsize++; 
	    sem_post(&latch[i]); 
	
        }
	 
	 }
	 }
	sem_post(&empty[i]);
	done[i]=1;
	 }
   	 pthread_exit(NULL);
}
