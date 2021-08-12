#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>




int main ()
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("org.gtk.calendar", G_APPLICATION_FLAGS_NONE);
    return 0;
}
