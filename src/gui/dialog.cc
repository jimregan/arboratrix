// -*- C++ -*-

/*
 * Creado:     Wed Jan 30 14:20:04 CET 2002
 * Modificado: Wed Jan 30 14:20:04 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "dialog.h"

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Dialog::Dialog()
{
  //set_title (title);
}

/*
 * Destructor.
 */
Dialog::~Dialog()
{
  //std::cerr << "Dialogo destruido.\n";
}

void
Dialog::dialog_accepted(GtkWidget* widget, gpointer data)
{
    Dialog* d = reinterpret_cast<Dialog*>(data);

    d->accept();
}

void
Dialog::dialog_cancelled(GtkWidget* widget, gpointer data)
{
    Dialog* d = reinterpret_cast<Dialog*>(data);

    d->cancel();
}

bool 
Dialog::key_pressed(GtkWidget* w, GdkEventKey* e, gpointer p)
{
  Dialog* d = reinterpret_cast<Dialog*>(p);

  // Si es return...aceptamos
  if (e -> string[0] == static_cast<char>(GDK_Return))
    {
      d->accept();
      return true;
    }

  // Si es ESC cancelamos
  if (e -> string[0] == static_cast<char>(GDK_Escape))
    {
      d->cancel();
      return true;
    }

  return false;
}

bool
Dialog::esc_key_pressed(GtkWidget* w, GdkEventKey* e, gpointer p)
{
  Dialog* d = reinterpret_cast<Dialog*>(p);

  //std::cerr << "ESC\n";
  // Si es ESC cancelamos
  if (e -> string[0] == static_cast<char>(GDK_Escape))
    {
      //std::cerr << "CANCEL.\n";
      d->cancel();
      return true;
    }

  return false;
}

void
Dialog::catch_enter_esc_keys(const std::string& wn)
{
  GtkWidget* w = lookup_widget(wn.c_str());

  if (w)
    g_signal_connect (GTK_OBJECT (w), "key_press_event",
		      G_CALLBACK (key_pressed), this);
}

void
Dialog::catch_esc_key()
{
  g_signal_connect (GTK_OBJECT (the_widget), "key_press_event",
		    G_CALLBACK (esc_key_pressed), this);
}
