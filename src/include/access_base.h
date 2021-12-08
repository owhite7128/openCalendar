#ifndef ACCESS_BASE_H
#define ACCESS_BASE_H
#include <stdio.h>

typedef struct B_DATETIME
{
    int day;
    int month;
    int year;
    int hour;
    int minute;
} B_DATETIME;

typedef struct B_EVENT
{
    B_DATETIME datetime;
    char* text;
} B_EVENT;


void n_base (char* base_path);

char* r_base(B_DATETIME datetime);

int w_base(char* out);


#endif