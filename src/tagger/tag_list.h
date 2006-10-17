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

#ifndef _TAG_LIST_H_
#define _TAG_LIST_H_

#include <list_manager.h>
#include <tagger_model.h>

class Tag_info;
class Tagger_view;

/*
 * Clase:      Tag_list
 * Hereda:     List_manager 
 * Version:    1.0
 * Creada:     Thu Dec 26 17:13:50 CET 2002
 * Modificada: Thu Dec 26 17:13:50 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Gestor de listas de tags.
 */

/*!
 * \class Tag_list
 * \brief Gestor de listas de tags.
 */
class Tag_list : public List_manager
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto. \sa List_manager.
  Tag_list(GtkWidget* ww, const std::string& wln, 
	   const std::vector<std::string>& vt, Tagger_view* ptv);

  //! Destructor.
  ~Tag_list();

  /*! Comprueba si el tag 's' se puede añadir a la lista. Es necesaria
   * ya que no admitimos tags duplicados.*/
  virtual bool can_be_added(const std::string& s);

  /*! Muestra una advertencia caso de que no se pueda añadir un item a
   * la lista.*/
  virtual void show_add_warning();

  //! Añade una etiqueta a la lista de etiquetas.
  bool add_tag(Tag_info* nt);

  void set_reading_from_file(bool rff) { reading_from_file = rff; }

  // Parte protegida.
protected:
  //! La vista sobre el modelo del etiquetador donde estan las frases.
  Tagger_view* tv;
  //! Nos indica si estamos leyendo los tags desde un fichero.
  bool reading_from_file;

  /*! Es llamada automaticamente cuando el usuario pulsa el boton
   * 'add'.
   */
  virtual void add_item();

  /*! Es llamada automaticamente cuando el usuario pulsa el boton
   * 'remove'.
   */
  virtual void remove_item();

  /*! Es llamada automaticamente cuando el usuario de/selecciona algo
   * de la lista. Debe ser redefinida en la subclase.*/
  virtual void selection_changed();

  /*! Inicia los widgets de gtk basicos: add_button, remove_button,
   * entry y the_list.
   */
  virtual void init_subwidgets();

  //! Manejador del evento motion_notify de alto nivel.
  virtual void motion_notify(GdkEventMotion* event) { std::cout << "ARRASTRAAAAAN\n"; }

  //! Conecta los callbacks propios de este widget.
  virtual void connect_callbacks();

  /*---------------------- DND ----------------------*/
  static void dnd_begin_cb(GtkWidget* widget, GdkDragContext* dc, gpointer data);
  static void dnd_end_cb(GtkWidget* widget, GdkDragContext* dc, gpointer data);
  static gboolean dnd_dragmotion_cb(GtkWidget* widget, GdkDragContext* dc,
				    gint x, gint y, guint t, gpointer data);
  static void dnd_data_received_cb(GtkWidget* widget, GdkDragContext* dc,
				   gint x, gint y, 
				   GtkSelectionData* selection_data,
				   guint info, guint t, gpointer data);
  static void dnd_data_request_cb(GtkWidget* widget, GdkDragContext* dc,
				  GtkSelectionData* selection_data, 
				  guint info, guint t, gpointer data);
  /*---------------------- DND ----------------------*/

  // Parte privada.
private:

};

#endif
