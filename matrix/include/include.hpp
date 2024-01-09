#include <Arduino.h>
#include <string>

#include <cstdarg>


#define DEBUG 1

#if DEBUG

#define DEBUG_BUF 512

#ifndef LOGF_FN
#define LOGF_FN
static inline void logf(const char* fmt, ...){

    size_t buf_size = DEBUG_BUF;
    if(strlen(fmt) > buf_size ) buf_size = strlen(fmt) * 2;
  va_list args;
  va_start(args, fmt);
  char buf[buf_size]; //too big?
  vsnprintf(buf, sizeof(char) * buf_size, fmt, args);
    
  Serial.println(buf);
  va_end(args);
}
#endif 

#define LOG(str) Serial.println(str);
#define LOGF(fmt, ...) logf(fmt, __VA_ARGS__);

#else
#define LOG(str) 
#define LOGF(fmt, ...)
#endif