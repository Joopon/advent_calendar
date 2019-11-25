LIBDIR=-L/usr/lib/
LIB=-lcairo -lgtk-3 -lgobject-2.0
INC=-I/usr/include/gtk-3.0/ -I/usr/include/cairo -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include/ -I/usr/include/pango-1.0 -I/usr/include/harfbuzz/ -I/usr/include/gdk-pixbuf-2.0/ -I/usr/include/atk-1.0/

default:	mainmake

mainmake: advent_calendar.cpp
	g++ -Wall $(LIBDIR) $(LIB) $(INC) advent_calendar.cpp
