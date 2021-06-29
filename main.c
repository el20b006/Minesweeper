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

	// fill window
	CreateMenu(vbox);

	// display and run everything
	gtk_widget_show_all(window);
	gtk_main();


	return 0;
}

void CreateMenu(GtkWidget* vbox)
{
	GtkWidget* menubar;
	GtkWidget* menu;
	GtkWidget* menuItem;

	GtkWidget* radioMenu;
	GSList* group = NULL;
	GtkWidget* radioItem;

	menubar = gtk_menu_bar_new();

	// game menu
	menu = gtk_menu_new();

	menuItem = gtk_menu_item_new_with_label("Game");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuItem);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);

	menuItem = gtk_menu_item_new_with_label("Restart");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
	g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(RestartGame), NULL);

	menuItem = gtk_menu_item_new_with_label("Difficulty");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

	radioMenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), radioMenu);

	radioItem = gtk_radio_menu_item_new_with_label(group, "Easy");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(radioItem));
	gtk_check_menu_item_set_active(GTK_RADIO_MENU_ITEM(radioItem), TRUE);
	gtk_menu_shell_append(GTK_MENU_SHELL(radioMenu), radioItem);
	g_signal_connect(G_OBJECT(radioItem), "activate", G_CALLBACK(SetDifficulty), 0);

	radioItem = gtk_radio_menu_item_new_with_label(group, "Normal");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(radioItem));
	gtk_menu_shell_append(GTK_MENU_SHELL(radioMenu), radioItem);
	g_signal_connect(G_OBJECT(radioItem), "activate", G_CALLBACK(SetDifficulty), 1);

	radioItem = gtk_radio_menu_item_new_with_label(group, "Hard");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(radioItem));
	gtk_menu_shell_append(GTK_MENU_SHELL(radioMenu), radioItem);
	g_signal_connect(G_OBJECT(radioItem), "activate", G_CALLBACK(SetDifficulty), 2);

	menuItem = gtk_menu_item_new_with_label("Highscores");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
	g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(ShowHighscores), NULL);

	menuItem = gtk_menu_item_new_with_label("Quit");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
	g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(gtk_main_quit), NULL);

	// help menu
	menu = gtk_menu_new();

	menuItem = gtk_menu_item_new_with_label("Help");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menuItem);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);

	menuItem = gtk_menu_item_new_with_label("How to play");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
	g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(ShowHelp), NULL);

	menuItem = gtk_menu_item_new_with_label("About");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
	g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(ShowAbout), NULL);

	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
}
