#include "list.h"
#include<stdio.h>
#include<stdlib.h>

struct student
{
  int id;
  char *name;
  struct list_head list;
 };
int main()
{

  struct student * stu;
  struct student *sttu;
  struct student stu_head;
  struct list_head *pos,*next;
  char * NAME[] = {"tonny","andy","michael","john","spike"};
  INIT_LIST_HEAD(&stu_head.list);

  for(int i = 0;i< 5 ;i++){
    stu = (struct student * )malloc(sizeof(struct student));
    stu->id = i+1;
    stu->name = NAME[i];
    list_add_tail(&(stu->list),&(stu_head.list));
  }
  //printf the list
  printf("first time print\n");
  list_for_each(pos,&stu_head.list)
    {
      stu = list_entry(pos,struct student,list);
      printf("id=%d,name=%s\n",stu->id,stu->name);
    }

  //del id =4
  printf("del the 4\n");
  list_for_each_safe(pos,next,&stu_head.list){
    stu = list_entry(pos,struct student,list);
    if(stu->id == 4){
      list_del(pos);
      free(stu);
    }
  }

  //printf the list
  printf("second time print\n");
  list_for_each(pos,&stu_head.list)
    {
      stu = list_entry(pos,struct student,list);
      printf("id=%d,name=%s\n",stu->id,stu->name);
    }
  /*
  //free the list
  list_for_each_safe(pos,next,&stu_head.list)
    {
      stu = list_entry(pos,struct student,list);
      list_del(pos);
      free(stu);
    }
  //free
  */
  list_for_each_entry(stu,&stu_head.list,list)
    {
      printf("pos-id=%d,pos-name=%s\n",stu->id,stu->name);
    }
  list_for_each_entry_safe(stu,sttu,&stu_head.list,list)
    {
      list_del(&(stu->list));
      free(stu);
    }
  list_for_each_entry(stu,&stu_head.list,list)
    {
      printf("pos-id=%d,pos-name=%s\n",stu->id,stu->name);
    }
  return 0;

}
