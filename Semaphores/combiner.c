#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

static sem_t latch;
static sem_t full;
static sem_t empty;
int done ;
int objprod = 0;
int objcon = 0;

typedef struct shared{
	char ** entries;
	char user[5];
	int obj;
}shared1;

shared1 sharedbuff[50];

typedef struct profile{
	char ** field;
	int fieldc;
	int points[1000];
	char nameid[5];
}profile2;

profile2 prof[50];

int sizebuff;
int userids;


void *producer(void *id){

	long thrdid = (long) id;
	printf("entered producer %ld\n", thrdid+1);
	FILE *fp  = fopen("input.txt", "r");
    char t;
    done = 0;
	char entry[30];
	char tmp[3];
	char unid[3];
	int count=1;
	char fentry[26];
	char q = fgetc(fp);
    if (fp == NULL)
        {
              printf("Error! Could not open file\n");
              exit(-1);
        }
	int q1=0;
	fseek(fp, 0, SEEK_SET);

	while(!feof(fp))
	{
		q = fgetc(fp);
		if (q == ')')
		{
			q1++;
		}
	}

	fseek(fp, 0, SEEK_SET);
	
    for(int j=0; j<=q1; j++)
    {
    	int index=0;
		while(1)
		{
			q=fgetc(fp);
			if (q == '(')
			{
				break;
			}
		}
		fread(entry, 22, 1, fp);
    	for(int x=0; x<=23; x++)
    	{
       		if(x>=0 && x<4)
			{
				fentry[x+1]=entry[x];
				unid[x]=entry[x];	
			}
			if(x==5)
			{
				t=entry[x];	
			}
			if(x>=7 && x<=22)
			{
				fentry[x-1]=entry[x];	
			}		
		}
		fentry[0]='(';
		fentry[5]=',';
		fentry[21]=',';
		if(t=='S')
		{
    	strcpy(tmp,"40");
    	}
        else if(t=='P')
        {
    	strcpy(tmp,"50");
    	} 
        else if(t=='L')
        {
    	strcpy(tmp,"20");
    	} 
        else if(t=='D')
        {
    	strcpy(tmp,"-10");
    	} 
        else if(t=='C')
        {
    	strcpy(tmp,"30");
    	}
    	else
    	{
    	strcpy(tmp,"\0");
        }
		for(int j=0; j<3; j++)
		{
			fentry[22+j]=tmp[j];
		}
		if (tmp[0]=='-')
		{
			fentry[25]=')';
			fentry[26]='\0';
		}
		else
		{
			fentry[24]=')';
			fentry[25]='\0';
		}
		unid[4]='\0';
	
		if (j==q1-1)
		{
			done=1;
		}
		sem_wait(&latch);
		
		for(int x=0; x<userids; x++){
			if(strcmp(unid, sharedbuff[x].user)==0){
				index=x;
				break;
			}
		}
		while (sharedbuff[index].obj==sizebuff) { 
      		printf("producer %ld waiting on full\n",thrdid+1);
      		sem_post(&latch);
       		sem_wait(&full);
       		sem_wait(&latch);

    	} 
		for(int x=0; x<userids; x++){
			if(strcmp(unid,sharedbuff[x].user)!=0){
				if(sharedbuff[x].user[0]=='\0'){
					strcpy(sharedbuff[x].user,unid);
					break;
				}
				continue;
			}
			else
			{
				break;
			}
		}

		for(int x=0; x<userids; x++){
			if(strcmp(unid, sharedbuff[x].user)==0){
				for(int e=0; e<sizebuff; e++){
					if(sharedbuff[x].entries[e][0]=='\0'){
						strcpy(sharedbuff[x].entries[e],fentry);
						objprod++;
						sharedbuff[x].obj++;
						break;
					}
				}
				break;
			}
		}
		if (done) {
       		printf("producer %ld is exiting\n",thrdid+1);
       		sem_post(&empty);
       		sem_post(&latch);
       		return NULL;
    	}
		sem_post(&empty);
		sem_post(&latch);
				
    }
    fclose(fp);
    done=1;
    return NULL;
}


