#ifndef MM_H
#define MM_H

char* get_full_block();
int memory_init();
void memory_destory();
int release_full_block();
void update_current_block();
void * mem_alloc(unsigned int size);

#endif
