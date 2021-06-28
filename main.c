#include <gtk/gtk.h>
#include "header.h"

int main(int argc, char** argv)
{
	GtkWidget* window;

	gtk_init(&argc, &argv);

	// create main window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Minesweeper");

	// disable window resize
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);

	// connect destroy event to main window
	g_signal_connect(G_OBJECT(window), "delete_event", G_CALLBACK(gtk_main_quit), NULL);

	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	gtk_container_add(GTK_CONTAINER(window), vbox);


	return 0;
}
