#pragma once

GtkWidget* vbox;

void CreateMenu(GtkWidget* vbox_main);
void RestartGame();
void SetDifficulty(GtkWidget* widget, gpointer data);
void ShowHighscores();
void ShowHelp();
void ShowAbout();