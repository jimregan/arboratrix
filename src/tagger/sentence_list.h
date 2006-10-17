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

#ifndef _SENTENCE_LIST_H_
#define _SENTENCE_LIST_H_

#include <list_manager.h>
#include <tagger_model.h>
#include <canvas.h>

class Tagger_view;

/*
 * Clase:      Sentence_list
 * Hereda:     List_manager 
 * Version:    1.0
 * Creada:     Thu Dec 26 17:13:50 CET 2002
 * Modificada: Thu Dec 26 17:13:50 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Gestor de listas de frases.
 */

/*!
 * \class Sentence_list
 * \brief Gestor de listas de frases.
 */
class Sentence_list : public List_manager
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto. \sa List_manager.
  Sentence_list(GtkWidget* ww, const std::string& wln, 
		const std::vector<std::string>& vt, Tagger_view* ptv);

  //! Destructor.
  ~Sentence_list();

  /*! Actualiza el interfaz de la lista dependiendo si hay items
   * seleccionados o no.*/
  virtual void update_interface ();
  //! Metodo invocado al cambiar la frase seleccionada.
  void selection_changed();

  //! Modifica el estado del boton 'finish_button'.
  void set_finish_button (bool status);
  //! Devuelve el estado del boton 'finish'.
  bool get_finish_button ();

  /*! Una frase siempre se puede añadir a la lista, se admiten
   * duplicados.*/
  virtual bool can_be_added(const std::string& s) { return true; }
  
  /*! Muestra una advertencia caso de que no se pueda añadir un item a
   * la lista.*/
  virtual void show_add_warning();

  //! Añade una frase a la lista de frases.
  void add_sentence(Sentence* ns);
  
  // Parte protegida.
protected:
  //! Widget check-button finish.
  GtkWidget* finish_button;
  //! La vista sobre el modelo del etiquetador donde estan las frases.
  Tagger_view* tv;


  /*! Es llamada automaticamente cuando el usuario pulsa el boton
   * 'add'.
   */
  virtual void add_item ();

  /*! Es llamada automaticamente cuando el usuario pulsa el boton
   * 'remove'.
   */
  virtual void remove_item ();

  /*! Inicia los widgets de gtk basicos: add_button, remove_button,
   * entry y the_list.
   */
  virtual void init_subwidgets ();

  /*! Conecta los callbacks a los botones.
   *
   * Si una Lista necesita los eventos de bajo nivel, debera invocar
   * el metodo heredado de Widget.
   *
   * Este metodo conecta el callback para el check-button 'finished'.
   */
  virtual void connect_callbacks ();

  static void finish_callback(GtkWidget* widget, gpointer data);

  // Parte privada.
private:

};

#endif
