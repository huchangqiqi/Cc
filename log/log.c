#include "log.h"
#include "mm.h"
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<stdarg.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define MAX_SIZE 200

pthread_mutex_t mutex ;
pthread_cond_t cond;
pthread_t thrd ;

static int log_fd ;
static int flag ;
static int is_full ;

/*
char* buff = NULL;
char* ptr = NULL;
char* pend = NULL;
*/

static char* get_level(log_level_t level){
  switch(level)
    {
    case DEBUG:return "DEBUG";
    case INFO:return "INFO";
    case WARN:return "WARN";
    case ERROR:return "ERROR";
    case FATAL:return "FATAL";
    default: return "";
    }
}


static void*
persistence(void* arg)
{
  char *page;
  while(1)
    {
      while((page=get_full_block()) == NULL)
        {
          pthread_cond_wait(&cond,&mutex);
        }
      write(log_fd,page,strlen(page));
      release_full_block();
      if(flag == 1)
        {
          break;
        }
    }
  return NULL;
}

int
log_cache(const char *log)
{
  int len = strlen(log);
  void* begin;

  cache:
  if( (begin = mem_alloc(len)) != NULL)
    {
      memcpy(begin,log,len);
    }
  else
    {
      pthread_cond_signal(&cond);
      goto cache;
    }
  return 0;
}


int
log_write(const char* file, const char* func,
              const int line,log_level_t level,char* msg,...)
{
  char log[MAX_SIZE] = {'\0'};
  size_t len = 0;

  time_t rawtime;
  time(&rawtime);
  struct tm* timeinfo = localtime(&rawtime);
  strftime(log,MAX_SIZE,"%y-%m-%d %H:%M:%S ",timeinfo);

  len = strlen(log);
  snprintf(log+len, MAX_SIZE - len,"%s %s %d [%s] ",
           file,func,line,get_level(level));

  len = strlen(log);
  va_list arg_ptr;
  va_start(arg_ptr,msg);
  vsnprintf(log+len,MAX_SIZE -len,msg,arg_ptr);
  va_end(arg_ptr);

  log_cache(log);

  return 0;
}

int
log_init()
{
  memory_init();
  pthread_mutex_init(&mutex,NULL);
  pthread_cond_init(&cond,NULL);
  if( -1 == (log_fd = open("test.log",O_CREAT | O_APPEND | O_RDWR,0666)))
    {
      perror("open file test.log failed.");
    }

  if(pthread_create(&thrd,NULL,persistence,NULL) != 0)
    {
      perror("create thread failed.");
    }
  return 0;
}

int
log_destory()
{
  flag = 1;
  update_current_block();
  pthread_cond_signal(&cond);
  pthread_join(thrd,NULL);
  memory_destory();
  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);
  close(log_fd);
  return 0;
}

