#include "include/access_base.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



/*
    How Database Stores Events


 */

char *base= NULL;
int baseSz, baseC, dateLine;
int lnum = 0;
FILE *basefile;
int dayY, monY, yrY, currD, currM, currY;


int ret_day_base (B_DATETIME datetime, char* buffer) {
    int line = -1;
    char* baseLine;
    char* buffer_rest;
    char* curr_date;
    char* curr_date_rest;
    baseLine = strtok_r (buffer, "\n", &buffer_rest);
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
            line = iL;
            break;
        }
        baseLine = strtok_r (NULL, "\n", &buffer_rest);
    }

    return line;
    free (buffer);
}


void n_base (char* base_path)
{
    base = base_path;
    basefile = fopen (base_path,"r+");
    if (basefile == NULL)
    {
        printf ("ERR: Database Does not Exist\n");
        exit(-1);
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
    fclose (basefile);
}

B_EVENT* r_base(B_DATETIME datetime, int *num_event)
{
    B_EVENT *ret;

    char *r_buffer, *r_buffer_ret, *r_buffer_rest;
    char *baseLine, *event_str;
    char *event_line, *event_line_rest;
    char *curr_time, *curr_time_rest;

    dateLine = -1;
    dayY = monY = yrY = -1;
    n_base(base);
    int n_entries;

    basefile = fopen (base, "r+");
    r_buffer = (char*) malloc (baseSz * sizeof(char));
    fread (r_buffer, 1, baseSz, basefile);
    rewind (basefile);
    r_buffer_ret = (char*) malloc (baseSz *sizeof(char));
    fread (r_buffer_ret, 1, baseSz, basefile);
    rewind (basefile);
    fclose (basefile);

    dateLine = ret_day_base (datetime, r_buffer_ret);

    if (dateLine == -1)
    {
        ret = (B_EVENT*) malloc (1*sizeof (B_EVENT));
        return ret;
    }

    baseLine = strtok_r (r_buffer,"\n",&r_buffer_rest);
    for (int i=0; i<dateLine;i++)
    {
        baseLine = strtok_r (NULL, "\n", &r_buffer_rest);
    }

    event_str = strtok (r_buffer_rest,":");
    int q=0;
    int lns=0;
    while (event_str[q] != '\0')
    {
        if (event_str[q] == '\n')
        {
            lns++;
        }
        if (event_str[q] == '\"')
        {
            event_str = strtok (event_str,"\"");
            break;
        }
        q++;
    } 
    *num_event = lns;

    ret = (B_EVENT*) malloc (lns*sizeof(B_EVENT));

    event_line = strtok_r (event_str, "\n", &event_line_rest);

    for (int i=0; i<lns; i++)
    {
        curr_time = strtok_r (event_line, " ", &curr_time_rest);
        curr_time = strtok_r (NULL, " ", &curr_time_rest);
        ret[i].datetime.day = datetime.day;
        ret[i].datetime.month = datetime.month;
        ret[i].datetime.year = datetime.year;
        ret[i].datetime.hour = atoi (curr_time);
        curr_time = strtok_r (NULL, " ", &curr_time_rest);
        ret[i].datetime.minute = atoi (curr_time);
        ret[i].text = curr_time_rest;

        event_line = strtok_r (NULL, "\n", &event_line_rest);
    }


    free (r_buffer);
    return ret;
}

void w_base(char* out, B_DATETIME datetime)
{
    char *w_buffer, *w_buffer_ret;
    char str[128];
    char str_head[128];
    int n, m;
    int entry;
    n_base(base);

    basefile = fopen (base, "r+");
    w_buffer_ret = (char*) malloc((baseSz + 255)*sizeof(char));
    w_buffer = (char*) malloc((baseSz + 255)*sizeof(char));
    fread(w_buffer, 1, baseSz+255, basefile);
    rewind(basefile);
    fread(w_buffer_ret, 1, baseSz+255, basefile);
    rewind(basefile);
    fclose (basefile);

    int ln = ret_day_base (datetime, w_buffer_ret);
    n = sprintf (str, "; %d %d %s\n", datetime.hour, datetime.minute, out);

    if (ln == -1) 
    {
        m = sprintf (str_head, ": %d %d %d\n", datetime.month, datetime.day, datetime.year);
        for (int i =(baseSz + 256 - n - m); i >= 0; i--)
        {
            w_buffer[i+n+m] = w_buffer[i];
        }

        for (int i=0; i<m; i++)
        {
            w_buffer[i] = str_head[i];
        }
        for (int i=0; i<n; i++)
        {
            w_buffer[i+m] = str[i];
        }
    } else 
    {
        int q = 0;
        for (int i=0; i< ln+1; i++)
        { 
            while (w_buffer[q] != '\n')
            {
                q++;
            }
            q++;
        }
        q--;

        for (int i=(baseSz + 256 - n); i > q; i--)
        {
            w_buffer[i+n] = w_buffer[i];
        }
        for (int i=0; i< n; i++)
        {
            w_buffer[q+1+i] = str[i];
        }
    }
    
    basefile = fopen (base, "w+");
    fwrite (w_buffer, 1, baseSz + n + m, basefile);
    fclose (basefile);
    free (w_buffer);
}