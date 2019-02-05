#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Node
{
  char *userid;
  int score;
  char *topic;
  struct Node *next;
};

int flag=0;
int flagt=0;

void printList(struct Node *node)                  ///printing of nodes
{
  //printf("Entered printing loop\n");
  while (node != NULL)
  {
     printf("(%s,%s,%d)\n",node->userid,node->topic,node->score);
     node = node->next;
  }
}

void finding(struct Node *node, char *uid2){
    
    while (node!=NULL)
    {
      //printf("PART 1");
      if(strcmp(node->userid,uid2)!=0)
      {
        //printf("PART 2");
        flag++;
        break;
      }
     node = node->next;
     }
     
}
        
void ftopic(struct Node *node, char *top2,int sc){
while (node!=NULL)
    {
      
      if(strcmp(node->topic,top2)==0)
      {
        node->score+=sc;
        flagt++;
        break;
      }
     node = node->next;
     }
     
}

void add(struct Node** head_ref, char *uid1,char *top1,int sc1)
{
   struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
   struct Node *last = *head_ref;  
 
   new_node->userid=malloc(sizeof(char) * (strlen(uid1)));
   strcpy(new_node->userid,uid1);
 
   new_node->topic=malloc(sizeof(char) * (strlen(top1)));
   strcpy(new_node->topic,top1);
    
   new_node->score=sc1;

   new_node->next = NULL;
 
   if (*head_ref == NULL)
    {
       *head_ref = new_node;
       return;
    }  
      
    while (last->next != NULL)
        last = last->next;
  
    
    last->next = new_node; 

    //printf("Done appending");
    return;    
}
void del(struct Node** head_ref)
{
   
   struct Node* current = *head_ref;
   struct Node* next;
 
   while (current != NULL) 
   {
       next = current->next;
       free(current);
       current = next;
   }
   
   *head_ref = NULL;
   flag=0;
   
}

 
int main() {
   struct Node* head = NULL;
   
   
   while(1){
   char s[256];
   flagt=0;
   //printf("Enter the tuple:");
   fgets(s,256,stdin);
   /*if(strcmp(s,NULL)==0)
   {
   break;
   }*/
   if(s[0]=='\n'){                               //end of input
   printList(head);	
   del(&head);
   break;
   }
   
   int p;
   for(int i=0;i<strlen(s); i++)
   {
    if (s[i]==')'){
     p=i;
     break;	
     }
   }
   for(int i=0;i<strlen(s); i++)
        {
             if (s[i]=='(')
             {
              char * uid;
              uid=malloc(sizeof(char) * (5));
              int x=0;
              int y=0;
              int z=0;
              char act;
              int score=0;
              char * temp;
              temp=malloc(sizeof(char) * (5));
              char top[15];
              for(int j=(i+1);j<(i+5);j++)
              {
                 uid[x]=s[j];
                 x++;
                 
              }
              for(int k=(i+6);k<(i+21);k++)
              {
                 top[y]=s[k];
                 y++;
              }
              for(int l=(i+22);l<p;l++){
                  temp[z]=s[l];
		  z++;
              }
              score=atoi(temp);
              finding(head,uid);
              if(flag==0)
              {
              ftopic(head,top,score);
              if (flagt==0)
              {
              add(&head,uid,top,score);
              }
              }
              else
              {
              printList(head);
              del(&head);
              //printf("DONE");
              add(&head,uid,top,score);
              }
              
        }
}}
   return 0;
}


           


	

