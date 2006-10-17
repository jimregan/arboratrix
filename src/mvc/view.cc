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

/*
 * Fecha: Tue Sep 12 16:42:18 CEST 2000
 * Autor: Antonio-M. Corbi Bellot
 * Email: acorbi@dlsi.ua.es
 */

#include "view.h"
#include "model.h"

using namespace acb_mvc;

/*
 * Constructor por defecto
 */
View::View (Model& m)
{
  model = &m;

  // La vista se anyade automaticamente a su modelo.
  model->add_view(this);
}

/*
 * Destructor
 */
View::~View ()
{

}

/*
 * update_model()
 */
void
View::update_model ()
{
  // Implementacion por defecto
  model->update_views(this);
}
