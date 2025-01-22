#pragma once

#include <stdio.h>
#include <stdarg.h>


void red()     { printf("\033[31m"); }
void green()   { printf("\033[32m"); }
void yellow()  { printf("\033[33m"); }
void blue()    { printf("\033[34m"); }
void magenta() { printf("\033[35m"); }
void cyan()    { printf("\033[36m"); }
void white()   { printf("\033[37m"); }

void reset()   { printf("\033[0m"); }


void printf_clr(void (*clr)(), const char *format, ...) {
    va_list args;

    clr();

    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    reset();
}