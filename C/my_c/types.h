#define MAL_GC_MALLOC malloc

typedef enum{
  MAL_SYMBOL = 32,
  MAL_LIST = 128,
}MalType;

typedef struct MalVal{
  MalType type;
  struct MalVal *metadata;
}MalVal;

extern struct MalVal *mal_error;
void _error(const char *fmt,...);
