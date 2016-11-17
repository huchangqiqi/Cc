#include "list.h"
#include "mm.h"
#include<stdlib.h>
#include<memory.h>
#include<pthread.h>


#define BLOCK_SIZE (1<<12) /* 块大小 */
#define BLOCK_COUNT 5      /* 初始化块数 */

struct block_list{
  void* blcok;
  struct list_head list;
};

static struct{
  struct block_list    free_block;
  struct block_list    full_blcok;
  struct block_list *  current_block;
  unsigned int         block_size;
  unsigned int         block_count;
  unsigned int         used_size;
  void *               begin;
}memory;

static pthread_mutex_t alloc_mutex;

/*
 *获得一个空的块
 */
static struct block_list *
get_free_block()
{

  if(list_empty(&(memory.free_block.list)))
    {
      memory.block_count += BLOCK_COUNT;
      for(int i = 0; i < BLOCK_COUNT; i++)
        {
          void* block = malloc(memory.block_size);
          struct block_list* block_ptr =
            (struct block_list *)malloc(sizeof(struct block_list));
          block_ptr->blcok = block;
          list_add(&(block_ptr->list),&(memory.free_block.list));
        }
    }

  struct block_list * free =
    list_entry(&(memory.free_block.list),struct block_list,list);
  list_del(&(free->list));
  return free;
}

/*
 *获得一个写满的块
 */
char *
get_full_block()
{
  char* ret = NULL;
  struct block_list* full;
  if( !list_empty(&(memory.full_blcok.list)))
    {
      full = list_entry(&(memory.full_blcok.list),struct block_list,list);
      ret = (char*) full->blcok;
    }
    return ret;
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
  memory.full_blcok.blcok = NULL;
  memory.free_block.blcok = NULL;
  INIT_LIST_HEAD(&(memory.full_blcok.list));
  INIT_LIST_HEAD(&(memory.free_block.list));
  for(int i = 0;i < BLOCK_COUNT; i++)
    {
      void* block = malloc(memory.block_size);
      struct block_list* block_ptr =
        (struct block_list *)malloc(sizeof(struct block_list));
      block_ptr->blcok = block;
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
  struct block_list* node ;
  struct block_list* node_n;
  list_for_each_entry_safe(node, node_n, &(memory.free_block.list),list)
    {
      free(node->blcok);
      node->blcok = NULL;
      list_del(&(node->list));
      free(node);
    }

  free(memory.current_block->blcok);
  free(memory.current_block);
  memory.current_block = NULL;

  pthread_mutex_destroy(&alloc_mutex);
}

/*
 *释放一块已写入磁盘的内存
 */

int release_full_block()
{
  struct block_list * rel = list_entry(&(memory.full_blcok.list),struct block_list,list);
  list_del(&(rel->list));
  list_add(&(memory.free_block.list),&(rel->list));
  return 0;

}


void update_current_block()
{
  memory.used_size = 0;
  *((char*)memory.begin) = '\0';
  list_add(&(memory.full_blcok.list),&(memory.current_block->list));
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
  else{
    ret = NULL;
  }
  pthread_mutex_unlock(&alloc_mutex);
  return ret;
}
