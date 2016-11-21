#include "list.h"
#include "mm.h"
#include<stdlib.h>
#include<memory.h>
#include<pthread.h>
#include<stdio.h>


#define BLOCK_SIZE (1<<12) /* 块大小 */
#define BLOCK_COUNT 5      /* 初始化块数 */

struct block_list{
  void* blcok;
  struct list_head list;
};

static struct{
  struct block_list    free_block;
  struct block_list    full_block;
  struct block_list *  current_block;
  unsigned int         block_size;
  unsigned int         block_count;
  unsigned int         used_size;
  void *               begin;
}memory;

static pthread_mutex_t alloc_mutex;

/*
 *获得一个空的块
 *
 *
 *@called by :memory_init update_current_block
 */
static struct block_list *
get_free_block()
{

  if(list_empty(&(memory.free_block.list)))
    {
      memory.block_count += BLOCK_COUNT;
      for(int i = 0; i < BLOCK_COUNT; i++)
        {
          struct block_list* block_ptr =
            (struct block_list *)malloc(sizeof(struct block_list));
          block_ptr->blcok  = malloc(memory.block_size);
          list_add(&(block_ptr->list),&(memory.free_block.list));
        }
    }

  struct block_list * free = list_first_entry(&(memory.free_block.list), struct block_list, list);
  list_del(&(free->list));

  return free;
}


/*
 * 初始化mem
 */

int
memory_init()
{
  memory.block_size = BLOCK_SIZE;
  memory.block_count = BLOCK_COUNT;
  memory.used_size = 0;
  memory.full_block.blcok = NULL;
  memory.free_block.blcok = NULL;
  INIT_LIST_HEAD(&(memory.full_block.list));
  INIT_LIST_HEAD(&(memory.free_block.list));

  for(int i = 0;i < BLOCK_COUNT; i++)
    {
      struct block_list* block_ptr =
        (struct block_list *)malloc(sizeof(struct block_list));
      block_ptr->blcok  = malloc(memory.block_size);
      list_add(&(block_ptr->list),&(memory.free_block.list));
    }

  memory.current_block = get_free_block();
  memory.begin = memory.current_block->blcok;

  pthread_mutex_init(&alloc_mutex,NULL);
  return 0;
}

/*
 *释放内存 
 */

void
memory_destory()
{
  struct block_list* pos;
  struct block_list* n;
  list_for_each_entry_safe(pos, n, &(memory.free_block.list), list)
    {
      list_del(&(pos->list));
      free(pos);
    }

  free(memory.current_block->blcok);
  free(memory.current_block);
  memory.current_block = NULL;

  pthread_mutex_destroy(&alloc_mutex);
}

/*
  *获得一个full块
  */
char *
get_full_block()
{
  // printf("start get_full_block\n");
  char* ret = NULL;
  struct block_list* full;
  if( !list_empty(&(memory.full_block.list)))
    {
      full = list_first_entry(&(memory.full_block.list),struct block_list,list);
      ret = (char*) full->blcok;
    }
  return ret;
}



/*
 *释放一个full块 移动到free链
 */

int release_full_block()
{
  //printf("start release_full_block\n");
  struct block_list * rel = list_first_entry(&(memory.full_block.list), struct block_list, list);
  list_del(&(rel->list));
  list_add(&(memory.free_block.list),&(rel->list));
  return 0;

}

/*
 * 讲当前块添加到full列表
 * 从free中取出一个作为current
 *
 */
void update_current_block()
{
  memory.used_size = 0;
  *((char*)memory.begin) = '\0';
  list_add(&(memory.full_block.list),&(memory.current_block->list));
  memory.current_block = get_free_block();
}

static int
append_isover(unsigned int size)
{
  int ret = 0;
  memory.used_size += size;
  if(memory.used_size + 1 > memory.block_size)
    {
      ret = 1;
      update_current_block();
      memory.begin = memory.current_block->blcok;
    }
  return ret;
}

/*
 *
 */
void*
mem_alloc(unsigned int size)
{
  void *ret ;
  pthread_mutex_lock(&alloc_mutex);
  ret = memory.begin;
  if(!append_isover(size))
    {
      memory.begin = (void*)((char*)memory.begin + size);
    }
  else
    {
    ret = NULL;
    }
  pthread_mutex_unlock(&alloc_mutex);
  return ret;
}
