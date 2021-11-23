#include "include/setup.h"
#include "include/netconnect.h"
#include "include/gtkwrapper.h"


char *buffer;
char *line;
int c;
int count = 0;
char *database;
char *path2folder;



int main (int argc, char **argv)
{
    test();
    FILE *config = fopen("config/config","r");
    if (config == NULL)
    {
        setup();
    }
    else {
        fseek(config,0L,SEEK_END);
        int sz = ftell(config);
        rewind(config);

        buffer = (char*) malloc (sz*sizeof(char));
        fread(buffer, 1, sz, config);
        rewind(config);
        do {
            c = getc(config);
            if (c == '\n') // Increment count if this character is newline
            count++;
        } while (c != EOF);
        line = strtok(buffer,"\n");
        for (int i=0; i < count; i++)
        {   
            if (line[0] != 35)
            {
                
                if (line[0] == 100 && line[1] == 98)
                {
                    database = (char*) malloc ((strlen(line)-7)*sizeof(char));
                    int q=0;
                    for (int j=6;j< strlen(line)-1;j++)
                    {
                        database[q]= line[j];
                        q++;
                    }
                }
                else if (line[0] == 100 && line[1] == 105 && line[2] == 114)
                {
                    path2folder = (char*) malloc ((strlen(line)-8)*sizeof(char));
                    int q=0;
                    for (int j=7;j< strlen(line)-1;j++)
                    {
                        path2folder[q]= line[j];
                        q++;
                    }
                }
            } 

            line = strtok(NULL, "\n");
        }
    }



    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    fclose (config);
    free (buffer);
    free(path2folder);
    free(database);
    return status;
}   