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

#ifndef _TAG_INFO_H_
#define _TAG_INFO_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <string>

/*
 * Clase:      Tag_info
 * Hereda:     De nadie, es clase base.
 * Version:    1.0
 * Creada:     Tue May 13 17:47:39 CEST 2003
 * Modificada: Tue May 13 17:47:39 CEST 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Guarda informacion de cada tag añadido al sistema
 */

/*!
 * \class Tag_info
 * \brief Guarda informacion de cada tag añadido al sistema
 */
class Tag_info
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto.
  Tag_info(const std::string& s);
  //! Destructor.
  ~Tag_info();

  const std::string& get_tag() { return the_tag; }

  friend bool operator==(Tag_info* ti, const std::string& s) { return ti->the_tag == s; }

  // Parte protegida.
protected:

  //! La etiqueta q representa
  std::string the_tag;

  // Parte privada.
private:

};

#endif
