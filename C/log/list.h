#ifndef LIST_H
#define LIST_H

struct list_head{
  struct list_head *next,*prev;
};
#define LIST_HEAD_INIT(name) {&(name),&(name)}

#define LIST_HEAD(name) \
  struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
  list->next = list;
  list->prev = list;
}

static inline void __list_add(struct list_head *new,
                              struct list_head *prev,
                              struct list_head *next)
{
  next->prev = new;
  new->next = next;
  new->prev = prev;
  prev->next = new;
}

/* @new : new entry to be added
 * @head : list head to add it after
 */

static inline void list_add(struct list_head *new,struct list_head *head)
{
  __list_add(new,head,head->next);
}
/*@new : new entry to be added
 *@head : list head to add it before
 */
static inline void list_add_tail(struct list_head *new,struct list_head *head)
{
  __list_add(new,head->prev,head);
}

static inline void __list_del(struct list_head *prev, struct list_head *next)
{
  next->prev = prev;
  prev->next = next;
}

static inline void list_del(struct list_head *entry)
{
  __list_del(entry->prev,entry->next);
  // entry->next = NULL;
  //entry->prev = NULL;
}

static inline int list_empty(const struct list_head *head)
{
  return head->next == head;
}

#define list_for_each(pos,head) \
  for (pos = (head)->next;pos !=(head);pos = pos->next)
/*
 *@n another &struct list_head to use as a temporary storage
 */
#define list_for_each_safe(pos,n,head) \
  for (pos = (head)->next, n = pos->next; pos != (head); \
       pos = n,n = pos->next)

#define list_entry(ptr,type,member)          \
  ((type *)((char *)(ptr) - (size_t)(&((type *)0)->member)))

#define list_first_entry(ptr,type,member) \
  list_entry((ptr)->next,type,member)

#define list_next_entry(pos,member) \
  list_entry((pos)->member.next, typeof(*(pos)), member)

#define list_for_each_entry(pos,head,member)             \
  for( pos = list_first_entry(head,typeof(*pos),member); \
       &pos->member != (head);                           \
       pos = list_next_entry(pos,member))

#define list_for_each_entry_safe(pos,n,head,member)       \
  for( pos = list_first_entry(head,typeof(*pos),member),  \
               n = list_next_entry(pos,member);           \
       &pos->member != (head);                            \
       pos = n , n =  list_next_entry(pos,member))

#endif
