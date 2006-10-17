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
 * Creado:     Thu Jan  9 18:11:13 CET 2003
 * Modificado: Thu Jan  9 18:11:13 CET 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "canvas.h"
#include "canvas_item.h"

using namespace acb_mvc;

/*
 * Constructor por defecto.
 */
Canvas_item::Canvas_item(acb_mvc::Model& m, const Canvas* c, bool auto_add):
  View(m), shadow_delta(5), canvas(c)
{
  selected = false;
  hidden = false;

  // Iniciación del rectángulo de este item
  r.x = 0; r.y = 0;
  r.width = r.height = 0;

  // Ultimo click del raton fue en...
  dxc = dyc = 0;

  // Por ultimo el item se añade a la lista de items de su canvas.
  if (auto_add)
    {
      const_cast<Canvas*>(canvas)->add_item(this);
      //std::cout << "AUTO_ADD\n";
    }

  // Por defecto los canvas_items se pueden mover
  draggable = true;
  //Reserva memoria para los GCs
  create_pens();
}

/*
 * Destructor.
 */
Canvas_item::~Canvas_item()
{
  g_object_unref(G_OBJECT(text_gc));
  g_object_unref(G_OBJECT(line_gc));
  g_object_unref(G_OBJECT(rect_gc));
}

void
Canvas_item::mouse_drag(int x, int y, bool force_drag) 
{
  if (force_drag)
    {
      clear_from_canvas(); 
      set_x(x-dxc); set_y(y-dyc); 
      //update();
      draw();
    } 
  else
    //if (draggable)
    {
      clear_from_canvas(); 
      set_x(x-dxc); set_y(y-dyc); 
      //update();
      draw();
    } 
}

void
Canvas_item::create_pens()
{
  GdkColormap* colormap;
  GdkWindow*   w = canvas->get_window();

  //std::cerr << "Citem::create_pens\n";

  colormap = gtk_widget_get_colormap(canvas->get_widget());
  text_gc  = gdk_gc_new(w);
  rect_gc  = gdk_gc_new(w);
  line_gc  = gdk_gc_new(w);
}

void
Canvas_item::compute_shadow()
{
  // Sombra inferior
  rs1.x      = r.x + shadow_delta;
  rs1.y      = r.y + r.height;
  rs1.width  = r.width;
  rs1.height = shadow_delta;

  // Sombra derecha
  rs2.x      = r.x + r.width;
  rs2.y      = r.y + shadow_delta;
  rs2.width  = shadow_delta;
  rs2.height = r.height;
}

void
Canvas_item::draw_shadow()
{
  compute_shadow();
  canvas->draw_rectangle(rs1, true);
  canvas->draw_rectangle(rs2, true);
}

bool
Canvas_item::is_point_inside(int x, int y)
{
  Rectangle aux;
  Rectangle dest;
  bool      inside = false;

  // Rectangulo del punto: x,y,1,1
  aux.x      = x;
  aux.y      = y;
  aux.width  = 1;
  aux.height = 1;

  inside = gdk_rectangle_intersect(&r, &aux, &dest);
  
  return inside;
}

void
Canvas_item::clear_from_canvas(bool generate_event)
{
  GdkWindow* da_w = canvas->get_window();
  int        ew = 1; //anchura extra en pixels del rectangulo borrado.

  //1 El rectangulo que ocupa el item + la sombra
  if (generate_event)
    gdk_window_clear_area_e(da_w, 
			    r.x - ew, r.y - ew,
			    r.width + shadow_delta + ew,
			    r.height + shadow_delta + ew);
  else
    gdk_window_clear_area(da_w, 
			  r.x - ew, r.y - ew,
			  r.width + shadow_delta + ew, 
			  r.height + shadow_delta + ew);
}

bool
Canvas_item::operator< (const Canvas_item &ci) const
{
  //std::cerr << "COMPARO" << r.x << " " << ci.r.x << endl;
  return r.x < ci.r.x; 
}

bool
Canvas_item::citem_less(const Canvas_item* c1, const Canvas_item* c2)
{
  return *c1 < *c2;
}

bool
Canvas_item::intersects_with(const Canvas_item* ci)
{
  Rectangle ir;

  return gdk_rectangle_intersect(&r, const_cast<Rectangle*>(&ci->r), &ir);
}

void
Canvas_item::set_canvas_gcs()
{
  const_cast<Canvas*>(canvas)->set_gcs(line_gc, rect_gc, text_gc);
}
