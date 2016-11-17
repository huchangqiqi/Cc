#ifndef LOG_H
#define LOG_H

typedef enum{
  DEBUG = 1,
  INFO,
  WARN,
  ERROR,
  FATAL,
}log_level_t;

int log_init();
int log_destory();
int log_write(const char* file,const char* func ,
              const int line,log_level_t level,char* msg ,...);

#define LOG_DEBUG(msg,args...) \
  log_write(__FILE__, __FUNCTION__, __LINE__, DEBUG, msg, ##args)
#define LOG_INFO(msg,args...)                                    \
  log_write(__FILE__, __FUNCTION__, __LINE__, INFO, msg, ##args)
#define LOG_WARN(msg,args...)                                    \
  log_write(__FILE__, __FUNCTION__, __LINE__, WARN, msg, ##args)
#define LOG_ERROR(msg,args...)                                    \
  log_write(__FILE__, __FUNCTION__, __LINE__, ERROR, msg, ##args)
#define LOG_FATAL(msg,args...)                                    \
  log_write(__FILE__, __FUNCTION__, __LINE__, FATAL, msg, ##args)

#endif