void *consumer(void *id)
{
	long thrdid = (long)id;
	printf("entered thread %ld\n", thrdid+1);
    while(1)
    {			
    			
    			sem_wait(&latch);
				while (sharedbuff[thrdid].obj==0 && !done) {
       				printf("consumer %ld waiting on empty\n",thrdid+1);
       				sem_post(&latch);
       				sem_wait(&empty);
       				sem_wait(&latch);
       			
    			}
    			if (sharedbuff[thrdid].obj==0 && done) {
      				printf("consumer %ld exiting\n", thrdid+1);
      				sem_post(&empty);
      				sem_post(&latch);
      				
       				return NULL;  
    			}
    			sem_post(&latch);
    			
			
			for (int i=0; i<sizebuff ; i++){
				char topic[16];
				char points[3];
				char uid1[5];
				char entry[30];
				int consumed=0;
				sem_wait(&latch);
				
				
				if(sharedbuff[thrdid].obj!=0){
					strcpy(entry, sharedbuff[thrdid].entries[i]);
					sharedbuff[thrdid].obj--;
					objcon++;
					free(sharedbuff[thrdid].entries[i]);
					sharedbuff[thrdid].entries[i]=malloc(28*sizeof(char));
					strcpy(uid1, sharedbuff[thrdid].user);
					consumed=1;
					sem_post(&full);
					sem_post(&empty);
					
				}
				
				sem_post(&latch);
				if(consumed){
					for(int x=0; x<=(strlen(entry)); x++)
					{
						if(x>=6 && x<=20)
						{
							topic[x-6]=entry[x];	
						}

						if(find(entry))
						{	
							if(x>=22 && x<=24)
							{
								points[x-22]=entry[x];	
							}
						}
						else
						{
							if(x>=22 && x<=23)
							{
								points[x-22]=entry[x];	
							}
						}			
					}
					if(!find(entry)){
						points[2]='\0';
					}
					strcpy(prof[thrdid].nameid, uid1);
					for(int x=0; x<1000; x++){
						if(strcmp(prof[thrdid].field[x],topic)==0){
							prof[thrdid].points[x]+=atoi(points);
							break;
						}
						else if(prof[thrdid].field[x][0]=='\0'){
							strcpy(prof[thrdid].field[x],topic);
							prof[thrdid].fieldc++;
							prof[thrdid].points[x]=atoi(points);
							break;
						}
					}


				}

			}
		
    }
    return NULL;
}

int main(int argc, char *argv[])
{
	sizebuff=atoi(argv[1]);
	userids=atoi(argv[2]);
	if(sizebuff<userids&&sizebuff!=0){
		sizebuff=userids;
	}
	pthread_t threadprod;
	pthread_t thrd[userids];
	void *status;
	pthread_attr_t attr;
	sem_init(&latch, 0, 1);
	sem_init(&full, 0, 0);
	sem_init(&empty, 0, 0);
	for(int i=0; i<userids; i++){
		sharedbuff[i].entries = malloc(sizebuff*sizeof(char *));
		sharedbuff[i].obj=0;
	}
	for(int i=0; i<userids; i++){
		for(int j=0; j<sizebuff; j++){
			sharedbuff[i].entries[j] = malloc(28*sizeof(char));
		}
	}
	for(int i=0; i<userids; i++){
    	prof[i].field = malloc(3000*sizeof(char *));
    }
    for(int i=0; i<userids; i++){
		for(int j=0; j<1000; j++){
			prof[i].field[j] = malloc(16*sizeof(char));
		}
	}
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&threadprod, &attr, producer, (void *)0);
	for(int i=0;i<userids;i++)
  	{
    	pthread_create(&thrd[i], &attr, consumer, (void *)i);; 
    }

    pthread_join(threadprod, &status);
	for(int i=0;i<userids;i++) {
  		pthread_join(thrd[i], &status);
  	}

    printf("The final output after reading all the input tuples are:\n");
	for(int i=0; i<userids; i++){
		for(int j=0; j<prof[i].fieldc; j++){
			printf("(%s,%s,%d) \n", prof[i].nameid,prof[i].field[j],prof[i].points[j]);
		}
	}
    
	pthread_attr_destroy(&attr);
    
    return 0;
    pthread_exit(NULL);
}

int find(char entry[30])
{
	for(int i=0; i<strlen(entry); i++)
	{ 
		if(entry[i]=='-')
		{
			return i;
		}
	}
	return 0;
}




