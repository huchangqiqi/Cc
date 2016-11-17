#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

void * print_message_function(void *ptr)
{
  char* message;
  message = (char*)ptr;
  printf("%s\n",message);
}

int main()
{
	pthread_t thread1,thread2;
  const char* message1 = "Thread1 's message";
  const char* message2 = "Thread2 's message";
  int iret1,iret2;

  /*create independent threads each of which will execute function */
  iret1 = pthread_create(&thread1,NULL,print_message_function,(void*)message1);
  if(iret1){
    fprintf(stderr,"Error - pthread_create return code :%d\n",iret1);
    exit(EXIT_FAILURE);
  }

  iret2 = pthread_create(&thread2,NULL,print_message_function,(void*)message2);
  if(iret2){
    fprintf(stderr,"Error - pthread_create return code :%d\n",iret2);
    exit(EXIT_FAILURE);
  }

  /*wait till threads are complete before main continues.*/
  pthread_join(thread1,NULL);
  pthread_join(thread2,NULL);

  printf("pthread_create() for thread1 returns: %d\n",iret1);
  printf("pthread_create() for thread2 returns: %d\n",iret2);


  exit(EXIT_SUCCESS);
}
