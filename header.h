#pragma once

GtkGrid* grid = NULL;
GtkWidget* vbox;
GtkWidget* statsLabel = NULL;
int rows = 9;
int cols = 9;
int bombs = 10;
int bombsFound = 0;
int flagsLeft = 10;
int difficulty = 0;
int moves = 0;

/*
*  create field with max possible size
*  only fields according to difficulty will be used
*  static array is used instead of malloc because of neglectible size and how much easier it is for 2d arrays
*/
int field[16][30] = { 0 };

typedef struct
{
	GtkWidget* widget;
	int x;
	int y;
	int flagged;
} mineButton;

void CreateMenu(GtkWidget* vbox_main);
void CreateGrid();
void DisplayStats();
void RestartGame();
void SetDifficulty(GtkWidget* widget, gpointer data);
void ShowHighscores();
void ShowHelp();
void ShowAbout();
void PlaceMines();
void ClickField(GtkWidget* widget, GdkEventButton* event, gpointer data);
void FlagField(mineButton* mine, int bombsNearby);
void DisplayButtonInfo(mineButton* mine, int bombsNearby);
int isInsideBounds(int x, int y);
void DisplayGameWon();
void on_response(GtkDialog* dialog, gint response_id, gpointer data);
void AddHighscore(char* name);