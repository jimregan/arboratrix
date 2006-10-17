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
 * Creado:     Sun Jan 12 13:22:32 CET 2003
 * Modificado: Sun Jan 12 13:22:32 CET 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

//#include <iostream>
#include <sstream>
#include <list>
#include <algorithm>
#include <gui/fixed_tip.h>
#include <tagger_view.h>
#include <text_item.h>
#include <text_model.h>
#include <canvas.h>

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Text_item::Text_item(Text_model& m, const Canvas* c, bool auto_add):
  Canvas_item(m, c, auto_add)
{

  draggable = false;
  pl        = NULL;
  pfd       = NULL;
  pli       = NULL;
  pfdi      = NULL;

  set_x(0); set_y(0);

  create_font_data("Sans 10 bold");
  create_index_font_data("Sans 8");

  empty_category = m.is_empty_category();

  // Asignamos los colores a los GCs
  set_colors();

  update();
}

/*
 * Destructor.
 */
Text_item::~Text_item()
{
  // Los objetos relacionados con PANGO
  /*if (pl)
    g_free(pl);
  if (pfd)
    g_free(pfd);
  if (pli)
    g_free(pli);
  if (pfdi)
  g_free(pfdi);*/

  //std::cout << "Destruido text_item\n";
}

std::ostream&
operator<< (std::ostream& os, const Text_item& ti)
{
  os << *ti.get_model();

  return os;
}

void
Text_item::create_rendered_text()
{
  gchar*       cstr;
  std::string  str = get_model()->get_text();
  std::string  idx;
  int          w, h;
  //gsize        br, bw;

  //std::cout << "\tUTF8 [" << str << "](" << str.length() << ")\n";
  //cstr = g_locale_to_utf8 (str.c_str(), str.length(),&br, &bw, NULL);

  cstr = const_cast<gchar*>(str.c_str());

  /*if (pl)
    g_free(pl);*/

  if (not pl)
    {
      pl = gtk_widget_create_pango_layout(canvas->get_widget(), cstr);
      //g_free(cstr);
      if (pfd)
	{
	  pango_layout_set_font_description(pl, pfd);
	  //g_free(pfd);
	  //pfd = NULL;
	}
    }
  pango_layout_get_pixel_size(pl, &w, &h);
  set_w(w); set_h(h);
}

void
Text_item::create_rendered_index()
{
  gchar*       cstr;
  std::string  str = get_model()->get_index();
  int          w, h;
  //gsize        br, bw;

  //std::cout << "\tUTF8 [" << str << "](" << str.length() << ")\n";
  //cstr = g_locale_to_utf8 (str.c_str(), str.length(),&br, &bw, NULL);
  cstr = const_cast<gchar*>(str.c_str());

  /*if (pli)
    g_free(pli);*/

  if (not pli)
    {
      pli = gtk_widget_create_pango_layout(canvas->get_widget(), cstr);
      //g_free(cstr);
      if (pfdi)
	{
	  pango_layout_set_font_description(pli, pfdi);
	  //g_free(pfd);
	  //pfd = NULL;
	}
    }
  pango_layout_get_pixel_size(pli, &w, &h);
  /*std::cout << "W=" << w << "\n";
    std::cout << "H=" << h << "\n";*/
  //set_w(get_w()+w); set_h(get_h()+h);
}

void
Text_item::create_font_data(const std::string& font_name)
{
  if (pfd)
    g_free(pfd);

  pfd = pango_font_description_from_string(font_name.c_str());
  create_rendered_text();
  //pango_layout_set_font_description(pl, pfd);
}

void
Text_item::create_index_font_data(const std::string& font_name)
{
  if (pfdi)
    g_free(pfdi);

  pfdi = pango_font_description_from_string(font_name.c_str());
  create_rendered_index();
  //pango_layout_set_font_description(pl, pfd);
}

void
Text_item::draw()
{
  if (not hidden)
    {
      // 1
      //clear_from_canvas();

      //std::cout << "UPDATE[" << *this << "]\n";

      //Ajustamos los GCs del canvas
      set_canvas_gcs();

      //create_rendered_text();
      //create_rendered_index();
      
      if (selected)
	{
	  draw_shadow();
	  canvas->draw_rectangle(r);
	}

      canvas->draw_text_item(this);

      //draw_line_from_parent();
      draw_lines_to_children();

      // 2
      //update_children();
    }
}

