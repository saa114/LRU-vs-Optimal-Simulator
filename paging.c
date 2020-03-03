#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include<unistd.h> 

struct  frame {
     int value;
     int fifo;
     struct frame *next;
};

void optimal(struct frame *page,int refrence[], int frames);
void lru(struct frame *page,int refrence[], int frames);
void printPage(struct frame *page);
void reset(struct frame *page);
int longest(int val, int refrence[], int index);
int isHit(struct frame *page, int val);
int isEmpty(struct frame *page);

int main()
{   
    int refrence[20];
    int frames;
    int page_fault = 0;
    struct frame *page = NULL;
    struct frame *page2 = NULL;

    srand(time(0));
    
    int i;
    for (i = 0; i < 20; ++i)
    {   
       refrence[i] = (rand() % 5) + 1;
       printf("%d " ,refrence[i]);
    }
    
    printf("\n\nPlease enter the number of frames. Must be from 1-5: \n");
    scanf("%d", &frames);
    
    if (frames < 1 || frames > 5)
    {
	printf("ERROR: Number of frames must be 1-5!!!\n");
	return -1;
    }

    printf("Number of frames: %d\n", frames);

    i = 0;
    while(i < frames)
    {
       struct frame *new_frame = (struct frame*) malloc(sizeof(struct frame));
       struct frame *new_frame2 = (struct frame*) malloc(sizeof(struct frame));

       
       if (page == NULL)
       {
           page = new_frame;
	   new_frame->value = -1;

	   page2 = new_frame2;
           new_frame2->value = -1;

       }

       else
       {   
          new_frame->value = -1;
          new_frame->next = page;
          page = new_frame;

	  new_frame2->value = -1;
          new_frame2->next = page2;
          page2 = new_frame2;
       }
       
       ++i;
    }
     
     
    lru(page,refrence,frames);
    optimal(page2,refrence,frames);
}


void optimal(struct frame *page,int refrence[], int frames)
{
   int i;
   int distance = 0;
   int farthest = 0;
   int page_faults = 0;
   int found = 1;
   int fifo;
   struct frame *pre, *cur;
   
   printf("\nUSING OPTIMAL ALGORITHIM..\n");

   for (i = 0; i < 20; ++i)
   {    

	pre = page;
	cur = page;
	farthest = 0;
	printf("\nPage number: %d\n", refrence[i]);
          
	if (!isHit(page, refrence[i]))
	{   
	    ++page_faults;
            printf("\nIt's a miss\n");
            
	    while(cur != NULL)
	    {
                if (isEmpty(page))
		{    
                     while (cur->value != -1)
			    cur = cur->next;
		      pre = cur;
                     
		     break;
		}

		else
		{
                   distance = longest(cur->value, refrence,i+1);

	           if (farthest < distance)
		   {    
			farthest = distance;
		        pre = cur;
	           }

		   else if (farthest == distance)
		   {
                        if(pre->fifo > cur->fifo)
				pre = cur;
		   }

		}

		cur = cur->next;
            }
 	    
	    pre->value = refrence[i];
	    pre->fifo = i;

        }
	
	else
	{
           printf("\nIt's a hit\n");
	}

        printPage(page);        
   }

   printf("\nTotal page faults: %d \n", page_faults);
}

void lru(struct frame *page, int refrence[], int frames)
{  
   int page_fault = 0;
   int i;

   printf("\nUSING LRU ALGORITHIM.. \n");

   struct frame *pre = NULL , *cur = NULL;

   for (i = 0; i < 20; ++i) 
   {
      printf("\nPage number: %d\n", refrence[i]);

      if (!isHit(page, refrence[i]))
      { 
	   
            printf("\nIt's a miss\n");
            ++page_fault;
             
            pre = page;
	    cur = page;
           
            while(cur->next != NULL)
            {   
		pre = cur;
                cur = cur->next;
            }
            
	   struct frame *temp = (struct frame*) malloc(sizeof(struct frame));
           temp->value = refrence[i];

            if (frames > 1)
	    {
            	 temp->next = page;
	    	 pre->next = NULL;
	    }

	    page = temp;
      }
      
      else if (frames > 1 && page->value != refrence[i])
      {
	  pre = page;
          cur = page;
	  printf("\nit's a hit \n");
          
	  while (cur->value != refrence[i] )
	  {      
		 pre = cur;
	         cur = cur->next;		
	  } 

	  struct frame *temp = (struct frame*) malloc(sizeof(struct frame));
          temp->value = cur->value;
          temp->next = page;
          page =  temp;
          pre->next = cur->next;
      }

      else
      {
         printf("\nit's a hit \n");

      }
      
      printPage(page);
   }
      
   printf("Total: number of page faults = %d\n", page_fault);
  
}

int longest(int val, int refrence[], int index)
{
      int longest = 0;
  
      while(index < 20)
      {
          if (refrence[index] == val)
	  {
              return longest;

          }

	  ++longest;
	  ++index;
      }
      
      return longest;
}



int isHit(struct frame *page, int val)
{    
     struct frame *p = page;
     
     if(page == NULL)
	return 0;

     while(p != NULL)
     {
	 if (p->value == val)
	     return 1;
              
         p = p->next;
     }

    return 0;
}

void printPage(struct frame *page)
{
    struct frame *cur = page;

    while (cur != NULL)
    {
        printf("Page value: %d\n", cur->value);
	cur= cur->next;
    }
}

int isEmpty(struct frame *page)
{
    struct frame *cur = page;
    
    while(cur != NULL)
    {
       if(cur->value == -1)
	   return 1;

       cur = cur->next;
    }

    return 0;
}

