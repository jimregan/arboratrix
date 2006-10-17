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

#ifndef _TAG_MODEL_H_
#define _TAG_MODEL_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include "text_model.h"

/*
 * Clase:      Tag_model
 * Hereda:     Text_model 
 * Version:    1.0
 * Creada:     Thu May  8 19:07:28 CEST 2003
 * Modificada: Thu May  8 19:07:28 CEST 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@peurl.dlsi.ua.es
 * Proposito:  Representa el modelo para las etiquetas del canvas.
 */

/*!
 * \class Tag_model
 * \brief Representa el modelo para las etiquetas del canvas.
 */
class Tag_model : public Text_model
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto.
  Tag_model(const std::string& nt, Text_model* pparent = NULL);
  //! Destructor.
  ~Tag_model();

  //! Dice si un elemento puede tener hijos.
  virtual bool can_have_sons() { return true; }

  //! Representacion XML de este nodo.
  void to_xml(xmlNodePtr xml_parent);
  /*! Convierte este modelo y sus hijos en formato qtree, dejando la
   * representacion en el stream \param os.*/
  void to_qtree(std::ostream& os);

  //! Nos dice si este item se trata de un triangulo
  bool is_triangle() { return as_triangle; }
  //! Dice si este item se trata como un triangulo o no.
  void set_triangle(bool t) { as_triangle = t; }

  // Parte protegida.
protected:
  //! Indica si ha de tratarse como un triangulo.
  bool as_triangle;

  // Parte privada.
private:

};

#endif