void
Text_item::update()
{
  if (not hidden)
    {
      // 1
      //clear_from_canvas();

      //std::cout << "UPDATE[" << *this << "]\n";

      //Ajustamos los GCs del canvas
      //set_canvas_gcs();

      create_rendered_text();
      create_rendered_index();
      
      /*if (selected)
	{
	  draw_shadow();
	  canvas->draw_rectangle(r);
	  }*/

      //canvas->draw_text_item(this);

      //draw_line_from_parent();
      //draw_lines_to_children();

      // 2
      //update_children();
    }
}

void 
Text_item::update_children()
{
  Text_model* m = get_model();

  if (m)
    m->update_children();
  else
    {
      //std::cerr << "No deberia suceder!!: Text_item::update_children\n";
      abort();
    }
}

int 
Text_item::get_children_width()
{
  Text_item* fc = get_first_child();
  Text_item* lc = get_last_child();
  int        w;

  w = lc->get_x() + lc->get_w() - fc->get_x();
  return w;
}

int
Text_item::get_pixel_span()
{
  int                    nc;
  int                    ps = 0;
  Text_model*            m  = get_model();
  std::list<Text_item*>* cl;

  if (m)
    {
      //std::cout << "Busco hijos de " << m->get_text() << "\n";
      nc = m->get_num_children();
      /*if (cl)
	std::cout << "Tiene (cl: " << cl << ") " << cl->size() << " hijos\n";*/

      if (nc < 1)
	{
	  //delete cl;
	  return get_w();
	}
      else
	{
	  cl = m->get_children_text_items();
	  for(std::list<Text_item*>::iterator i = cl->begin(); 
	      i != cl->end(); i++)
	    {
	      //std::cout << "Llamo pixel_span()" << ++j << "\n";
	      /*std::cout << "[" << **i << "] X: " << (*i)->get_x()
			<< " PS: " << (*i)->get_pixel_span()
			<< std::endl;*/

	      ps +=  (*i)->get_pixel_span();
	    }
	  delete cl;

	  if (nc > 1)
	    ps += (nc-1) * Text_item::get_pad_w();

	  //return (nc%2 != 0) ? ps : (ps + get_w()); 
	  return ps;
	}
    }
  else
    return 0; // No deberia ocurrir nunca...
}

int
Text_item::get_direct_pixel_span()
{
  int                    nc;
  int                    ps = 0;
  Text_model*            m  = get_model();
  std::list<Text_item*>* cl;

  if (m)
    {
      //std::cout << "Busco hijos de " << m->get_text() << "\n";
      nc = m->get_num_children();
      cl = m->get_children_text_items();
      /*if (cl)
	std::cout << "Tiene (cl: " << cl << ") " << cl->size() << " hijos\n";*/

      if (nc < 1)
	{
	  delete cl;
	  return get_w();
	}
      else
	{
	  for(std::list<Text_item*>::iterator i = cl->begin(); 
	      i != cl->end(); i++)
	    {
	      //std::cout << "Llamo pixel_span()" << ++j << "\n";
	      ps +=  (*i)->get_w();
	    }
	  delete cl;
	  return (nc%2 != 0) ? ps : (ps + get_w()); 
	}
    }
  else
    return 0; // No deberia ocurrir nunca...
}

int
Text_item::get_x_delta_in_span()
{
  int ts = get_children_width(); //get_pixel_span();
  int w  = get_w();
  //int nc = m->get_num_children();

  /*if (nc == 1)
    ts -= w;*/

  return (ts-w)/2;
}

void
Text_item::calc_x_pos()
{
  int x = get_x_delta_in_span();
  int offset_x;

  offset_x = get_first_child()->get_x();
  set_x(offset_x + x);

  /*std::cout << *this << " W:" << get_w() << " CW: " << get_children_width()
	    << " X: " << get_x() << " FCX: " << offset_x
	    << std::endl;*/

}

void
Text_item::calc_y_pos()
{
  int l = get_model()->get_level();

  set_y(l * get_pad_h());
}

