#include "include/fuxgtk.h"
#include <gtk/gtk.h>
#include <stdio.h>

GtkWidget *window;

void _quit (GtkWidget *widget, gpointer data) {
    exit(0);
}
/* 

*/
int day_of_week (int d, int m, int y) {
    static int t[] = { 0, 3, 2, 5, 0, 3,
                       5, 1, 4, 6, 2, 4 };
    y -= m < 3;
    return ( y + y / 4 - y / 100 +
             y / 400 + t[m - 1] + d) % 7;
}



static void cal_draw (cairo_t *cr, int c_wid, int c_hei, int month, int year) {
    int rect_h = (c_hei-10) / 6;
    int rect_w = (c_wid-10) / 7;

    int d_start = day_of_week(1, month, year);
    int x_off = 10 + (d_start * rect_w);
    int y_off = 20;
    printf("%d\n",d_start);
    cairo_select_font_face (cr, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 13);

    for (int i=0;i<6;i++) {
        for (int j=0;j<7;j++) {
            cairo_rectangle (cr, rect_w * j + 5, rect_h * i + 5, rect_w, rect_h);
        }
    }
    for (int i=0;i<31;i++) {
        char str[5];
        int n;

        cairo_move_to (cr, x_off, y_off);
        n = sprintf(str, "%d", i+1);
        cairo_show_text (cr, str);

        if (x_off + rect_w >= c_wid - 5 ){
            y_off += rect_h;
            x_off = 10;
        }else {
            x_off += rect_w;
        }
    }

}

static void draw_func (GtkDrawingArea *area, cairo_t *cr, int width, int height, gpointer data) {
    GdkRGBA color;
    GtkStyleContext *context;

    context = gtk_widget_get_style_context (GTK_WIDGET (area));

    cal_draw (cr, width, height,8,2022);

    gtk_style_context_get_color (context, &color);
    gdk_cairo_set_source_rgba (cr, &color);

    cairo_stroke(cr);
}



void aactivate (GtkApplication *app, gpointer user_data) {
    GtkWidget *grid, *quit_button, *calendar;
    
    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Calendar");
    gtk_window_set_default_size (GTK_WINDOW (window), 600, 600);

    grid = gtk_grid_new ();
    gtk_window_set_child (GTK_WINDOW (window), grid);

    calendar = gtk_drawing_area_new ();
    gtk_drawing_area_set_content_height (GTK_DRAWING_AREA (calendar), 500);
    gtk_drawing_area_set_content_width (GTK_DRAWING_AREA (calendar), 600);
    gtk_drawing_area_set_draw_func (GTK_DRAWING_AREA (calendar), draw_func, NULL, NULL);
    gtk_grid_attach (GTK_GRID (grid), calendar, 0, 0, 3, 1);

    quit_button = gtk_button_new_with_label ("Quit");
    g_signal_connect (quit_button, "clicked", G_CALLBACK (_quit), NULL);
    gtk_grid_attach (GTK_GRID (grid), quit_button, 1, 1, 1, 1);

    gtk_widget_show (window);
}