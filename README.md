# Minesweeper

This is a game of Minesweeper for Windows.

Different difficulties can be chosen under Game > Difficulty
Past Scores can be seen under Game > Highscores
Instructions how to play can be seen under Help > How to play

V1.0 07/21 Fröler

----------------------------------------------------

git repo can be found at
github.com/el20b006/Minesweeper

----------------------------------------------------

To build in Visual Studio 2019 on Windows:
- install and setup gtk 3.0
- include directories: 
	\gtk-build\gtk\x64\release\include\
		gtk-3.0
		cairo
		pango-1.0
		glib-2.0
		gdk-pixbuf-2.0
		atk-1.0
		harfbuzz
		libpng16
		freetype2
	\gtk-build\gtk\x64\release\include
	\gtk-build\gtk\x64\release\lib\glib-2.0\include
- linker dependencies:
	C:\gtk-build\gtk\x64\release\lib\
		gdk-3.0.lib
		gtk-3.0.lib
		atk-1.0.lib
		gio-2.0.lib
		cairo.lib
		gobject-2.0.lib
		gmodule-2.0.lib
		gthread-2.0.lib
		glib-2.0.lib
		intl.lib
		pango-1.0.lib
		pangocairo-1.0.lib
		pangowin32-1.0.lib
		gdk_pixbuf-2.0.lib
		gtk-3.lib
		gdk-3.lib
		libpng16.lib
		libpng16_static.lib
		gettextpo.lib
- .dll's needed to run the .exe:
	atk-1.0.dll
	cairo.dll
	cairo-gobject.dll
	epoxy-0.dll
	ffi-7.dll
	fontconfig.dll
	freetype-6.dll
	fribidi-0.dll
	gdk_pixbuf-2.0-0.dll
	gdk-3-vs16.dll
	gio-2.0-0.dll
	gobject-2.0-0.dll
	gtk-3-vs16.dll
	harfbuzz.dll
	iconv.dll
	intl.dll
	libpng16.dll
	libxml2.dll
	pango-1.0-0.dll
	pangocairo-1.0-0.dll
	pangoft2-1.0-0.dll
	pangowin32-1.0-0.dll
	zlib1.dll