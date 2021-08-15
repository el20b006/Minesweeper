#include <gtk/gtk.h>
#include "header.h"

mineButton *helper;

int main(int argc, char **argv)
{
	GtkWidget *window;

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

void CreateMenu(GtkWidget *vbox)
{
	GtkWidget *menubar;
	GtkWidget *menu;
	GtkWidget *menuItem;

	GtkWidget *radioMenu;
	GSList *group = NULL;
	GtkWidget *radioItem;

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
	gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(radioItem), TRUE);
	gtk_menu_shell_append(GTK_MENU_SHELL(radioMenu), radioItem);
	g_signal_connect(G_OBJECT(radioItem), "activate", G_CALLBACK(SetDifficulty), (gpointer)0);

	radioItem = gtk_radio_menu_item_new_with_label(group, "Normal");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(radioItem));
	gtk_menu_shell_append(GTK_MENU_SHELL(radioMenu), radioItem);
	g_signal_connect(G_OBJECT(radioItem), "activate", G_CALLBACK(SetDifficulty), (gpointer)1);

	radioItem = gtk_radio_menu_item_new_with_label(group, "Hard");
	group = gtk_radio_menu_item_get_group(GTK_RADIO_MENU_ITEM(radioItem));
	gtk_menu_shell_append(GTK_MENU_SHELL(radioMenu), radioItem);
	g_signal_connect(G_OBJECT(radioItem), "activate", G_CALLBACK(SetDifficulty), (gpointer)2);

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
	GtkWidget *button;

	// destroy and recreate grid on restart
	if (grid)
	{
		gtk_widget_destroy(GTK_WIDGET(grid));
	}

	grid = gtk_grid_new();

	// fill the grid with buttons
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			mineButton *mine = malloc(sizeof(*mine));
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

	gtk_widget_show_all(GTK_WIDGET(grid));
	gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(grid), FALSE, FALSE, 0);

	// display stats at the bottom
	if (statsLabel)
	{
		gtk_widget_destroy(statsLabel);
	}

	statsLabel = gtk_label_new("");
	gtk_widget_show(statsLabel);
	gtk_box_pack_start(GTK_BOX(vbox), statsLabel, FALSE, FALSE, 10);
}

void DisplayStats() 
{
	char buffer[30];
	sprintf_s(buffer, sizeof(buffer), "Bombs left: %d \nMoves: %d", flagsLeft, moves);
	gtk_label_set_label(GTK_LABEL(statsLabel), buffer);
}

void RestartGame()
{
	free(helper);
	
	// reset stats
	bombsFound = 0;
	moves = 0;

	// set the whole board to empty
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			field[i][j] = 0;
		}
	}

	// check difficulty and set parameters
	if (difficulty == 1)
	{
		rows = 16;
		cols = 16;
		bombs = 40;
		flagsLeft = 40;
	}
	else if (difficulty == 2)
	{
		rows = 16;
		cols = 30;
		bombs = 99;
		flagsLeft = 99;
	}
	else
	{
		rows = 9;
		cols = 9;
		bombs = 10;
		flagsLeft = 10;
	}

	// place new bombs and render grid
	PlaceMines();
	CreateGrid();
	DisplayStats();
}

void SetDifficulty(GtkWidget *widget, gpointer data)
{
	difficulty = (int)(*(int*)data);
	RestartGame();
}

void ShowHighscores()
{
	FILE *file;
	errno_t err;
	char buf[100]; // assuming that a single line is never more than 100 characters
	char highscores[10000];
	strcpy_s(highscores, 100, "Your past scores were:\n\n");
	if (err = fopen_s(&file, ".mines", "r") != 0)
	{
		printf("Error opening file!");
	}
	else
	{
		while (fgets(buf, 100, file) != NULL)
		{
			strcat_s(highscores, 1000, buf);
		}
		fclose(file);
	}

	// create dialog
	GtkWidget *highscoresDialog;
	highscoresDialog = gtk_dialog_new_with_buttons("Highscores", GTK_WINDOW(vbox), GTK_DIALOG_MODAL, "Close", GTK_RESPONSE_CLOSE, NULL);

	// get content area where widgets can be added
	GtkWidget *dialogVbox = gtk_dialog_get_content_area(GTK_DIALOG(highscoresDialog));

	// create text
	GtkWidget *text = gtk_label_new(highscores);
	gtk_widget_show(text);
	gtk_box_pack_start(GTK_BOX(dialogVbox), text, FALSE, FALSE, 10);

	gtk_dialog_run(GTK_DIALOG(highscoresDialog));
	gtk_widget_destroy(highscoresDialog);
}

