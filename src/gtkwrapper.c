#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>

GtkWidget *window;
char *buffer;
char *line;
int c;
int count = 0;
int slcDay, slcMon, slcYr;
char *database;
char *path2folder;
GtkWidget *list;
GtkWidget *hselect, *mselect, *text_entry;


/* 
TODO:
    1) Get the calendar to add events format - hh:mm | text
    2) Come up with a way to store events in a database
    3) Get the setup to create the filesystem if it is not there already
    4) Get The Calendar to Events On Day by Day? Or Week By Week?


*/


void setup ()
{



}

//Adding Entry To calendardb
void add_entry ()
{
    double hours = gtk_spin_button_get_value (GTK_SPIN_BUTTON (hselect));
    double minutes = gtk_spin_button_get_value (GTK_SPIN_BUTTON (mselect));
    const char *name;
    name = gtk_entry_buffer_get_text (GTK_ENTRY_BUFFER (gtk_entry_get_buffer (GTK_ENTRY (text_entry))));

    printf("Hours: %f, Minutes: %f\n",hours, minutes);
    printf("Day %d, Month %d, Year %d\n", slcDay, slcMon, slcYr);
    printf("%s\n",name);
}


/* void new_entry (char* i, GDateTime *date, GtkWidget *box_list)
{
    GtkEntryBuffer *temp_buffer;
    GtkWidget *temp_text, *entry;
    
    entry = gtk_list_box_row_new ();
    temp_buffer = gtk_entry_buffer_new (i, strlen(i));
    temp_text = gtk_text_new_with_buffer (temp_buffer);
    gtk_list_box_row_set_child (GTK_LIST_BOX_ROW (entry), temp_text);
    gtk_list_box_append (GTK_LIST_BOX (box_list), entry);
} */


void load_events ()
{

}


//Event Dialogue Option
static void entry_dialogue (GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog, *content_area, *grid_dia, *cancel_button, *entry_button_dia;
    GtkEntryBuffer *entry_buffer;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    GtkAdjustment *hadjust, *madjust;
    
    dialog = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (dialog), "Add Event");
    gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (window));
    grid_dia = gtk_grid_new ();
    gtk_window_set_default_size (GTK_WINDOW (dialog), 200, 75);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

    hadjust = gtk_adjustment_new(12.0, 0.0, 24.0, 1.0, 5.0, 0.0);
    hselect = gtk_spin_button_new(hadjust, 1.0, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE (hselect), GTK_ORIENTATION_VERTICAL);
    gtk_grid_attach(GTK_GRID (grid_dia), hselect, 0, 0, 1, 1);

    madjust = gtk_adjustment_new(30.0, 0.0, 60.0, 1.0, 5.0, 0.0);
    mselect = gtk_spin_button_new(madjust, 1.0, 0);
    gtk_orientable_set_orientation(GTK_ORIENTABLE (mselect), GTK_ORIENTATION_VERTICAL);
    gtk_grid_attach(GTK_GRID (grid_dia), mselect, 1, 0, 1, 1);

    entry_buffer = gtk_entry_buffer_new (NULL, -1);
    text_entry = gtk_entry_new_with_buffer (entry_buffer);
    gtk_grid_attach (GTK_GRID (grid_dia), text_entry, 0, 1, 2, 1);

    entry_button_dia = gtk_button_new_with_label("Add Event");
    gtk_grid_attach(GTK_GRID (grid_dia), entry_button_dia, 2, 0, 1, 1);

    g_signal_connect (entry_button_dia, "clicked", G_CALLBACK (add_entry), NULL);
    g_signal_connect_swapped (entry_button_dia, "clicked", G_CALLBACK (gtk_window_destroy), dialog);

    cancel_button = gtk_button_new_with_label ("Cancel");
    gtk_grid_attach (GTK_GRID (grid_dia), cancel_button, 2, 1, 1, 1);

    g_signal_connect_swapped (cancel_button, "clicked", G_CALLBACK (gtk_window_destroy), dialog);
    gtk_box_append (GTK_BOX (content_area), grid_dia);

    gtk_widget_show (dialog);
}

//Retrieve the Current Selected Day
static void day_marked (GtkWidget *widget, gpointer data)
{
    GDateTime *tempDateTime = gtk_calendar_get_date (GTK_CALENDAR (widget));
    slcDay = g_date_time_get_day_of_month (tempDateTime);
    slcMon = g_date_time_get_month (tempDateTime);
    slcYr = g_date_time_get_year (tempDateTime);
    load_events();
}

//Main Window
static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *grid, *calendar, *frame, *entry_button;
    
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window),"Calendar");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);

    grid = gtk_grid_new ();
    gtk_window_set_child (GTK_WINDOW (window), grid);

    calendar = gtk_calendar_new ();
    gtk_widget_set_hexpand (GTK_WIDGET (calendar), TRUE);
    gtk_widget_set_vexpand (GTK_WIDGET (calendar), TRUE);
    g_signal_connect (calendar, "day-selected", G_CALLBACK (day_marked), NULL);
    gtk_grid_attach (GTK_GRID (grid), calendar, 0, 0, 6, 1);

    frame = gtk_frame_new (NULL);

    list = gtk_list_box_new ();
    gtk_frame_set_child (GTK_FRAME (frame), list);
    gtk_grid_attach (GTK_GRID (grid), frame, 0, 2, 6, 1);

    entry_button = gtk_button_new_with_label ("Add Event");
    g_signal_connect (entry_button, "clicked", G_CALLBACK (entry_dialogue), NULL);
    gtk_grid_attach (GTK_GRID (grid), entry_button, 0, 3, 6, 1);

    gtk_widget_show (window);
}


int main (int argc, char **argv)
{
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
