#pragma once

#include <stdio.h>
#include <stdarg.h>


// Basic font colors
void black()   { printf("\033[30m"); }
void red()     { printf("\033[31m"); }
void green()   { printf("\033[32m"); }
void yellow()  { printf("\033[33m"); }
void blue()    { printf("\033[34m"); }
void magenta() { printf("\033[35m"); }
void cyan()    { printf("\033[36m"); }
void white()   { printf("\033[37m"); }


// Bright font colors
void bright_black()   { printf("\033[90m"); }
void bright_red()     { printf("\033[91m"); }
void bright_green()   { printf("\033[92m"); }
void bright_yellow()  { printf("\033[93m"); }
void bright_blue()    { printf("\033[94m"); }
void bright_magenta() { printf("\033[95m"); }
void bright_cyan()    { printf("\033[96m"); }
void bright_white()   { printf("\033[97m"); }


// special
void bold_underline_cyan() { printf("\033[1m\033[4m\033[36m"); }


// Reset to default font color
void reset() { printf("\033[0m"); }


void printf_clr(void (*clr)(), const char *format, ...) {
    va_list args;

    clr();

    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    reset();
}