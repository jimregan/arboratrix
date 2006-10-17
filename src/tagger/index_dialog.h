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

#ifndef _INDEX_DIALOG_H_
#define _INDEX_DIALOG_H_

#include <string>
#include <gui/acb_gui.h>

/*
 * Clase:      Index_dialog
 * Hereda:     Dialog 
 * Version:    1.0
 * Creada:     Tue Jul 15 12:16:52 CEST 2003
 * Modificada: Tue Jul 15 12:16:52 CEST 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Permite solicitar un indice al usuario.
 */

using namespace acb_gui;

/*!
 * \class Index_dialog
 * \brief Permite solicitar un indice al usuario.
 */
class Index_dialog : public Dialog
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto.
  Index_dialog();
  //! Destructor.
  ~Index_dialog();

  //! Antes de mostrar debemos crear un bucle local
  virtual void show();
  //! Metodo invocado al 'aceptar' el dialogo.
  virtual void accept();
  //! Metodo invocado al 'cancelar' el dialogo.
  virtual void cancel();

  //! Devuelve el texto tecleado por el usuario.
  std::string get_index();

  //! Nos dice si el usuario respondio SI.
  bool is_accepted() { return accepted; }

  // Parte protegida.
protected:
  //! Contiene el codigo generado por Glade.
  GtkWidget* create_gui();
  //! El widget con el texto del indice
  GtkWidget* index;
  //! Indica si se respondio SI a la pregunta del dialgo.
  bool accepted;

  // Parte privada.
private:

};

#endif
