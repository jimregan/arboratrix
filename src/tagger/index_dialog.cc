// -*- C++ -*-

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

/*
 * Creado:     Tue Jul 15 12:16:52 CEST 2003
 * Modificado: Tue Jul 15 12:16:52 CEST 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include <index_dialog.h>


/*
 * Constructor por defecto.
 */
Index_dialog::Index_dialog()
{
  // Inicialmente el dialogo no-esta-aceptado.
  accepted = false;

  build(); // crea interfaz y salva this

  set_wm_close(IGNORE);
 
  // ESC cancela el dialogo.
  catch_enter_esc_keys("index");

  index = lookup_widget("index");
}

/*
 * Destructor.
 */
Index_dialog::~Index_dialog()
{
  gtk_widget_destroy(the_widget);
}

void 
Index_dialog::accept()
{
  //accepted = true;
  gtk_dialog_response(GTK_DIALOG(the_widget), GTK_RESPONSE_OK);
}

void 
Index_dialog::cancel()
{
  //accepted = false;
  gtk_dialog_response(GTK_DIALOG(the_widget), GTK_RESPONSE_CANCEL);
}

std::string
Index_dialog::get_index()
{
  return gtk_entry_get_text(GTK_ENTRY(index));
}

void  
Index_dialog::show()
{
  gint result;

  acb_gui::Widget::show();
  result = gtk_dialog_run(GTK_DIALOG(the_widget));

  switch (result)
    {
    case GTK_RESPONSE_OK:
      accepted = true;
      break;
    default:
      accepted = false;
      break;
    }
  //std::cerr << "Salimos del dialog yrs-no\n";
}

GtkWidget*
Index_dialog::create_gui(void)
{
  GtkWidget *index_dialog;
  GtkWidget *dialog_vbox3;
  GtkWidget *hbox5;
  GtkWidget *label9;
  GtkWidget *index;
  GtkWidget *dialog_action_area3;
  GtkWidget *cancelbutton1;
  GtkWidget *okbutton1;

  index_dialog = gtk_dialog_new ();
  gtk_container_set_border_width (GTK_CONTAINER (index_dialog), 1);
  gtk_window_set_title (GTK_WINDOW (index_dialog), _("Enter an index"));
  gtk_window_set_modal (GTK_WINDOW (index_dialog), TRUE);
  gtk_window_set_resizable (GTK_WINDOW (index_dialog), FALSE);

  dialog_vbox3 = GTK_DIALOG (index_dialog)->vbox;
  gtk_widget_show (dialog_vbox3);

  hbox5 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox5);
  gtk_box_pack_start (GTK_BOX (dialog_vbox3), hbox5, FALSE, FALSE, 0);

  label9 = gtk_label_new (_("Index:"));
  gtk_widget_show (label9);
  gtk_box_pack_start (GTK_BOX (hbox5), label9, FALSE, FALSE, 1);
  gtk_label_set_justify (GTK_LABEL (label9), GTK_JUSTIFY_LEFT);

  index = gtk_entry_new ();
  gtk_widget_show (index);
  gtk_box_pack_start (GTK_BOX (hbox5), index, TRUE, TRUE, 1);
  gtk_entry_set_max_length (GTK_ENTRY (index), 3);
  gtk_entry_set_width_chars (GTK_ENTRY (index), 4);

  dialog_action_area3 = GTK_DIALOG (index_dialog)->action_area;
  gtk_widget_show (dialog_action_area3);
  gtk_button_box_set_layout (GTK_BUTTON_BOX (dialog_action_area3), GTK_BUTTONBOX_END);

  cancelbutton1 = gtk_button_new_from_stock ("gtk-cancel");
  gtk_widget_show (cancelbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (index_dialog), cancelbutton1, GTK_RESPONSE_CANCEL);
  GTK_WIDGET_SET_FLAGS (cancelbutton1, GTK_CAN_DEFAULT);

  okbutton1 = gtk_button_new_from_stock ("gtk-ok");
  gtk_widget_show (okbutton1);
  gtk_dialog_add_action_widget (GTK_DIALOG (index_dialog), okbutton1, GTK_RESPONSE_OK);
  GTK_WIDGET_SET_FLAGS (okbutton1, GTK_CAN_DEFAULT);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (index_dialog, index_dialog, "index_dialog");
  GLADE_HOOKUP_OBJECT_NO_REF (index_dialog, dialog_vbox3, "dialog_vbox3");
  GLADE_HOOKUP_OBJECT (index_dialog, hbox5, "hbox5");
  GLADE_HOOKUP_OBJECT (index_dialog, label9, "label9");
  GLADE_HOOKUP_OBJECT (index_dialog, index, "index");
  GLADE_HOOKUP_OBJECT_NO_REF (index_dialog, dialog_action_area3, "dialog_action_area3");
  GLADE_HOOKUP_OBJECT (index_dialog, cancelbutton1, "cancelbutton1");
  GLADE_HOOKUP_OBJECT (index_dialog, okbutton1, "okbutton1");

  return index_dialog;
}
