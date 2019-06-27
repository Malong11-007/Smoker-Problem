Name : Danyal Javed
Roll no. : CS-133

//Smoker’s Problem using Semaphore
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//check if any one smoking
sem_t smoking;
int canSmoke = 0;

//materials for smoking
int generatedItem[2], areItemGenerated = 0;
char *items[] = { "tobacco", "paper", "matches" };
	// tobacco - 0 : paper - 1 : matches - 2

int getRandomNum(int lower, int upper) 	//Randomize what items agent generate
{ 
    srand(time(NULL)); //Seeding with current time
    return rand() % (upper) + lower; 
} 
void *agent(void *arg)
{
	int randNum;
	while(1)
	{
		sem_wait(&smoking);
		if(canSmoke == 0)
		{
			randNum = getRandomNum(0,3);
			//printf("generated Number %i\n",randNum); //generate number between 0 - 2
			switch (randNum)
			{
			    case 0:	// tobacco and paper
			    	generatedItem[0] = 0; 
			    	generatedItem[1] = 1;
			        break;

			    case 1: // paper and matches
			    	generatedItem[0] = 1;
			    	generatedItem[1] = 2;
			        break;

			    case 2: // matches and tobacco
			    	generatedItem[0] = 2;
			    	generatedItem[1] = 0;
			        break;

			    default: // tobacco and paper
			    	generatedItem[0] = 0; 
			    	generatedItem[1] = 1;
			}
			printf("Agent produced %s,%s\n", items[generatedItem[0]], items[generatedItem[1]]);
			areItemGenerated = 1; // items are generated 
			canSmoke = 1; // someone can smoke 
		}
		sem_post(&smoking);	
	}
}

void *smokerI(void *arg)
{
	int i = *((int *)arg);
	while(1){
		
		sleep(1);
		sem_wait(&smoking);
		if(canSmoke == 1)
		{
			if(areItemGenerated && generatedItem[0] != i && generatedItem[1] != i)			
{						//skip if you already have one of the items
				printf("Smoker with %s collected items\n",items[i]);
				printf("preparing to smoke\n");
				printf("smoking...\n");
				sleep(2);
				printf("Smoker with %s has completed smoking\n",items[i]);
				printf("------------------------------------------------\n\n");
printf(“Press Enter to Produce New Items”);
				getchar();
				areItemGenerated = 0;
				canSmoke = 0;
			}
		}
		sem_post(&smoking);
	}
}

int main()
{
	int s0 = 0, s1 = 1, s2 = 2; 
	//Smoker0 has tobacco - 0 
	//Smoker1 has paper   - 1
	//Smoker2 has matches - 2
	pthread_t smoker0, smoker1, smoker2, agentThread;

	sem_init(&smoking,0,1);

	pthread_create(&agentThread,0,agent,0);
	pthread_create(&smoker0,0,smokerI,(void * ) &s0);
	pthread_create(&smoker1,0,smokerI,(void * ) &s1);
	pthread_create(&smoker2,0,smokerI,(void * ) &s2);

	pthread_join(smoker0,NULL);		
	pthread_join(smoker1,NULL);
	pthread_join(smoker2,NULL);
	pthread_join(agentThread,NULL);
	
	while(1);
}
