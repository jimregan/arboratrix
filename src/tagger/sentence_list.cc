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
 * Creado:     Thu Dec 26 17:13:50 CET 2002
 * Modificado: Thu Dec 26 17:13:50 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include <tagger_view.h>
#include <sentence_list.h>
#include <sentence.h>

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Sentence_list::Sentence_list(GtkWidget* ww, const std::string& wln,
			     const std::vector<std::string>& vt,
			     Tagger_view* ptv):
  List_manager(ww, wln, vt), tv(ptv)
{
  build();
  init_widget();
  catch_enter_key();
  update_interface();
}

/*
 * Destructor.
 */
Sentence_list::~Sentence_list()
{
  //std::cout << "SL: destructor.\n";
}

/*
 * add_item
 */
void
Sentence_list::add_item()
{
  std::string str = gtk_entry_get_text(GTK_ENTRY(entry));
  Sentence*   ns;

  //A la vista
  ns = tv->create_sentence(str);

  //No es necesario añadir la frase al widget lista_de_frases pq se
  //hace automatico al pulsar el boton añadir.

  // Y guardamos un ptro. al modelo en el GUI
  //save_row_data(ns);
  row_data = ns;
}

void
Sentence_list::add_sentence(Sentence* ns)
{
  //save_row_data(ns);
  //row_data = ns;
  add_element(ns->get_text(), ns);
}

/*
 * remove_item
 */
void
Sentence_list::remove_item()
{
  Sentence* s;

  // plir contiene el puntero al ultimo elemento borrado de la lista.
  s = static_cast<Sentence*>(plir);
  if (s)
    {
      //std::cout << "SL: borramos la frase [" << *s << "]\n";
      tv->get_model()->remove_sentence(s);
    }
  else
    {
      //std::cout << "Error, frase NULA\n";
    }

  // plvi contiene el puntero al nuevo elemento valido de la lista.
  s = static_cast<Sentence*>(plvi);
  if (s)
    {
      //std::cout << "La nueva frase seleccionada [" << *s << "]\n";
      tv->get_canvas()->set_sentence(s);
      //update_interface(); -> Ya lo hace la clase base
    }
}

/*
 * init_subwidgets
 */
void
Sentence_list::init_subwidgets()
{
  
  add_button    = Widget::lookup_widget(parent_window, "sentence_add_button");
  remove_button = Widget::lookup_widget(parent_window, "sentence_delete_button");
  entry         = Widget::lookup_widget(parent_window, "sentence");
  the_list      = Widget::lookup_widget(parent_window, "sentence_list");
  finish_button = Widget::lookup_widget(parent_window, "finish_button");
}

void
Sentence_list::connect_callbacks()
{
  // Para los botones add y remove.
  List_manager::connect_callbacks();

  // Para el boton finished
  g_return_if_fail(finish_button != NULL);
  g_signal_connect (G_OBJECT (finish_button), "toggled",
		    G_CALLBACK (finish_callback), 
		    this);
}

void
Sentence_list::finish_callback(GtkWidget* widget, gpointer data)
{
  Sentence_list* sl = reinterpret_cast<Sentence_list*>(data);
  Sentence*      s;

  // last_item contiene el puntero al ultimo elemento borrado de la lista.
  s = static_cast<Sentence*>(sl->plis);

  if (s)
    {
      s->set_finished(sl->get_finish_button());
      sl->tv->get_canvas()->deselect_all_items();
    }

  //std::cout << "Boton finished cambiado.\n";
}

void
Sentence_list::update_interface()
{
  Sentence* s;

  // last_item contiene el puntero al ultimo elemento borrado de la lista.
  s = static_cast<Sentence*>(plis);
  // De la clase base, para el boton-añadir
  List_manager::update_interface();

  // Propio de esta clase.
  if (are_items_selected())
    gtk_widget_set_sensitive(finish_button, true);
  else
    {
      set_finish_button(false);
      gtk_widget_set_sensitive(finish_button, false);

      tv->get_canvas()->empty();
    }

  if (s)
    set_finish_button(s->is_finished());
}

void
Sentence_list::set_finish_button(bool status)
{
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON(finish_button), status);
}

bool
Sentence_list::get_finish_button()
{
  return gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(finish_button));
}

/*
 * selection_changed
 */
void
Sentence_list::selection_changed()
{
  Sentence* s;

  // last_item contiene el puntero al ultimo elemento borrado de la lista.
  s = static_cast<Sentence*>(plis);

  if (s)  //Por si no habia frase seleccionada.
    {
      tv->get_canvas()->set_sentence(s);
      /*if (s->is_finished())
	std::cout << "frase TERMINADA: " << *s << std::endl;
      else
      std::cout << "frase _NO_ TERMINADA: " << *s << std::endl;*/
    }
}

void
Sentence_list::show_add_warning()
{
  acb_gui::Info_dialog d(_("Warning:"), _("Sentences can't be empty."));
  d.show();
}
