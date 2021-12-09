#include "include/access_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char *base= NULL;
int baseSz, baseC, dateLine;
int lnum = 0;
FILE *basefile;
char *r_buffer;
char *r_buffer_rest;
char *curr_date;
char *curr_date_rest;
char *baseLine;
int dayY, monY, yrY, currD, currM, currY;

void n_base (char* base_path)
{
    base = base_path;
    basefile = fopen (base_path,"r+");
    if (basefile == NULL)
    {
        printf ("ERR: Database Does not Exist\n");
    }
    else {
        fseek (basefile, 0L, SEEK_END);
        baseSz = ftell (basefile);
        rewind (basefile);

        do {
            baseC = getc(basefile);
            if (baseC == '\n')
            lnum++;
        } while (baseC != EOF);

    }
    rewind(basefile);
}

char* r_base(B_DATETIME datetime)
{
    dateLine = -1;
    dayY = monY = yrY = -1;

    r_buffer = (char*) malloc (baseSz * sizeof(char));
    fread (r_buffer, 1, baseSz, basefile);
    rewind (basefile);

    baseLine = strtok_r (r_buffer, "\n", &r_buffer_rest);
    for (int iL=0; iL < lnum; iL++)
    {
        if (baseLine[0] == 58)
        {   
            curr_date = strtok_r (baseLine, " ", &curr_date_rest);
            curr_date = strtok_r (NULL, " ", &curr_date_rest);
            if ( atoi(curr_date) == datetime.month )
            {
                monY = 1;
            }else
            {
                monY = 0;
            }
            curr_date = strtok_r (NULL, " ", &curr_date_rest);
            if ( atoi(curr_date) == datetime.day )
            {
                dayY = 1;
            }else
            {
                dayY = 0;
            }
            curr_date = strtok_r (NULL, " ", &curr_date_rest);
            if ( atoi(curr_date) == datetime.year )
            {
                yrY = 1;
            }else
            {
                yrY = 0;
            }

        }
        else if (baseLine[0] == 34)
        {
            break;
        }
        if (dayY == 1 && monY == 1 && yrY == 1)
        {   
            dateLine = iL;
            break;
        }
        baseLine = strtok_r (NULL, "\n", &r_buffer_rest);
    }
    if (dateLine != -1)
    {
        printf ("Date Exists: %d/%d/%d on Line: %d\n",datetime.month,datetime.day,datetime.year, dateLine);
    }
    else
    {
        printf("Day Does Not Exists: %d/%d/%d\n",datetime.month,datetime.day,datetime.year);
    }
    


    free (r_buffer);
    return "Test";
}

int w_base(char* out)
{

    printf("%s\n",base);
}