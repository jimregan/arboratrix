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

#ifndef _MODEL_H_
#define _MODEL_H_


#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <list>

/*
 * Clase:     Model
 * Fecha:     Tue Sep 12 18:30:54 CEST 2000
 * Autor:     Antonio-M. Corbi Bellot
 * Email:     acorbi@dlsi.ua.es
 * Proposito: La clase base de los modelos.
 */

namespace acb_mvc
{
  class View;

  /*!
   * \class Model
   * \brief Es la clase base de los modelos en este ejemplo.
   */
  class Model
  {
    //friend class View;
    // Parte publica
  public:

    //! Constructor por defecto
    Model();

    //! Destructor
    virtual ~Model();

    /*! 
     * \brief Añade una nueva vista para este modelo.
     *
     * \param v La vista a añadir al modelo.
     */
    void add_view(View* v) { lv.push_back(v); }

    /*!
     * \brief Actualiza las vistas sobre este modelo.
     *
     * Excepto la que se pasa como parametro, por defecto el parámetro es
     * NULL y en ese caso se actualizan todas.
     *
     * Es útil cuando una vista actualiza el modelo y solicita a este
     * que actualice todas sus vistas asociadas, en este caso puede
     * indicar que actualice a todas menos a ella que ya muestra el
     * estado en el que quedara el modelo.
     */
    void update_views(View* except_v = NULL);

    // Parte protegida
  protected:
    //! La lista de vistas asociadas con este modelo.
    std::list<View*> lv;

    // Parte privada
  private:

  };

}
#endif
