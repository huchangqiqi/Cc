#include"reader.h"
#include"types.h"

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <glib-2.0/glib.h>
#include <glib-2.0/glib-object.h>
#include <glib-2.0/glib/gregex.h>
#include <glib-2.0/glib/garray.h>

//Declare
MalVal *read_form(Reader *reader);

Reader *reader_new()
{
  Reader *reader = (Reader*)MAL_GC_MALLOC(sizeof(Reader));
  reader->array = g_array_sized_new(TRUE,FALSE,sizeof(char*),8);
  reader->position = 0;
  return reader;
}

char *reader_peek(Reader *reader)
{
  return g_array_index(reader->array,char * ,reader->position);
}

Reader *tokenizer(char *line)
{

  GRegex *regex;
  GMatchInfo *matchInfo;
  GError * err = NULL;

  Reader *reader = reader_new();
  regex = g_regex_new("[\\s ,]*(~@|[\\[\\]{}()'`~@]|\"(?:[\\\\].|[^\\\\\"])*\"|;.*|[^\\s \\[\\]{}()'\"`~@,;]*)", 0, 0, &err);
  g_regex_match(regex,line,0,&matchInfo);
  if(err != NULL)
    {
      fprintf(stderr,"Tokenize error:%s\n",err->message);
      return NULL;
    }
  while(g_match_info_matches(matchInfo))
    {
      gchar *result = g_match_info_fetch(matchInfo,1);
      if(result[0] != '\0' && result[0] != ';')
        {
          reader_append(reader,result);
        }
      g_match_info_next(matchInfo,&err);
    }
  g_match_info_free(matchInfo);
  g_regex_unref(regex);
  if(reader->array->len == 0)
    {
      reader_free(reader);
      return NULL;
    }
  else
    {
      return reader;
    }

}

/* a left paren then read_list
 * otherwise read_aton is called
 * with the Reader object
 */
MalVal *read_form(Reader *reader)
{
  char *token;
  MalVal *form = NULL,*tmp;

  token = reader_peek(reader);
  if(!token){return NULL;}
  switch(token[0])
    {
    case ';':
      break;
    case '\'':
      reader_next(reader);
      form = _listX(2,malval_new_symbol("quote"),read_form(reader));
      break;
    case '`':
      reader_next(reader);
      form = 
    }
}



MalVal *read_atom(Reader *reader)
{
  char *token;
  GRegex *regex;
  GMatchInfo * matchInfo;
  GError *err = NULL;
  gint pos;
  MalVal *atom;

  token = reader_next(reader);
  regex = g_regex_new("(^-?[0-9]+$)|(^-?[0-9][0-9.]*$)|(^nil$)|(^true$)|(^false$)|^\"(.*)\"$|:(.*)|(^[^\"]*$)", 0, 0, &err);
  g_regex_match(regex,token,0,&matchInfo);

  if(g_match_info_fetch_pos(matchInfo,1,&ops,NULL) && pos != -1)
    {
      //integer
      atom = malval_new_integer(g_ascii_strtoll(token,NULL,10));
    }
  else
    {
      malval_free(atom);
      atom = NULL;
    }
  return atom;

}

/* repeatedly call read_form until a ')'
 *
 */
MalVal *read_list(Reader *reader,MalType type,char start,char end)
{
  MalVal *ast,*form;
  char *token = reader_next(reader);
  if(token[0] != start)
    {
      abort("expected '(' or '['");
    }
  ast = malval_new_list(type,g_array_new(TRUE,TRUE,sizeof(MalVal*)));
  while((token = reader_peek(reader)) && token[0] != end)
    {
      form = read_form(reader);
      if(!form)
        {
          if(!mal_error)
            {
              abort("unknown read_list failure");
            }
          g_array_free(ast);
          return NULL;
        }
      g_array_append_val(ast->val.array,form);
    }
  if(!token){ abort ("expected ')' or ']', got EOF"); }
  reader_next(reader);
  return ast;
}

/*call tokenizer create a new Reader object
 *call read_form with the Reader instance
 *
 */
MalVal *read_str(char *str)
{
  Reader *reader;
  char *token;
  MalVal *ast = NULL;
  reader = tokenizer(str);
  if(reader)
    {
      ast = read_form(reader);
      reader_free(reader);
    }
  return ast;
}

