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
	PlaceMines();
	CreateGrid();
	DisplayStats();

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

void CreateGrid()
{
	GtkWidget* button;

	// destroy and recreate grid on restart
	if (grid)
	{
		gtk_widget_destroy(grid);
	}

	grid = gtk_grid_new();

	// fill the grid with buttons
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			mineButton* mine = malloc(sizeof(*mine));
			mine->x = i;
			mine->y = j;
			mine->flagged = 0;
			button = gtk_button_new();
			mine->widget = button;
			g_signal_connect(button, "button_press_event", G_CALLBACK(ClickField), mine);
			gtk_grid_attach(GTK_GRID(grid), button, j, i, 1, 1);
			helper = mine;
		}
	}

	gtk_widget_show_all(grid);
	gtk_box_pack_start(GTK_BOX(vbox), grid, FALSE, FALSE, 0);

	// display stats at the bottom
	if (statsLabel)
	{
		gtk_widget_destroy(statsLabel);
	}

	statsLabel = gtk_label_new("");
	gtk_widget_show(statsLabel);
	gtk_box_pack_start(GTK_BOX(vbox), statsLabel, FALSE, FALSE, 10);
}

void PlaceMines()
{
	// place bombs on random fields
	srand(time(0));
	int bombsplaced = 0;
	while (bombsplaced < bombs)
	{
		int x = (rand() % (rows - 1)) + 1;
		int y = (rand() % (cols - 1)) + 1;
		if (field[x][y] != -1)
		{
			field[x][y] = -1;
			bombsplaced++;

			// increment bomb count of neighbouring fields
			for (int m = -1; m <= 1; m++)
			{
				for (int n = -1; n <= 1; n++)
				{
					if (isInsideBounds(x + m, y + n) && field[x + m][y + n] != -1)
					{
						field[x + m][y + n]++;
					}
				}
			}
		}
	}
}

int isInsideBounds(int x, int y)
{
	return x >= 0 && x < rows&& y >= 0 && y < cols;
}