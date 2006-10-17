// -*- C++ -*-

/*
 * Creado:     Wed Jan 30 10:55:29 CET 2002
 * Modificado: Wed Jan 30 10:55:29 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "window.h"

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Window::Window(GtkWindowType t): wtype(t)
{
  sh = 0;
}

Window::~Window()
{
  //std::cerr << "Window destruida.\n";
}

void
Window::set_title (const std::string& t)
{ 
  gtk_window_set_title (GTK_WINDOW (the_widget), t.c_str ());
}

void
Window::set_icon (const char** xpm)
{ 
#if 0
  GdkPixmap *pixmap;
  GdkBitmap *mask;

  /* Now on to the xpm stuff */
  pixmap = gdk_pixmap_colormap_create_from_xpm_d (the_widget->window, NULL,
						  &mask, NULL, 
						  const_cast<gchar**>(xpm));
  gdk_window_set_icon (the_widget->window, NULL, pixmap, mask);
#endif

  GdkPixbuf* icon;

  icon=gdk_pixbuf_new_from_xpm_data(xpm);
  gtk_window_set_icon(GTK_WINDOW(the_widget), icon);
  g_object_unref(icon);
}

void 
Window::set_transient_for (Window* w)
{
  gtk_window_set_transient_for (GTK_WINDOW(the_widget),
				GTK_WINDOW(w->the_widget));
}

void 
Window::set_transient_for (const Window& w)
{
  gtk_window_set_transient_for (GTK_WINDOW(the_widget),
				GTK_WINDOW(w.the_widget));
}

gint
Window::wm_no_close(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  std::cerr << "No Close\n";
  return true;
}

gint
Window::wm_do_close(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  Window* w = reinterpret_cast<Window*>(data);

  std::cerr << "close\n";
  gtk_widget_destroy(w->the_widget);
  gtk_main_quit();
  return false;
}

gint
Window::wm_do_hide(GtkWidget *widget, GdkEvent *event, gpointer data)
{
  Window* w = reinterpret_cast<Window*>(data);

  std::cerr << "hide\n";
  gtk_widget_hide(w->the_widget);
  //gtk_main_quit();
  return false;
}

void
Window::set_wm_close(Close_action a)
{
  //static guint sh = 0;

  if (sh != 0)
  {
    g_signal_handler_disconnect (G_OBJECT (the_widget), sh);
    sh = 0;
  }

  switch (a)
    {
    case DESTROY:
      sh = g_signal_connect (G_OBJECT (the_widget), "delete_event",
			     GTK_SIGNAL_FUNC (wm_do_close), this);
      break;
    case HIDE:
      sh = g_signal_connect (G_OBJECT (the_widget), "delete_event",
			     GTK_SIGNAL_FUNC (wm_do_hide), this);
      break;
    case IGNORE:
      sh = g_signal_connect (G_OBJECT (the_widget), "delete_event",
			     GTK_SIGNAL_FUNC (wm_no_close), this);
      break;
    default:
      break;
    }
}
