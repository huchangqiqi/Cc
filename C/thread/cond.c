#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_mutex_t count_mutex ;
pthread_cond_t condition_var;
int count = 0;
#define COUNT_DONE 10
#define COUNT_HALT1 3
#define COUNT_HALT2 6

void *functionCount1(){
  for(;;)
    {
      /*lock mutex and then wait for signal to relase mutex*/

      pthread_mutex_lock(&count_mutex);
      //wait while functionCount2() operates on count
      // mutex unlocked if condition varialbe in functionCount2() signaed.
      pthread_cond_wait(&condition_var,&count_mutex);
      count++;
      printf("Counter value functionCount1 :%d\n",count);

      pthread_mutex_unlock(&count_mutex);
      if(count >= COUNT_DONE) return (NULL);

    }
}
void *functionCount2(){
  for(;;)
    {
      pthread_mutex_lock(&count_mutex);
      if(count < COUNT_HALT1 || count > COUNT_HALT2)
        {
          pthread_cond_signal(&condition_var);
        }
      else
        {
          count++;
          printf("Counter value functionCount2 :%d\n",count);
        }
      pthread_mutex_unlock(&count_mutex);
      if(count >= COUNT_DONE) return (NULL);

    }
}
int main()
{
  pthread_t thread1,thread2;
  pthread_create(&thread1,NULL,&functionCount1,NULL);
  pthread_create(&thread2,NULL,&functionCount2,NULL);

  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);
  printf("Final count:%d\n",count);
  exit(EXIT_SUCCESS);
}

