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
 * Fecha: Tue Sep 12 18:30:54 CEST 2000
 * Autor: Antonio-M. Corbi Bellot
 * Email: acorbi@dlsi.ua.es
 */

#include "model.h"
#include "view.h"

using namespace acb_mvc;
/*
 * Constructor por defecto
 */
Model::Model()
{
}

/*
 * Destructor
 */
Model::~Model()
{
  std::list<View*>::iterator t;
  
  /*for(t = lv.begin(); t != lv.end(); t++)
    delete *t;*/
}

/*
 * update_view
 */
void
Model::update_views(View* except_v)
{
  std::list<View*>::iterator t;

  for(t = lv.begin(); t != lv.end(); t++)
    if ((*t) != except_v) // todas menos except_v
      {
	(*t)->update();
	//cout << "Actualizo " << *t << endl;
      }
  //else cout << "No actualizo " << *t << endl;
}
