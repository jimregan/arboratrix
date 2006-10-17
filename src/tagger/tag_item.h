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

#ifndef _TAG_ITEM_H_
#define _TAG_ITEM_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include "text_item.h"
#include "tag_model.h"


/*
 * Clase:      Tag_item
 * Hereda:     Text_item 
 * Version:    1.0
 * Creada:     Thu May  8 19:03:22 CEST 2003
 * Modificada: Thu May  8 19:03:22 CEST 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@peurl.dlsi.ua.es
 * Proposito:  Representa las etiquetas en el canvas.
 */

/*!
 * \class Tag_item
 * \brief Representa las etiquetas en el canvas.
 */
class Tag_item : public Text_item
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto.
  Tag_item(Tag_model& m, const Canvas* c, bool auto_add = false);

  //! Destructor.
  ~Tag_item();

  //! Devuelve el modelo con el tipo apropiado.
  Tag_model* get_model() { return dynamic_cast<Tag_model*>(model); }
  //! Devuelve el modelo con el tipo apropiado.
  Tag_model* get_model() const { return dynamic_cast<Tag_model*>(model); }

  /*! Indica si el item se puede borrar. Por defecto un Tag_item se
   * puede borrar.*/
  bool is_erasable() { return true; }

  //! Nos dice si este item (su modelo) puede tener hijos.
  virtual bool can_have_sons() { return true; }

  //! Nos dice si este item se trata de un triangulo
  bool is_triangle() { return as_triangle; }
  //! Dice si este item se trata como un triangulo o no.
  void set_triangle(bool t) { as_triangle = t; get_model()->set_triangle(t); }

  /*! Muestra caracteristicas del item en un tooltip en las
   * coordenadas 'x', 'y'.*/
  void show_properties(int x, int y);

  /*! Dibuja las lineas de conexion con sus hijos. Se redefine de la
   * clase base para tratar el caso especial de los triangulos.*/
  void draw_lines_to_children();

  /*! Indica al usuario caracteristicas relevantes cuando se intenta
    'dejar' otro item sobre el.*/
  void show_feedback_for(Canvas_item* son, bool shift, bool control);

  // Parte protegida.
protected:
  //! Asigna sus colores a los GCs.
  virtual void set_colors();
  //! Indica si ha de tratarse como un triangulo.
  bool as_triangle;

  // Parte privada.
private:

};

#endif
