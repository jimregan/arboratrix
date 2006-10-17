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

#ifndef _VIEW_H_
#define _VIEW_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

/*
 * Clase:     View
 * Fecha:     Tue Sep 12 16:42:18 CEST 2000
 * Autor:     Antonio-M. Corbi Bellot
 * Email:     acorbi@dlsi.ua.es
 * Proposito: La clase base de las vistas.
 */

namespace acb_mvc
{
  class Model;

  /*!
   * \class View
   * \brief Es la clase base de las vistas en este ejemplo.
   */
  class View
  {
    // Parte publica
  public:

    /*! 
     * \brief Constructor por defecto.
     *
     * \param m El modelo con el que se asocia esta vista.
     */
    View(Model& m);

    //! Destructor
    virtual ~View();

    /*! 
     * \brief Actualización de la vista ordenada por el modelo.
     *
     * Es virtual pura ya que son las vistas derivadas de esta clase las
     * que deben saber cómo actualizarse.
     */
    virtual void update() ABSTRACT;

    /*!
     * \brief Actualizacion del modelo ordenada por la vista.
     *
     * Tiene sentido cuando el usuario actuando sobre la vista modifica
     * datos que han de reflejarse posteriormente en el modelo.
     *
     * Por defecto le dice al modelo que actualice sus otras posibles
     * vistas sobre el. Tiene sentido que se llame en ultimo lugar
     * dentro del propio metodo redefinido en una clase derivada.
     */
    virtual void update_model();

    //! Devuelve el modelo de esta vista.
    virtual Model* get_model() { return model; }
    virtual Model* get_model() const { return model; }

    // Parte protegida
  protected:
    //! El modelo al que se asocia esta vista.
    Model* model;

    // Parte privada
  private:

  };
}

#endif
