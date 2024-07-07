#ifndef __STD_IO_H__
#define __STD_IO_H__

#include <types.h>
#include <stdarg.h>

int printf(const char *fmt, ...);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);

#endif
