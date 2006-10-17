// -*- C++ -*-

/*
 * Creado:     Wed May 15 17:02:45 CEST 2002
 * Modificado: Wed May 15 17:02:45 CEST 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "yes_no_dialog.h"

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Yes_no_dialog::Yes_no_dialog(const std::string& t, const std::string& q)
{
  gsize bytes_read;
  gsize bytes_written;

  // Inicialmente el dialogo no-esta-aceptado.
  accepted = false;

  //Necesito poner el mensaje en 'utf8_m' antes de crear el interfaz.
  utf8_m = g_locale_to_utf8 (q.c_str(), -1,
			     &bytes_read, &bytes_written,
			     NULL);

  build(); // crea interfaz y salva this

  utf8_t = g_locale_to_utf8 (t.c_str(), -1,
			     &bytes_read, &bytes_written,
			     NULL);
  set_title(t);
  //gtk_window_set_title(GTK_WINDOW(the_widget), utf8_t);
  set_wm_close(IGNORE);
 
  // ESC cancela el dialogo.
  catch_esc_key();
}

void  
Yes_no_dialog::show()
{
  gint result;

  acb_gui::Widget::show();
  result = gtk_dialog_run(GTK_DIALOG(the_widget));
  gtk_widget_destroy(the_widget);

  switch (result)
    {
    case GTK_RESPONSE_YES:
      accepted = true;
      break;
    default:
      accepted = false;
      break;
    }
  //std::cerr << "Salimos del dialog yrs-no\n";
}

/*
 * Destructor.
 */
Yes_no_dialog::~Yes_no_dialog()
{
  g_free(utf8_m);
  g_free(utf8_t);
}

void 
Yes_no_dialog::accept()
{
  //accepted = true;
  gtk_dialog_response(GTK_DIALOG(the_widget), GTK_RESPONSE_YES);
}

void 
Yes_no_dialog::cancel()
{
  //accepted = false;
  gtk_dialog_response(GTK_DIALOG(the_widget), GTK_RESPONSE_NO);
}

GtkWidget* 
Yes_no_dialog::create_gui()
{
  GtkWidget* dialog;

  dialog = gtk_message_dialog_new (NULL,
				   GTK_DIALOG_MODAL,
				   GTK_MESSAGE_QUESTION,
				   GTK_BUTTONS_YES_NO,
				   utf8_m);
  return dialog;
}
