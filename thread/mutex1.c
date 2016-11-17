#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int counter = 0;
void* function()
{
  pthread_mutex_lock(&mutex1);
  counter++;
  printf("Counter value:%d\n",counter);
  pthread_mutex_unlock(&mutex1);
}
int main()
{
  int rc1 ,rc2;
  pthread_t pthread1,pthread2;
  if((rc1=pthread_create(&pthread1,NULL,&function,NULL))){
    printf("Thread createion failed:%d\n",rc1);
  }

  if((rc2=pthread_create(&pthread2,NULL,&function,NULL))){
    printf("Thread createion failed:%d\n",rc2);
  }

  pthread_join(pthread1,NULL);
  pthread_join(pthread2,NULL);

  exit(EXIT_SUCCESS);
}
