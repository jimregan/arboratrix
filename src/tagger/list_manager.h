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

#ifndef _LIST_MANAGER_H_
#define _LIST_MANAGER_H_

#include <gtk/gtk.h>
#include <string>
#include <vector>
#include <gui/acb_gui.h>

#define _(x) x

/*
 * Clase:      List_manager
 * Hereda:     Widget 
 * Version:    1.0
 * Creada:     Thu Dec 26 12:53:35 CET 2002
 * Modificada: Thu Dec 26 12:53:35 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Gestor de listas. Permite añadir y eliminar elementos de texto.
 */

/*!
 * \class List_manager
 * \brief Gestor de listas. 
 *
 * Se comporta como un widget compuesto de varios subwidgets (dos
 * botones add/remove) y un campo de entrada de texto.Permite añadir y
 * eliminar elementos de texto a la lista.
 *
 * Clases derivadas deben definir los metodos: add_item, remove_item e
 * init_subwidgets.
 */
class List_manager : public acb_gui::Widget
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  /*! Constructor por defecto.
   * \param wwindow La ventana donde se muestra esta lista.
   * \param wln El nombre del widget de gtk que contiene la lista,
   * entry y botones.
   * \param vt El vector con los titulos de las columnas de la lista.
   */
  List_manager(GtkWidget* wwindow, const std::string& wln, 
		const std::vector<std::string>& vt);
  //! Destructor.
  ~List_manager();

  //! Elimina los elementos de la lista.
  void clear();

  //! Indica si hay elementos de la lista seleccionados.
  bool are_items_selected();

  /*! Actualiza el interfaz de la lista dependiendo si hay items
   * seleccionados o no.*/
  virtual void update_interface();

  //! Vacia el campo de entrada de texto
  void empty_text_entry() { gtk_entry_set_text(GTK_ENTRY(entry),""); }

  //! Comprueba si el item se puede añadir a la lista
  virtual bool can_be_added(const std::string& s) ABSTRACT;

  /*! Muestra una advertencia caso de que no se pueda añadir un item a
   * la lista. La clase derivada debe ocuparse de redefinirla.*/
  virtual void show_add_warning() ABSTRACT;

  /*! Añade un elemento de texto a la lista. Este elemento representa
    en la aplicacion al objeto apuntado por \param p. \returns
    verdadero si el item se ha podido añadir a la lista.*/
  bool add_element(const std::string& str, gpointer p = NULL);

  // Parte protegida.
protected:
  //! Titulos de las columnas de la lista
  std::vector<std::string> titulo;
  //! El modelo del tree-view
  GtkTreeModel* model;
  //! El puntero al ultimo item añadido
  GtkTreeIter last_item;
  //! Puntero al ultimo item seleccionado
  gpointer plis;
  //! Puntero al ultimo item eliminado
  gpointer plir;
  //! Puntero al ultimo item valido tras un borrado
  gpointer plvi;
  //! Puntero usado para guardar informacion asociada a cada fila
  gpointer row_data;
  //! La ventana donde reside la lista y sus botones.
  GtkWidget* parent_window;
  //! Widget boton añadir.
  GtkWidget* add_button;
  //! Widget boton eliminar.
  GtkWidget* remove_button;
  //! Widget entrada de texto.
  GtkWidget* entry;
  //! Widget lista de opciones.
  GtkWidget* the_list;
  //! Texto del ultimo item seleccionado.
  std::string lst;

  //! Inicia subwidgets y conecta callbacks.
  void init_widget() { init_subwidgets(); init_list(); connect_callbacks(); }

  //! Inicia el treeview de la lista.
  void init_list();

  //! Recoge el código generado por glade en 'create_...'
  virtual GtkWidget* create_gui();

  /*! Es llamada automaticamente cuando el usuario pulsa el boton
   * 'add'. Debe ser redefinida en la subclase.*/
  virtual void add_item() ABSTRACT;

  /*! Es llamada automaticamente cuando el usuario pulsa el boton
   * 'remove'. Debe ser redefinida en la subclase.*/
  virtual void remove_item() ABSTRACT;

  /*! Es llamada automaticamente cuando el usuario de/selecciona algo
   * de la lista. Debe ser redefinida en la subclase.*/
  virtual void selection_changed() ABSTRACT;

  /*! Inicia los widgets de gtk basicos: add_button, remove_button,
   * entry y the_list. Debe ser redefinida en la subclase.*/
  virtual void init_subwidgets() ABSTRACT;

  /*! Conecta los callbacks a los botones.
   *
   * Si una Lista necesita los eventos de bajo nivel, debera invocar
   * el metodo heredado de Widget.
   */
  virtual void connect_callbacks();

  //! Crea el modelo en el que se basa GtkTreeView.
  GtkTreeModel* create_model(void);

  void save_row_data(gpointer p);

  /*! Metodo encargado de capturar la tecla 'enter' en el widget
   * 'entry'.*/
  void catch_enter_key();

  /*________CALLBACKS_________*/

  //! Callback invocado al pulsar el boton 'add'.
  static void add_callback(GtkWidget* widget, gpointer data);

  //! Callback invocado al pulsar el boton 'remove'.
  static void remove_callback(GtkWidget* widget, gpointer data);

  //! Callback invocado al cambiar la seleccion de la lista.
  static void selection_changed_callback(GtkWidget* widget, gpointer data);

  //! Callback invocado al pulsar 'enter' en la entrada de texto.
  static bool enter_key_pressed(GtkWidget* widget, GdkEventKey* e, gpointer data);

  /*________CALLBACKS_________*/


  // Parte privada.
private:

};

#endif
