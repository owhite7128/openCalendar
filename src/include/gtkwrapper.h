#ifndef GTKWRAPPER_H
#define GTKWRAPPER_H
#include <gtk/gtk.h>

void activate (GtkApplication *app, gpointer user_data);
void day_marked (GtkWidget *widget, gpointer data);
void entry_dialogue (GtkWidget *widget, gpointer data);
void load_events ();
void add_entry ();


#endif