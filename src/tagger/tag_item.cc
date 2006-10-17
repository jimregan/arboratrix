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
 * Creado:     Thu May  8 19:03:22 CEST 2003
 * Modificado: Thu May  8 19:03:22 CEST 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@peurl.dlsi.ua.es
 */

#include <gtk/gtk.h>
#include <gui/fixed_tip.h>
#include <tagger_view.h>
#include <tag_item.h>
#include <canvas.h>

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Tag_item::Tag_item(Tag_model& m, const Canvas* c, bool auto_add):
  Text_item(m, c, auto_add)
{
  draggable = true;

  // Asignamos los colores a los GCs
  set_colors();

  as_triangle = false;
}

/*
 * Destructor.
 */
Tag_item::~Tag_item()
{

}

void
Tag_item::show_properties(int x, int y)
{
  std::stringstream s;
  std::string       info, w;
  Fixed_tip*        t = new Fixed_tip;

  s << *this << " ocupa la columna: " << get_column();
  while (not s.eof ())
    {
      s >> w;
      info += w+" ";
    }

  if (get_model()->is_leaf())
    {
      info += "\n";
      info += "Es nodo hoja.";
    }

  if (as_triangle)
    {
      info += "\n";
      info += "Es un triangulo.";
    }

  // Tooltip durante 3sg.
  t->show(x, y, info, 3000);
}

void
Tag_item::set_colors()
{
  GdkColor     c;
  GdkColormap* colormap;
  //  GdkWindow*   w = canvas->get_window();


  //std::cerr << "Tag_item::set_colors\n";

  colormap = gtk_widget_get_colormap(canvas->get_widget());

  // GC para el contenido del item y las lineas hacia sus hijos
  // Me quedo con los puestos en Text_item::set_colors

  // GC para el rectangulo de seleccion
  //c.red = 0; c.green = 50000; c.blue = 25000;
  gdk_color_parse("navy", &c);
  if (not gdk_colormap_alloc_color(colormap, &c, false, true))
    {
      //std::cerr << "Tag_item 2\n";
    }

  gdk_gc_set_foreground(rect_gc, &c);
  //gdk_gc_set_line_style(rect_gc, GDK_LINE_ON_OFF_DASH);
  gdk_gc_set_line_attributes(rect_gc,
			     1, GDK_LINE_ON_OFF_DASH,
			     GDK_CAP_NOT_LAST, GDK_JOIN_MITER);
}

void
Tag_item::draw_lines_to_children()
{
  Point       to;
  Point       from;
  Point       hp1;
  Point       hp2;
  Text_model* m = get_model();
  //std::list<Text_item*>* til = m->get_children_text_items();
  

  if (not as_triangle)
    Text_item::draw_lines_to_children();
  else
    if (m->get_num_children() > 0) // Si tiene hijos
      {
	Text_item* fc = get_first_child();
	Text_item* lc = get_last_child();

	set_canvas_gcs();
	canvas->set_line_mode_xor();

	//1- Linea diagonal izquierda
	from = out_point(fc);
	hp1 = to = fc->in_point();

	/*hp1.x = to.x = children.front ()->get_x() - 
	  children.front ()->get_drawing_offset();*/
	canvas->draw_line(from.x, from.y, to.x, to.y);

	//-----------------------------

	//2- Linea diagonal derecha
	from = out_point(lc);
	hp2 = to = lc->in_point();

	/*hp2.x = to.x =  children.back ()->get_x() + 
	  children.back ()->get_w() + 
	  children.back ()->get_drawing_offset();*/
	canvas->draw_line(from.x, from.y, to.x, to.y);

	//3- Linea horizontal hp1---hp2
	canvas->draw_line(hp1.x, hp1.y, hp2.x, hp2.y);
      }
}

void
Tag_item::show_feedback_for(Canvas_item* son, bool shift, bool control)
{
  Text_item*   tis = dynamic_cast<Text_item*>(son);
  Tagger_view* tv  = const_cast<Canvas*>(canvas)->get_tagger_view();
  bool         trace_possible;

  trace_possible = get_model()->can_create_trace(tis->get_model());

  if (is_triangle())
    tv->show_statusbar_message(_("The node under the mouse is a triangle."),
			       2000);
  else
    {
      if ((shift) and (not control))          //Queremos adjunto
	{
	  if (tis->can_have_sons())
	    tv->show_statusbar_message(_("The node under the mouse is a valid"
					 " sister for the adjunct you are dragging."),
				       2000);
	  else
	    tv->show_statusbar_message(_("The node you are dragging is not valid"
					 " for creating an adjunct."),
				       2000);
	}
      else if ((not shift) and (not control)) //Queremos colgar un hijo a un nodo.
	{
	  tv->show_statusbar_message(_("The node under the mouse is a valid"
				       " parent for the node you are dragging."),
				     2000);
	}
      else if ((not shift) and (control))      //Queremos crear traza.
	{
	  if (trace_possible)
	    tv->show_statusbar_message(_("The node under the mouse is a valid"
					 " parent for creating a trace."),
				       2000);
	  else
	    tv->show_statusbar_message(_("The node under the mouse is not a valid"
					 " parent for creating a trace."),
				       2000);
	}
    }
}