Text_item*
Text_item::get_parent()
{
  Text_model* parent_tm = get_model()->get_parent();

  return parent_tm->get_first_view();
}

void
Text_item::clear_from_canvas(bool generate_event)
{
  set_canvas_gcs();

  Canvas_item::clear_from_canvas();

  //Debe borrar las antiguas
  draw_lines_to_children();
  //draw_line_from_parent();
}

void
Text_item::draw_lines_to_children()
{
  Point                  to;
  Point                  from;
  Text_model*            m   = get_model();
  Text_model*            tdm = m->get_trace_destination();
  //Text_item*             tdi;
  Rectangle              tdr; //trace destination rectangle
  Rectangle              tor; //trace origin rectangle
  Rectangle              tfr; //trace final rectangle
  std::list<Text_item*>* til = m->get_children_text_items();

  if (til) // Si tiene hijos
    {
      set_canvas_gcs();
      canvas->set_line_mode_xor();
      for (std::list<Text_item*>::iterator i = til->begin(); 
	   i != til->end(); i++)
	{
	  from = out_point(*i);
	  to = (*i)->in_point();
	  canvas->draw_line(from.x, from.y, to.x, to.y);
	}
      canvas->set_line_mode_copy();
      delete til;
    }
  //Arco de una traza.
#if 0
  if (tdm)
    {
      //tdi contiene el text_item destino de la traza
      tdi = tdm->get_first_view();
      to = tdi->out_point(this);
      from.x = get_x();
      from.y = get_y() + (get_w()/2.0);

      canvas->set_line_mode_on_off();
      canvas->draw_line(from.x, from.y, to.x, to.y);
      canvas->set_line_mode_on();
    }
#endif

  //Probamos con elipses
  if (tdm)
    {
      tor = r;
      tdr = tdm->get_first_view()->get_rect();

      tfr.x      = std::min(tor.x, tdr.x);
      tfr.y      = std::min(tor.y, tdr.y);
      tfr.width  = std::max(tor.x + tor.width, tdr.x + tdr.width) - tfr.x;
      tfr.height = std::max(tor.y + tor.height, tdr.y + tdr.height) - tfr.y;

      canvas->set_line_mode_on_off();
      canvas->draw_arc(tfr, 90, 360, false);
      canvas->draw_rectangle(tfr);
      canvas->set_line_mode_on();      
    }
}

void
Text_item::draw_line_from_parent()
{
  Point      to;
  Point      from;
  Text_item* p = get_parent();

  if (p) // Si tiene padre
    {
      set_canvas_gcs();
      canvas->set_line_mode_xor();
      from = p->out_point(this);
      to = in_point();
      canvas->draw_line(from.x, from.y, to.x, to.y);
      canvas->set_line_mode_copy();
    }
}

Point
Text_item::in_point()
{
  Point      p;
  Text_item* parent = get_parent();

  if (parent)
    {
      if (parent->r.y <= r.y) //Su padre esta arriba de el
	p.y = r.y;
      else
	p.y = r.y + r.height;

      p.x = r.x + r.width/2;
    }
  return p;
}

Point
Text_item::out_point(const Canvas_item* c)
{
  Point      p;
  //Text_item* parent = get_parent();

  p.x = r.x + r.width/2;

  if (r.y <= c->get_y()) //El padre esta arriba del hijo 'c'
    p.y = r.y + r.height;
  else
    p.y = r.y;

  return p;
}

std::list<Text_item*>*
Text_item::get_children_text_items()
{
  Text_model*  m = get_model();

  return m->get_children_text_items();
}

Text_item*
Text_item::get_first_child()
{
  Text_model*            m  = get_model();
  Text_item*             ti = NULL;
  std::list<Text_item*>* cl;

  if (m)
    {
      cl = m->get_children_text_items();
      if (cl)
	{
	  cl->sort(Canvas_item::citem_less);
	  ti = *cl->begin();
	  delete cl;
	}
    }
  return ti;
}

Text_item*
Text_item::get_last_child()
{
  Text_model*            m  = get_model();
  Text_item*             ti = NULL;
  std::list<Text_item*>* cl;

  if (m)
    {
      cl = m->get_children_text_items();
      if (cl)
	{
	  cl->sort(Canvas_item::citem_less);
	  ti = cl->back();
	  delete cl;
	}
    }
  return ti;
}

