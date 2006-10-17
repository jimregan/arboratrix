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

#ifndef _TEXT_MODEL_H_
#define _TEXT_MODEL_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <iostream>
#include <list>
//#include <sstream>
#include <string>
#include <mvc/model.h>
#include <exporter.h>
#include <utils.h>

class Sentence;

/*
 * Clase:      Text_model
 * Hereda:     Model Exporter
 * Version:    1.0
 * Creada:     Fri Jan 10 16:46:36 CET 2003
 * Modificada: Fri Jan 10 16:46:36 CET 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Modelo del texto para ser visualizado en el canvas.
 */

class Text_item;

/*!
 * \class Text_model
 * \brief Modelo del texto para ser visualizado en el canvas.
 */
class Text_model : public acb_mvc::Model, public Exporter
{
  // Declaraciones friend --si las hubiera--.
  friend std::ostream& operator<< (std::ostream& os, const Text_model& tm);

  // Parte publica.
public:

  /*! Constructor por defecto.
   * \param nt El texto de este nodo.
   * \param pparent El padre de este nodo, por defecto _nadie_. */
  Text_model(const std::string& nt, Text_model* pparent = NULL);

  //! Destructor.
  ~Text_model();

  //! Ordena redibujar sus hijos.
  void update_children();

  //! Devuelve el texto de este modelo.
  const std::string& get_text() const { return text; }
  //! Cambia el texto de este modelo.
  void set_text(const std::string& nt);

  //! Devuelve la etiqueta de este modelo.
  const std::string& get_tag() { return tag; }
  //! Cambia la etiqueta de este modelo.
  void set_tag(const std::string& nt) { tag = nt; }

  //! Devuelve el indice de este modelo.
  const std::string& get_index() const { return index; }
  //! Cambia el indice de este modelo.
  void set_index(const std::string& nidx);
  //! Indica si el modelo tiene indice asociado.
  bool has_index() const { return get_index() != ""; }

  //! Devuelve la representacion del conjunto vacio.
  static const std::string& get_empty_set();
  //{ return empty_set; }

  //! Dice si este modelo es una categoria nula.
  bool is_empty_category() const { return get_text() == get_empty_set(); }
  //! Dice si este nodo se creo como conecuencia de una traza.
  bool is_trace() const { return trace_destination != NULL; }

  //! Dice si un elemento puede tener hijos.
  virtual bool can_have_sons() { return false; }
  //! Adopta un text_model como hijo.
  void add_child(Text_model* child);
  //! Elimina el text_model child como hijo.
  void remove_child(Text_model* child);
  //! Lo elimina de la lista de hijos de su padre y lo deja sin padre.
  void remove_from_parent();
  //! Lo elimina de la lista de hijos de su padre y le cede sus hijos.
  void remove_from_parent_and_adopt();

  //! Lee el padre de este nodo.
  Text_model* get_parent() { return parent; }
  //! Cambia el padre de este nodo.
  void set_parent(Text_model* p);

  //! Indica si este es un nodo hoja.
  bool is_leaf() { return children.size() == 0; }
  //! Nos dice si un nodo es hijo directo o indirecto de este.
  bool is_son(Text_model* child);
  //! Nos dice si un nodo es hijo directo de este.
  bool is_direct_son(Text_model* child);
  //! Nos dice si este nodo es hijo de otro.
  bool is_son_of(Text_model* parent);
  //! Nos dice si se trata de un nodo adjunto.
  bool is_adjoint();
  //! Devuelve el hijo que lo convierte en adjunto.
  Text_model* get_adjoint_son() { return adjoint_son; }

  /*! Devuelve la altura de este item en el arbol.
   *  Las hojas tienen una altura = N y la raiz = 0.*/
  int get_level();
  //! Devuelve la columna que ocupa en su nivel en el arbol.
  int get_column() { return column; }
  //! Cambia la columna que ocupa en su nivel en el arbol.
  void set_column(int c) { column = c; }

  //! Nos dice si un nodo es la raiz o no.
  bool is_root() { return parent == NULL; }