void ShowHelp()
{
#ifdef _WIN32
	system("explorer https://en.wikipedia.org/wiki/Minesweeper_(video_game)");
#elif defined __unix__
	system("xdg-open 'https://en.wikipedia.org/wiki/Minesweeper_(video_game)'");
#elif define __APPLE
	system("open https://en.wikipedia.org/wiki/Minesweeper_(video_game)");
#endif
}

void ShowAbout()
{
	// create dialog
	GtkWidget *aboutDialog;
	aboutDialog = gtk_dialog_new_with_buttons("About", GTK_WINDOW(vbox), GTK_DIALOG_MODAL, "Close", GTK_RESPONSE_CLOSE, NULL);

	// get content area where widgets can be added
	GtkWidget *dialogVbox = gtk_dialog_get_content_area(GTK_DIALOG(aboutDialog));

	// create text
	GtkWidget *text = gtk_label_new("Minesweeper\nVersion 1.0\nMade by Sarah Froeler\n07/21");
	gtk_widget_show(text);
	gtk_box_pack_start(GTK_BOX(dialogVbox), text, FALSE, FALSE, 10);

	gtk_dialog_run(GTK_DIALOG(aboutDialog));
	gtk_widget_destroy(aboutDialog);
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

void ClickField(GtkWidget *widget, GdkEventButton *event, gpointer data)
{
	mineButton *mine = (mineButton*)data;

	if (event->type == GDK_BUTTON_PRESS && event->button == 3) // right mouse button
	{
		FlagField(mine, field[mine->x][mine->y]);
	}
	else if (event->type == GDK_BUTTON_PRESS && event->button == 1) // left mouse button
	{
		if (mine->flagged == 1)	// flagged fields are not clickable
		{
			return;
		}
		else if (field[mine->x][mine->y] == -1) // clicked on a mine
		{
			GtkWidget *image = gtk_image_new_from_file("bomb.png");
			gtk_button_set_image(GTK_BUTTON(mine->widget), image);

			gtk_widget_set_sensitive(GTK_WIDGET(grid), FALSE);
			GtkWidget *loseDialog;
			loseDialog = gtk_dialog_new_with_buttons("You lost!", GTK_WINDOW(vbox), GTK_DIALOG_MODAL, "Quit", 0, "Restart", 1, NULL);
			g_signal_connect(GTK_DIALOG(loseDialog), "response", G_CALLBACK(on_response), NULL);
			gtk_dialog_run(GTK_DIALOG(loseDialog));
			gtk_widget_destroy(loseDialog);
		}
		else
		{
			DisplayButtonInfo(mine, field[mine->x][mine->y]);
			moves++;
		}
	}

	DisplayStats();
}

void FlagField(mineButton *mine, int bombsNearby)
{
	GtkWidget *image = gtk_image_new_from_file("flag.png");

	if (mine->flagged == 0 && flagsLeft > 0)	// place flag
	{
		gtk_button_set_image(GTK_BUTTON(mine->widget), image);
		mine->flagged = 1;
		flagsLeft--;
		moves++;
		if (bombsNearby == -1)
		{
			bombsFound++;
			if (bombsFound == bombs) // all bombs flagged
			{
				DisplayGameWon();
			}
		}
	}
	else if (mine->flagged == 1)	// remove flag
	{
		gtk_button_set_image(GTK_BUTTON(mine->widget), NULL);
		mine->flagged = 0;
		flagsLeft++;
		moves++;
		if (bombsNearby == -1)
		{
			bombsFound--;
		}
	}
}

void DisplayButtonInfo(mineButton *mine, int bombsNearby)
{
	// ignore already opened fields
	if (!gtk_widget_get_sensitive(mine->widget))
	{
		return;
	}

	gtk_widget_set_sensitive(mine->widget, FALSE);

	if (bombsNearby == 0)	// open adjacent fields
	{
		for (int m = -1; m <= 1; m++)
		{
			for (int n = -1; n <= 1; n++)
			{
				if (isInsideBounds(mine->x + m, mine->y + n) && field[mine->x + m][mine->y + n] != -1)
				{
					GtkWidget *buttonToUse;
					buttonToUse = gtk_grid_get_child_at(GTK_GRID(grid), mine->y + n, mine->x + m);
					mineButton *neighborMine = malloc(sizeof(*neighborMine));
					neighborMine->widget = buttonToUse;
					neighborMine->x = mine->x + m;
					neighborMine->y = mine->y + n;
					DisplayButtonInfo(neighborMine, field[neighborMine->x][neighborMine->y]);
					free(neighborMine);
				}
			}
		}
	}
	else // display number of nearby bombs
	{
		GtkWidget *image = gtk_image_new_from_file("1.png");
		switch (bombsNearby)
		{
		case 1:
			image = gtk_image_new_from_file("1.png");
			break;
		case 2:
			image = gtk_image_new_from_file("2.png");
			break;
		case 3:
			image = gtk_image_new_from_file("3.png");
			break;
		case 4:
			image = gtk_image_new_from_file("4.png");
			break;
		case 5:
			image = gtk_image_new_from_file("5.png");
			break;
		case 6:
			image = gtk_image_new_from_file("6.png");
			break;
		case 7:
			image = gtk_image_new_from_file("7.png");
			break;
		case 8:
			image = gtk_image_new_from_file("8.png");
			break;
		default:
			// something went wrong
			break;
		}
		gtk_button_set_image(GTK_BUTTON(mine->widget), image);
	}
}

int isInsideBounds(int x, int y)
{
	return x >= 0 && x < rows&& y >= 0 && y < cols;
}

void DisplayGameWon()
{
	// make grid non-interactive
	gtk_widget_set_sensitive(GTK_WIDGET(grid), FALSE);

	// create dialog
	GtkWidget *winDialog;
	winDialog = gtk_dialog_new_with_buttons("You won!", GTK_WINDOW(vbox), GTK_DIALOG_MODAL, "Add to highscores", 2, NULL);

	// get content area where widgets can be added
	GtkWidget *dialogVbox = gtk_dialog_get_content_area(GTK_DIALOG(winDialog));

	// create text input widget and add to dialog
	GtkWidget *input;
	input = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(input), "Enter your name");
	gtk_widget_show(input);
	gtk_box_pack_start(GTK_BOX(dialogVbox), input, FALSE, FALSE, 0);

	gchar *pinput = (gchar*)gtk_entry_get_text(GTK_ENTRY(input));
	g_signal_connect(GTK_DIALOG(winDialog), "response", G_CALLBACK(on_response), (gpointer) pinput);
	gtk_dialog_run(GTK_DIALOG(winDialog));
	gtk_widget_destroy(winDialog);
}

void on_response(GtkDialog *dialog, gint response_id, gpointer data) // deal with buttons on game won/lost windows
{
	char *name = (char*)data;
	switch (response_id)
	{
	case 0:
		gtk_main_quit();
		break;
	case 1:
		RestartGame();
		break;
	case 2:
		AddHighscore(name);
		break;
	default:
		break;
	}
}

void AddHighscore(char *name)
{
	char *difficultyText = "Easy";
	switch (difficulty)
	{
	case 0:
		difficultyText = "Easy";
		break;
	case 1:
		difficultyText = "Normal";
		break;
	case 2:
		difficultyText = "Hard";
		break;
	default:
		break;
	}

	FILE *file;
	errno_t err;
	if (err = fopen_s(&file, ".mines", "a") != 0) // file is saved in program directory to avoid troubles with OS differences or permissions
	{
		printf("Error opening file!");
	}
	else
	{
		fprintf(file, "%s - %d moves - Difficulty: %s \n", name, moves, difficultyText);
		fclose(file);
	}
}


/*
* TODO / Issues:
*
* Memory management is scuffed, there's at least one mem leak when opening adjacent fields due to the hacky button accessing
* Some nice-to-have improvements (track time in addition to moves, sort highscores by moves/time)
*/