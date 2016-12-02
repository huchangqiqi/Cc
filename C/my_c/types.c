#include"types.h"
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
#include <glib-2.0/glib.h>

MalVal * malval_new(MalType type,MalVal *metadata)
{
  MalVal *mv = (MalVal*)MAL_GC_MALLOC(sizeof(MalVal));
  mv->type = type;
  mv->metadata = metadata;
  return mv;
}
MalVal * malval_new_list(MalType type,GArray *val)
{
  MalVal *mv = malval_new(type,NULL);
  mv->val.array = val;
  return mv;
}
MalVal *malval_new_string()
{

}

MalVal *_listX(int count,...)
{
  MalVal *seq = malval_new_list( MAL_LIST,
                                g_array_sized_new(TRUE,TRUE,sizeof(MalVal*),
                                                  count));
  MalVal *v;
  va_list ap;
  va_start(ap,count);
  while(count-- > 0)
    {
      v = va_arg(ap,MalVal*);
      g_array_append_val(seq->val.array,v);
    }
  va_end(ap);
  return seq;

}

MalVal *mal_error = NULL;

void _error(const char *fmt,...)
{
  va_list args;
  va_start(args,fmt);
  mal_error = malval_new_string(g_strdup_vprintf(fmt,)args);
}