  //! Numero de hijos directos que tiene este text_item.
  int get_num_children() { return children.size(); }
  //! Indica si tiene numero impar de hijos.
  bool has_odd_children() { return (get_num_children() % 2) != 0; }
  /*! Indica el numero de celdas [1..N] q ocupa el recuadro con sus
   * hijos directos e indirectos. \sa Text_item::get_pixel_span */
  int get_cell_span();
  //! Indica la celda [1..N] q ocupa este nodo respecto a sus hijos.
  int get_cell_in_span() { return (get_cell_span() / 2) + 1; }

  /*! Devuelve la lista de sus hijos.  No se debe liberar la
   * memoria.*/
  std::list<Text_model*>* get_children() { return &children; }
  /*! Devuelve una lista con los text_items (las vistas) de sus hijos.
   * El receptor es responsable de liberarla para no producir perdidas
   * de memoria.*/
  std::list<Text_item*>* get_children_text_items();

  //! Devuelve la vista (la primera) sobre este modelo.
  Text_item* get_first_view();

  //! Devuelve su primer hijo
  Text_model* get_first_child() { return children.front(); }

  //--- EXPORTER ----
  //! Convierte este modelo y sus hijos en xml.
  void to_xml(xmlNodePtr xml_parent);
  //! Un text model no puede convertirse por si solo a postscript.
  void to_ps(std::ostream& os) {}
  //! Un text model no puede convertirse por si solo a PDF.
  void to_pdf(std::ostream& os) {}
  /*! Convierte este modelo en formato qtree, dejando la
   * representacion en el stream \param os.*/
  void to_qtree(std::ostream& os);

  /*! Indica si se puede crear una traza, el nodo receptor del mensaje
   * es el nodo 'destino', mientras que \param orig es el nodo que
   * arrastra el usuario y comienza la traza.*/
  bool can_create_trace(Text_model* orig);
  /*! Crea una traza si es posible, el nodo receptor del mensaje es el
   * nodo 'destino', mientras que \param orig es el nodo que arrastra
   * el usuario y comienza la traza.*/
  bool create_trace(Text_model* orig);
  //! Da valor a la referencia usada en las trazas.
  void set_reference(const std::string& r) { reference = r; }
  //! Lee el valor de su referencia.
  const std::string& get_reference() { return reference; }
  //! Da valor a la 'etiqueta' (label) usada en las trazas.
  void set_label(const std::string& l) { label = l; }
  //! Lee el valor de su 'etiqueta' (label).
  const std::string& get_label() { return label; }
  //! Da valor al nodo destino de la traza.
  void set_trace_destination(Text_model* td) { trace_destination = td; }
  //! Lee el valor del nodo destino de la traza.
  Text_model* get_trace_destination() { return trace_destination; }

  //! Guarda la frase que crea este nodo.
  void set_sentence(Sentence* s) { the_sentence = s; }
  //! Recupera la frase que crea este nodo.
  Sentence* get_sentence() { return the_sentence; }

  // Parte protegida.
protected:
  //! Representacion de las categorias nulas.
  static std::string empty_set;
  //! El texto de este modelo.
  std::string text;
  //! La etiqueta asociada.
  std::string tag;
  //! El indice asociado al item.
  std::string index;
  //! La referencia a una etiqueta usada en las trazas.
  std::string reference;
  //! La etiqueta usada en las trazas.
  std::string label;
  //! El nodo destino de una traza con origen en este.
  Text_model* trace_destination;
  //! La frase que crea este nodo.
  Sentence* the_sentence;

  //! La lista de hijos suyos.
  std::list<Text_model*> children;
  //! El padre de este elemento
  Text_model* parent;
  /*! El hijo que hace que este nodo sea adjunto.  A esta variable se
   * le asigna valor en \sa is_adjoint()*/
  Text_model* adjoint_son;
  //! Columna que ocupa en la fila de su nivel en el arbol.
  int column;

  // Parte privada.
private:
};

#endif
