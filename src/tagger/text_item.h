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

#ifndef _TEXT_ITEM_H_
#define _TEXT_ITEM_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <iostream>
#include <string>
#include <gdk/gdk.h>
#include "canvas_item.h"
#include "text_model.h"

/*
 * Clase:      Text_item
 * Hereda:     Canvas_item 
 * Version:    1.0
 * Creada:     Sun Jan 12 13:22:32 CET 2003
 * Modificada: Sun Jan 12 13:22:32 CET 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Vista sobre un modelo de texto.
 */

/*!
 * \class Text_item
 * \brief Vista sobre un modelo de texto.
 */
class Text_item : public Canvas_item
{
  // Declaraciones friend --si las hubiera--.
  friend std::ostream& operator<< (std::ostream& os, const Text_item& ti);

  // Parte publica.
public:

  //! Constructor por defecto.
  Text_item(Text_model& m, const Canvas* c, bool auto_add = false);
  //! Destructor.
  ~Text_item();

  //! Crea las estructuras de datos para PANGO.
  void create_font_data(const std::string& font_name);

  //! Crea las estructuras de datos para PANGO.
  void create_index_font_data(const std::string& font_name);

  //! Actualizacion de un Text_item pedida desde el modelo.
  virtual void update();
  //! Visualización de un Text_item.
  virtual void draw();

  //! Devuelve el objeto PanGo empleado para escribir el texto.
  PangoLayout* get_text_pango_layout() const { return pl; }
  //! Devuelve el objeto PanGo empleado para escribir el indice.
  PangoLayout* get_index_pango_layout() const { return pli; }

  /*! Devuelve la anchura en pixels de los hijos de este item (en
   * realidad, de su modelo). A cada item le suma despues el ajuste o
   * 'padding' definido en esta misma clase.*/
  int get_children_width();

  //! Devuelve la separacion horizontal entre nodos.
  static int get_pad_w() { return pad_w; }
  //! Devuelve la separacion vertical entre nodos.
  static int get_pad_h() { return pad_h; }
  //! Devuelve la representacion del conjunto vacio.
  static const std::string& get_empty_set() { return Text_model::get_empty_set(); }

  //! Borra este item solo del canvas.
  virtual void clear_from_canvas(bool generate_event = false);
  //! Dibuja las lineas de conexion con sus hijos.
  virtual void draw_lines_to_children();
  //! Dibuja la linea desde su padre hasta el.
  void draw_line_from_parent();

  //! Aplica un offset-x a este nodo y sus hijos directos.
  void offset_by_x_with_children(int ox);
  //! Llama a text_model::get_cell_span.
  int get_cell_span() { return dynamic_cast<Text_model*>(model)->get_cell_span(); }
  /*! Indica el numero de pixels horizontales que ocupa el recuadro
   * con sus hijos directos e indirectos. */
  int get_pixel_span();
  /*! Indica el numero de pixels horizontales que ocupa el recuadro
   * _solo_ de sus hijos directos. */
  int get_direct_pixel_span();
  //! Calcula los pixeles desde el ppio de su recuadro para ser dibujado.
  int get_x_delta_in_span();
  //! Calcula y fija la posicion 'x,y' del nodo en el arbol.
  //void calc_xy_pos(int offset_x) { calc_x_pos(offset_x); calc_y_pos(); }
  void calc_xy_pos() { calc_x_pos(); calc_y_pos(); }
  //! Calcula y fija el valor de 'x' del nodo en el arbol.
  //  void calc_x_pos(int offset_x);
  void calc_x_pos();
  //! Calcula y fija el valor de 'y' del nodo en el arbol.
  void calc_y_pos();

  //! Calcula el punto de salida de la linea hacia el hijo c.
  Point out_point(const Canvas_item* c);
  /*! Calcula el punto de entrada de la linea que llega desde su
   * padre.*/
  Point in_point();

  //! Devuelve el Text_item correspondiente a su padre en el arbol.
  Text_item* get_parent();

  /*! Devuelve una lista con los text_items (las vistas) de sus hijos.
   * El receptor es responsable de liberarla para no producir perdidas
   * de memoria.*/
  std::list<Text_item*>* get_children_text_items();

  //! Devuelve su primer hijo.
  Text_item* get_first_child();
  //! Devuelve su ultimo hijo.
  Text_item* get_last_child();

  //! Devuelve la columna que ocupa en su nivel en el arbol el modelo.
  int get_column() { return dynamic_cast<Text_model*>(model)->get_column(); }

  /*! Muestra caracteristicas del item en un tooltip en las
   * coordenadas 'x', 'y'.*/
  void show_properties(int x, int y);
  /*! Indica al usuario caracteristicas relevantes cuando se intenta
    'dejar' otro item sobre el.*/
  void show_feedback_for(Canvas_item* son, bool shift, bool control);

  //! Devuelve el modelo con el tipo apropiado.
  Text_model* get_model() { return dynamic_cast<Text_model*>(model); }
  //! Devuelve el modelo con el tipo apropiado.
  Text_model* get_model() const { return dynamic_cast<Text_model*>(model); }

  //! Asigna sus colores a los GCs.
  virtual void set_colors();

  /*! Indica si el item se puede borrar. Un Text_item _no_ se puede
   * borrar, salvo q sea categoria nula.*/
  bool is_erasable() { return empty_category ? true : false; }

  //! Nos dice si este item (su modelo) puede tener hijos.
  virtual bool can_have_sons() { return false; }

  /*! Ayuda en la creacion de la lista de items seleccionados,
   * incluyendo a sus hijos (si los tiene) de manera recursiva aunque
   * no esten seleccionados.*/
  void compute_selected_items(std::list<Canvas_item*>& cisl);

  //! Facilita la modificacion del indice del modelo.
  void set_index(const std::string& idx) { get_model()->set_index(idx); }
  //! Indica si el modelo tiene indice asociado.
  bool has_index() const { return get_model()->has_index(); }
  //! Indica si el item es una categoria nula.
  bool is_empty_category() const { return empty_category; }

  /*! Crea una traza, el nodo receptor del mensaje es el nodo
   * 'destino', mientras que \param orig es el nodo que arrastra el
   * usuario y comienza la traza. Llama a
   * Text_model::create_trace().*/
  void create_trace(Text_item* orig);

  // Parte protegida.
protected:
  //! Crea el objeto pango_layout
  void create_rendered_text();
  //! Crea el objeto pango_layout para el indice
  void create_rendered_index();
  //! Visualizacion de los hijos del modelo de este text_item.
  void update_children();

  //! Separacion horizontal entre items de la misma altura.
  static const int pad_w = 20;
  //! Separacion vertical entre items del arbol.
  static const int pad_h = 45;
  //! Indica si se trata de una categoria_nula.
  bool empty_category;
  /*! Indica si se trata de un nodo creado como consecuencia de una
   * traza.*/
  bool is_trace;

  // Parte privada.
private:
  PangoLayout*             pl;
  PangoLayout*             pli;
  PangoFontDescription*    pfd;
  PangoFontDescription*    pfdi;
};

#endif
