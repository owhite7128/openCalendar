#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>

GtkWidget *window;
char *buffer;
int slcDay, slcMon, slcYr;
const char* const hours[24] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24"};
const char* const minutes[31] = {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31"};
GtkWidget *list;


void setup ()
{


}

void new_entry (char* i, GDateTime *date, GtkWidget *box_list)
{
    GtkEntryBuffer *temp_buffer;
    GtkWidget *temp_text, *entry;
    
    entry = gtk_list_box_row_new ();
    temp_buffer = gtk_entry_buffer_new (i, strlen(i));
    temp_text = gtk_text_new_with_buffer (temp_buffer);
    gtk_list_box_row_set_child (GTK_LIST_BOX_ROW (entry), temp_text);
    gtk_list_box_append (GTK_LIST_BOX (box_list), entry);
}


void load_events ()
{

}

static void new_entry_callback (GtkWidget *widget, gpointer data)
{

}

static void entry_dialogue (GtkWidget *widget, gpointer data)
{
    GtkWidget *dialog, *content_area, *hselect, *mselect, *text_entry, *grid_dia, *cancel_button;
    GtkEntryBuffer *entry_buffer;
    GtkDialogFlags flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT;
    
    dialog = gtk_dialog_new ();
    gtk_window_set_title (GTK_WINDOW (dialog), "Add Event");
    gtk_window_set_transient_for (GTK_WINDOW (dialog), GTK_WINDOW (window));
    grid_dia = gtk_grid_new ();
    gtk_window_set_default_size (GTK_WINDOW (dialog), 200, 75);
    content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));

    hselect = gtk_drop_down_new (NULL, NULL);
    gtk_grid_attach (GTK_GRID (grid_dia), hselect, 0, 0, 1, 1);
    mselect = gtk_drop_down_new (NULL, NULL);
    gtk_grid_attach (GTK_GRID (grid_dia), mselect, 1, 0, 1, 1);
    entry_buffer = gtk_entry_buffer_new (NULL, -1);
    text_entry = gtk_entry_new_with_buffer (entry_buffer);
    gtk_grid_attach (GTK_GRID (grid_dia), text_entry, 0, 1, 2, 1);
    cancel_button = gtk_button_new_with_label ("Cancel");
    gtk_grid_attach (GTK_GRID (grid_dia), cancel_button, 3, 1, 1, 1);

    g_signal_connect_swapped (cancel_button, "clicked", G_CALLBACK (gtk_window_destroy), dialog);
    gtk_box_append (GTK_BOX (content_area), grid_dia);

    gtk_widget_show (dialog);
}


static void day_marked (GtkWidget *widget, gpointer data)
{
    GDateTime *tempDateTime = gtk_calendar_get_date (GTK_CALENDAR (widget));
    slcDay = g_date_time_get_day_of_month (tempDateTime);
    slcMon = g_date_time_get_month (tempDateTime);
    slcYr = g_date_time_get_year (tempDateTime);
    char* temp_string = (char*) malloc (28*sizeof(char));
    sprintf(temp_string,"Day %d, Month %d, Year %d", slcDay, slcMon, slcYr);
    new_entry (temp_string, tempDateTime, list);
    free(temp_string);
    load_events();
}

static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *grid, *calendar, *frame, *entry_button;
    
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window),"Open Calendar");
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
    return;
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
    }



    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    free (buffer);
    return status;
}   
