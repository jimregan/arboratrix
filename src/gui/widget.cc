// -*- C++ -*-

/*
 * Creado:     Tue Jan 29 18:56:19 CET 2002
 * Modificado: Tue Jan 29 18:56:19 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "widget.h"

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Widget::Widget()
{
  registered = false;
  the_widget = 0;
}

/*
 * Destructor.
 */
Widget::~Widget()
{
}

/*
 * destroy_widget
 */
void
Widget::destroy_widget()
{
  if (GTK_IS_WIDGET(the_widget))
    gtk_widget_destroy(the_widget);
    //g_object_unref(G_OBJECT(the_widget));
}

/*
 * save_this
 */
void
Widget::save_this()
{
  g_return_if_fail(the_widget != NULL);

  registered = true;
  g_object_set_data(G_OBJECT(the_widget), "the_widget", this);
}

/*
 * get_this
 */
Widget*
Widget::get_this(GtkWidget* w)
{
  if (w != NULL)
    return reinterpret_cast<Widget*>(g_object_get_data(G_OBJECT(w), "the_widget"));
  else
    return NULL;
}

/*
 * lookup_widget
 */
GtkWidget*
Widget::lookup_widget(const std::string& widget_name)
{
  GtkWidget* widget = the_widget;
  GtkWidget* parent;
  GtkWidget* found_widget;

  for(;;)
    {
      if (GTK_IS_MENU(widget))
        parent = gtk_menu_get_attach_widget(GTK_MENU(widget));
      else
        parent = widget->parent;
      if (parent == NULL)
        break;
      widget = parent;
    }

  found_widget =(GtkWidget*) g_object_get_data(G_OBJECT(widget),
						 widget_name.c_str());
  if (!found_widget)
    g_warning("Widget not found: %s", widget_name.c_str());

  return found_widget;
}

/*
 * lookup_widget
 */
GtkWidget*
Widget::lookup_widget(GtkWidget* widget, const std::string& widget_name)
{
  //GtkWidget* widget = the_widget;
  GtkWidget* parent;
  GtkWidget* found_widget;

  for(;;)
    {
      if (GTK_IS_MENU(widget))
        parent = gtk_menu_get_attach_widget(GTK_MENU(widget));
      else
        parent = widget->parent;
      if (parent == NULL)
        break;
      widget = parent;
    }

  found_widget =(GtkWidget*) g_object_get_data(G_OBJECT(widget),
						 widget_name.c_str());
  if (!found_widget)
    g_warning("Widget not found: %s", widget_name.c_str());

  return found_widget;
}

void
Widget::change_mouse_shape(int w, int h, char mask[], char bits[])
{
  GdkCursor* cursor;
  GdkPixmap* source;
  GdkPixmap* mask_pxm;
  GdkColor fg = { 0, 0, 0, 0 }; /* Black. */
  GdkColor bg = { 0, 65535, 65535, 65535 }; /* White. */

  source = gdk_bitmap_create_from_data(NULL, bits, w, h);
  mask_pxm = gdk_bitmap_create_from_data(NULL,mask, w, h);

  cursor = gdk_cursor_new_from_pixmap(source, mask_pxm, &fg, &bg, 1, 1);
  g_object_unref(source);
  g_object_unref(mask_pxm);

  g_return_if_fail(the_widget != NULL);

  if (not GTK_WIDGET_REALIZED(the_widget))
    gtk_widget_realize(the_widget);

  gdk_window_set_cursor(the_widget->window, cursor);
  GTK_WIDGET_SET_FLAGS(the_widget, GTK_CAN_FOCUS);
}

void
Widget::change_mouse_shape(CursorType ct)
{
  GdkCursor* cursor = gdk_cursor_new(ct);

  g_return_if_fail(the_widget != NULL);

  if (not GTK_WIDGET_REALIZED(the_widget))
    gtk_widget_realize(the_widget);

  gdk_window_set_cursor(the_widget->window, cursor);
  gdk_cursor_unref(cursor);
}

void
Widget::connect_callbacks()
{
  g_return_if_fail(the_widget != NULL);

  g_signal_connect((gpointer) the_widget, "expose_event", 
		    G_CALLBACK(expose_event), this);

  g_signal_connect((gpointer) the_widget, "button_press_event", 
		    G_CALLBACK(button_press_event), this);

  g_signal_connect((gpointer) the_widget, "button_release_event", 
		    G_CALLBACK(button_release_event), this);

  g_signal_connect((gpointer) the_widget, "motion_notify_event", 
		    G_CALLBACK(motion_notify_event), this);

  g_signal_connect((gpointer) the_widget, "configure_event",
		     G_CALLBACK(configure_event), this);
}

gboolean
Widget::expose_event(GtkWidget* widget, GdkEventExpose* event, gpointer data)
{
  Widget* t = reinterpret_cast<Widget*>(data);
  
  if (t)
    {
      t->expose(event);
    }
  
  return false;
}

gboolean
Widget::button_press_event(GtkWidget* widget, GdkEventButton* event,
			   gpointer data)
{
  Widget* t = reinterpret_cast<Widget*>(data);

  if (t)
    {
      t->button_press(event);
    }
  
  return false;
}

gboolean
Widget::button_release_event(GtkWidget* widget, GdkEventButton* event,
			     gpointer data)
{
  Widget* t = reinterpret_cast<Widget*>(data);

  if (t)
    {
      t->button_release(event);
    }
  
  return false;
}

gboolean
Widget::motion_notify_event(GtkWidget* widget, GdkEventMotion* event,
			    gpointer data)
{
  Widget* t = reinterpret_cast<Widget*>(data);

  if (t)
    {
      t->motion_notify(event);
    }
  
  return false;
}

gboolean
Widget::configure_event(GtkWidget* widget, GdkEventConfigure* event,
			gpointer data)
{
  Widget* t = reinterpret_cast<Widget*>(data);
  
  if (t)
    {
      t->configure(event);
    }
  
  return false;
}
