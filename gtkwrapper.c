#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>


char i[] = "9:00-13:00; Dentist Appt.";
int slcDay, slcMon, slcYr;

/* GtkWidget new_entry (char* i, GDateTime *date)
{
    GtkEntryBuffer TempBuffer;
    return entry;
} */

void refresh_events ()
{

}


static void day_marked (GtkWidget *widget, gpointer data)
{
    GDateTime *tempDateTime = gtk_calendar_get_date (GTK_CALENDAR (widget));
    slcDay = g_date_time_get_day_of_month (tempDateTime);
    slcMon = g_date_time_get_month (tempDateTime);
    slcYr = g_date_time_get_year (tempDateTime);

    g_print ("Day %d, Month %d, Year %d\n", slcDay, slcMon, slcYr);
}

static void activate (GtkApplication *app, gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *grid, *calendar, *list, *frame, *row1, *text1;
    GtkEntryBuffer *entryBuffer1;
    
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

    row1 = gtk_list_box_row_new ();
    entryBuffer1 = gtk_entry_buffer_new (i, sizeof (i));
    text1 = gtk_text_new_with_buffer (entryBuffer1);
    gtk_list_box_row_set_child (GTK_LIST_BOX_ROW (row1), text1);
    gtk_list_box_append (GTK_LIST_BOX (list), row1);

    row1 = gtk_list_box_row_new ();
    entryBuffer1 = gtk_entry_buffer_new (i, sizeof (i));
    text1 = gtk_text_new_with_buffer (entryBuffer1);
    gtk_list_box_row_set_child (GTK_LIST_BOX_ROW (row1), text1);
    gtk_list_box_append (GTK_LIST_BOX (list), row1);

    


    gtk_widget_show (window);
    return;
}


int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run(G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}   
