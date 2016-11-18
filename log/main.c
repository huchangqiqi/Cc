#include "log.h"
#include<pthread.h>
#include<stdio.h>
void*
print_log(void* arg)
{
  for(int i = 0;i < 10 ; i++)
    {
      LOG_DEBUG(":我是测试字符串%s ,第 %d次打印, 线程 %s \n","zp",i,(char*)arg);
      LOG_INFO(":我是测试字符串%s ,第 %d次打印, 线程 %s \n","cc",i,(char*)arg);
      LOG_ERROR(":我是测试字符串%s ,第 %d次打印, 线程 %s \n","hm",i,(char*)arg);
      LOG_FATAL(":我是测试字符串%s ,第 %d次打印, 线程 %s \n","xh",i,(char*)arg);
      LOG_WARN(":我是测试字符串%s ,第 %d次打印, 线程 %s \n","xl",i,(char*)arg);
    }

  return NULL;
}

int
main()
{
  pthread_t thread_id[10];
  char threadname[10][10] = {
    "thread-A",
    "thread-B",
    "thread-C",
    "thread-D",
    "thread-E",
    "thread-F",
    "thread-G",
    "thread-H",
    "thread-I",
    "thread-J",
  };

  log_init();

  for(int i = 0;i < 10; i++)
    {
      pthread_create(&thread_id[i],NULL,print_log,(void*)threadname[i]);
      //printf("starting thread %s\n",threadname[i]);
    }
  for(int i = 0 ;i < 10 ;i++)
    {
      pthread_join(thread_id[i],NULL);
    }
  log_destory();
  return 0;
}
