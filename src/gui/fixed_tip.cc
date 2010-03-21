// -*- C++ -*-

/*
 * Creado:     Mon Dec  2 16:28:03 CET 2002
 * Modificado: Mon Dec  2 16:28:03 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "fixed_tip.h"
#include <gdk/gdkx.h>

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Fixed_tip::Fixed_tip()
{
  label = NULL;
  screen_width = 0;
  screen_height = 0;
  screen_number = DefaultScreen (gdk_display);

  build (); // crea interfaz y salva this
}

/*
 * Destructor.
 */
Fixed_tip::~Fixed_tip()
{
  //std::cout << "~Fixed_tip()\n";
}

/*
 * Asignacion.
 */
GtkWidget* 
Fixed_tip::create_gui()
{
  if (the_widget == NULL)
    {      
      the_widget = gtk_window_new (GTK_WINDOW_POPUP);
#ifdef HAVE_GTK_MULTIHEAD
      {
        GdkScreen *gdk_screen;
	
        gdk_screen = gdk_display_get_screen (gdk_display_get_default (),
                                             screen_number);
        gtk_window_set_screen (GTK_WINDOW (the_widget),
                               gdk_screen);
        screen_width = gdk_screen_get_width (gdk_screen);
        screen_height = gdk_screen_get_height (gdk_screen);
      }
#else
      screen_width = gdk_screen_width ();
      screen_height = gdk_screen_height ();      
#endif
      
      gtk_widget_set_app_paintable (the_widget, TRUE);
      //gtk_window_set_policy (GTK_WINDOW (the_widget), FALSE, FALSE, TRUE);
      gtk_window_resize (GTK_WINDOW (the_widget), 1, 1); // gtk2

      gtk_widget_set_name (the_widget, "gtk-tooltips");
      gtk_container_set_border_width (GTK_CONTAINER (the_widget), 4);

      g_signal_connect (G_OBJECT (the_widget), 
			"expose_event",
			G_CALLBACK (expose_handler),
			static_cast<gpointer>(this));

      label = gtk_label_new (NULL);
      gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
      gtk_misc_set_alignment (GTK_MISC (label), 0.5, 0.5);
      gtk_widget_show (label);
      
      gtk_container_add (GTK_CONTAINER (the_widget), label);

      g_signal_connect (G_OBJECT (the_widget),
			"destroy",
			G_CALLBACK (gtk_widget_destroyed),
			&the_widget);
    }

  return the_widget;
}

void
Fixed_tip::show(int x, int y, const std::string& t, int tout)
{
  int w, h;

  gtk_label_set_markup (GTK_LABEL (label), t.c_str());
  
  /* FIXME should also handle Xinerama here, just to be
   * really cool
   */
  gtk_window_get_size (GTK_WINDOW (the_widget), &w, &h);
  if ((x + w) > screen_width)
    x -= (x + w) - screen_width;
  
  gtk_window_move (GTK_WINDOW (the_widget), x, y);

  gtk_widget_show (the_widget);

  if (tout > 0) // Si se pide auto-eliminacion
    {
      g_timeout_add(tout, tip_remover, this);
    }
}


bool 
Fixed_tip::expose_handler (GtkTooltip *tooltip, GdkEvent *event, 
			   gpointer data)
{
  GtkWidget* w = static_cast<Fixed_tip*>(data)->the_widget;

  gtk_paint_flat_box (w->style, w->window,
                      GTK_STATE_NORMAL, GTK_SHADOW_OUT, 
                      NULL, w, "tooltip",
                      0, 0, -1, -1);

  return FALSE;
}

gint
Fixed_tip::tip_remover(gpointer data)
{
  Fixed_tip* ft = reinterpret_cast<Fixed_tip*>(data);

  ft->hide();
  delete ft;

  return false;
}
