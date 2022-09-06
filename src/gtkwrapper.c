#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include "include/gtkwrapper.h"
#include "include/access_base.h"

GtkWidget *window;
int slcDay, slcMon, slcYr;
GtkWidget *list;
GtkWidget *hselect, *mselect, *text_entry;

B_DATETIME tdatetime;

void remove_events (GtkWidget *ltr)
{
    while (gtk_widget_get_first_child (ltr)) {
        gtk_list_box_remove (GTK_LIST_BOX (ltr), gtk_widget_get_first_child (ltr));
    }
}

//Adding Entry To calendardb
void add_entry ()
{
    double hours = gtk_spin_button_get_value (GTK_SPIN_BUTTON (hselect));
    double minutes = gtk_spin_button_get_value (GTK_SPIN_BUTTON (mselect));
    const char *name;
    name = gtk_entry_buffer_get_text (GTK_ENTRY_BUFFER (gtk_entry_get_buffer (GTK_ENTRY (text_entry))));

    B_DATETIME datetime;
    datetime.day = slcDay;
    datetime.month = slcMon;
    datetime.year = slcYr;
    datetime.hour = (int) hours;
    datetime.minute = (int) minutes;

    // Format for Time and Text
    char entry[100];
    snprintf (entry, 100, "%d/%d/%d, %d:%d | %s",slcDay,slcMon,slcYr,(int) hours,(int) minutes,name);

    GtkWidget *entry_item, *entry_text;
    GtkEntryBuffer *entry_buffer;

    entry_item = gtk_list_box_row_new ();
    entry_buffer = gtk_entry_buffer_new(entry ,strlen(entry));
    entry_text = gtk_text_new_with_buffer (entry_buffer);
    gtk_list_box_row_set_child (GTK_LIST_BOX_ROW (entry_item), entry_text);
    gtk_list_box_append (GTK_LIST_BOX (list), entry_item);

    w_base ((char*) name, datetime);


    /*

        THIS IS WHERE THE DATABASE FUNCTIONALITY GOES, 
        SUCH AS ADDING AN EVENT TO A DATABASE
    
    */
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


void load_events (GtkWidget *ltr, B_EVENT* event_list)
{
    /*
        NEXT STEP:
        Once a Day has been selected, 
            utilize this method to add events to sidebar;
    
    */
   for 
}


//Event Dialogue Option
void entry_dialogue (GtkWidget *widget, gpointer data)
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
void day_marked (GtkWidget *widget, gpointer data)
{
    B_EVENT *events;
    GDateTime *tempDateTime = gtk_calendar_get_date (GTK_CALENDAR (widget));
    slcDay = g_date_time_get_day_of_month (tempDateTime);
    slcMon = g_date_time_get_month (tempDateTime);
    slcYr = g_date_time_get_year (tempDateTime);
    tdatetime.day = slcDay;
    tdatetime.month = slcMon;
    tdatetime.year = slcYr;
    events = r_base (tdatetime);
    remove_events(list);
    load_events(list, events);

    free(events);
}

//Main Window
void activate (GtkApplication *app, gpointer user_data)
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

    //Listed Events Are Applied Here
    list = gtk_list_box_new ();
    gtk_frame_set_child (GTK_FRAME (frame), list);
    gtk_grid_attach (GTK_GRID (grid), frame, 7, 0, 6, 2);

    entry_button = gtk_button_new_with_label ("Add Event");
    g_signal_connect (entry_button, "clicked", G_CALLBACK (entry_dialogue), NULL);
    gtk_grid_attach (GTK_GRID (grid), entry_button, 0, 2, 12, 1);

    gtk_widget_show (window);
}