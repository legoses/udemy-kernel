#ifndef STRING_H
#define STRING_H

#include <stdbool.h>

int strlen(const char *ptr);
int strnlen(const char *ptr, int max);
bool isdigit(char c);
char *strcpy(char *dest, const char *src);
int tonumericdigit(char c);

#endif