void
Text_item::offset_by_x_with_children(int ox)
{
  Text_model*            m  = get_model();
  std::list<Text_item*>* cl;

  if (m)
    {
      offset_by_x(ox);
      cl = m->get_children_text_items();
      if (cl)
	{
	  for (std::list<Text_item*>::iterator i = cl->begin(); 
	       i != cl->end(); i++)
	    {
	      (*i)->offset_by_x(ox);
	    }
	  delete cl;
	}
    }
}

void
Text_item::show_properties(int x, int y)
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

  // Tooltip durante 3sg.
  t->show(x, y, info, 3000);
}

void
Text_item::show_feedback_for(Canvas_item* son, bool shift, bool control)
{
  //Text_item*   tis = dynamic_cast<Text_item*>(son);
  Tagger_view* tv  = const_cast<Canvas*>(canvas)->get_tagger_view();

  //if (di->can_have_sons())
  tv->show_statusbar_message(_("The node under the mouse is not a valid"
			       " parent for the node you are dragging."),
			     2000);
}

void
Text_item::set_colors()
{
  GdkColor     c;
  GdkColormap* colormap;
  //GdkWindow*   w = canvas->get_window();


  //std::cerr << "Text_item::set_colors\n";


  colormap = gtk_widget_get_colormap(canvas->get_widget());

  // GC para el texto del item y las lineas hacia sus hijos
  c.red = 0; c.green = 0; c.blue = 0;
  if (not gdk_colormap_alloc_color(colormap, &c, false, true))
    {
      //std::cerr << "Text_item 1\n";
    }
  gdk_gc_set_foreground(line_gc, &c); //lineas

  if (empty_category)
    {
        c.red = 0; c.green = 0; c.blue = 40000;
	if (not gdk_colormap_alloc_color(colormap, &c, false, true))
	  {
	  }
    }
  else
    if (get_model()->is_trace())
    {
        c.red = 0; c.green = 40000; c.blue = 0;
	if (not gdk_colormap_alloc_color(colormap, &c, false, true))
	  {
	  }
    }
  gdk_gc_set_foreground(text_gc, &c); //texto

  c.red = 65535; c.green = 65535; c.blue = 65535;
  gdk_colormap_alloc_color(colormap, &c, false, true);
  gdk_gc_set_background(line_gc, &c);
  gdk_gc_set_background(text_gc, &c);

  //set_line_width(2);

  // GC para el rectangulo de seleccion
  //c.red = 65000; c.green = 0; c.blue = 0;
  gdk_color_parse("red", &c);
  if (not gdk_colormap_alloc_color(colormap, &c, false, true))
    {
      //std::cerr << "Text_item 2\n";
    }
  gdk_gc_set_foreground(rect_gc, &c);
  //gdk_gc_set_line_style(rect_gc, GDK_LINE_ON_OFF_DASH);
  gdk_gc_set_line_attributes(rect_gc,
			     1, GDK_LINE_ON_OFF_DASH,
			     GDK_CAP_NOT_LAST, GDK_JOIN_MITER);
}

void
Text_item::compute_selected_items(std::list<Canvas_item*>& cisl)
{
  std::list<Text_item*>* cl;
  
  if (can_have_sons())
    {
      //std::cout << "]]Busco entre los hijos de " << *this << std::endl;
      cl = get_children_text_items();
      if (cl)
	{
	  for (std::list<Text_item*>::iterator i = cl->begin(); 
	       i != cl->end(); i++)
	    {
	      if (find(cisl.begin(), cisl.end(), *i) == cisl.end())
		{
		  cisl.push_back(*i);
		  //std::cout << "]]HIJO no seleccionado y añadido: " << **i << std::endl;
		}
	      (*i)->compute_selected_items(cisl);
	    }
	  delete cl;
	}
    }
}


void
Text_item::create_trace(Text_item* orig)
{
  bool got_trace = false;

  //La traza llega al modelo, y este nos dice si se hizo
  got_trace = get_model()->create_trace(orig->get_model());

  if (got_trace)
    {
    }
}
