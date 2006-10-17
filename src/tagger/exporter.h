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

#ifndef _EXPORTER_H_
#define _EXPORTER_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <iostream>

/*
 * Clase:      Exporter
 * Hereda:     De nadie, es clase base.
 * Version:    1.0
 * Creada:     Wed Apr 23 10:26:54 CEST 2003
 * Modificada: Wed Apr 23 10:26:54 CEST 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@peurl.dlsi.ua.es
 * Proposito:  Clase base de los objetos que pueden exportarse a xml,ps,pdf o qtree.
 *             Se trata de una clase interfaz.
 */

/*!
 * \class Exporter
 * \brief Clase base de los objetos que pueden exportarse a xml,ps,pdf o qtree.
 */
class Exporter
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  virtual void to_xml(xmlNodePtr xml_parent) ABSTRACT;
  virtual void to_ps(std::ostream& os)       ABSTRACT;
  virtual void to_pdf(std::ostream& os)      ABSTRACT;
  virtual void to_qtree(std::ostream& os)    ABSTRACT;

  // Parte protegida.
protected:

  // Parte privada.
private:

};

#endif
