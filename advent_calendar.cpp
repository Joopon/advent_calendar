#include <cairo.h> 
#include <gtk/gtk.h>
#include <iostream>
#include <vector>

#define NUM_DOORS 24

struct advent_door_t {
  cairo_surface_t *image;
  int image_width;
  bool open=false;
  ~advent_door_t() {
    cairo_surface_destroy(image);
  }
};

struct advent_calendar_t {
  cairo_surface_t *background;
  int background_width;
  std::vector<advent_door_t> *doors;
  advent_calendar_t() {
    doors = new std::vector<advent_door_t>(NUM_DOORS);
  }
  ~advent_calendar_t() {
    cairo_surface_destroy(background);
    delete(doors);
  }
};

struct advent_calendar_t *calendar;
GtkWidget *window;

static void do_drawing(cairo_t *);

static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, 
			      gpointer user_data)
{      
  do_drawing(cr);

  return FALSE;
}

static void do_drawing(cairo_t *cr)
{
  int window_width = gtk_widget_get_allocated_width (window);
  int window_height = gtk_widget_get_allocated_height (window);
  double door_width = window_width * 10 / 67;
  double door_height = door_width * 3/4;
  int offset = (int)(door_width/10);
  int init_offset_height = (int)((window_height - 4*door_height - 3*offset)/2);

  double background_scale = (double)(window_width)/(calendar->background_width);
  cairo_scale(cr, background_scale, background_scale);
  cairo_set_source_surface(cr, calendar->background, 0, 0);
  cairo_paint(cr);
  cairo_identity_matrix(cr);
  

  for(int j=0; j<4; j++) {
    for(int i=0; i<6; i++) {
      if((*calendar->doors)[i+j*6].open) {
	cairo_translate(cr, (int)(offset + i*(offset+door_width)), init_offset_height+j*(offset+door_height));
	double image_scale = door_width/((*calendar->doors)[i+j*6].image_width);
	cairo_scale(cr, image_scale, image_scale);
	cairo_set_source_surface(cr, (*calendar->doors)[i+j*6].image, 0, 0);
	cairo_paint(cr);
	cairo_identity_matrix(cr);
      }
    }
  }
}
int read_images() {
  calendar = new struct advent_calendar_t();
  calendar->background = cairo_image_surface_create_from_png("background.png");
  if(cairo_surface_status(calendar->background) != CAIRO_STATUS_SUCCESS) {
    std::cout << "Error: couldn't load background image\n";
    delete(calendar);
    return 1;
  }
  calendar->background_width = cairo_image_surface_get_width (calendar->background);

  for(int i=0; i<NUM_DOORS; i++) {
    (*calendar->doors)[i].image = cairo_image_surface_create_from_png((std::to_string(i+1)+".png").c_str());
    if(cairo_surface_status((*calendar->doors)[i].image) != CAIRO_STATUS_SUCCESS) {
      std::cout << "Error: couldn't load door image " << i << '\n';
      delete(calendar);
      return 1;
    }
    (*calendar->doors)[i].image_width = cairo_image_surface_get_width((*calendar->doors)[i].image);
  }
  return 0;
}
  
  
int main(int argc, char *argv[])
{
  GtkWidget *darea;

  if(read_images()!=0) {
    return 0;
  }
  
  gtk_init(&argc, &argv);

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

  darea = gtk_drawing_area_new();
  gtk_container_add(GTK_CONTAINER (window), darea);

  g_signal_connect(G_OBJECT(darea), "draw", 
		   G_CALLBACK(on_draw_event), NULL); 
  g_signal_connect(window, "destroy",
		   G_CALLBACK (gtk_main_quit), NULL);

  gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 220); 
  gtk_window_set_title(GTK_WINDOW(window), "Image");

  gtk_widget_show_all(window);

  gtk_main();

  delete(calendar);
  return 0;
}
