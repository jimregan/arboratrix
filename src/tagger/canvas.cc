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
 * Creado:     Mon Dec 30 17:09:03 CET 2002
 * Modificado: Mon Dec 30 17:09:03 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include <gui/acb_gui.h>
#include <canvas.h>
#include <tagger_view.h>
#include <canvas_item.h>
#include <text_item.h>
#include <tag_item.h>
#include <sentence.h>

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Canvas::Canvas(Tagger_view* tv, const std::string& cname)
{
  ctv = tv;

  parent_window = tv->get_widget();
  g_return_if_fail(parent_window != NULL);

  the_widget = Widget::lookup_widget(parent_window, cname);
  g_return_if_fail(the_widget != NULL);

  // Como siempre...
  build();

  // Conectamos los callbacks de bajo nivel.
  connect_callbacks();

  // Los contextos graficos estan vacios.
  line_gc = rect_gc = text_gc = NULL;

  pl  = NULL;
  pfd = NULL;

  // Crea el objeto pango-layout
  create_font_data("Sans 18");

  // Inicialmente no hay lista de items ni frase.
  cs  = NULL;
  cil = NULL;

  // Borramos la zona de dibujo.
  //clear();
}

Canvas::~Canvas()
{
  // Los objetos relacionados con PANGO
  //g_free(pl);
  //g_free(pfd);
}

void
Canvas::empty()
{
  cs = NULL;
  cil = NULL;
  cisl.clear();

  clear();
}

void
Canvas::force_redraw() const
{
  gdk_window_clear_area_e(the_widget->window,
			  0,0,
			  the_widget->allocation.width,
			  the_widget->allocation.height);

  //gdk_window_clear (the_widget->window);
}

void
Canvas::force_redraw(const Rectangle& r)
{
  gdk_window_clear_area_e(the_widget->window,
			  r.x, r.y, r.width, r.height);
}

void
Canvas::force_redraw(const Canvas_item* ci)
{
  force_redraw(ci->get_rect());
}

void
Canvas::clear()
{
  if (not GTK_WIDGET_REALIZED(the_widget))
    gtk_widget_realize(the_widget);

  gdk_window_clear(the_widget->window);

  // Draw background
  /*gdk_draw_rectangle(the_widget->window,
		     the_widget->style->bg_gc[0], //FIXME: no me gusta
		     TRUE,
		     0, 0,
		     the_widget->allocation.width,
		     the_widget->allocation.height);*/
  //force_redraw ();
}

void
Canvas::expose(GdkEventExpose* event)
{
  //std::cerr << "Canvas: Expose event\n";

  //tti->update();
  if (cil)
    for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
      {
	//(*i)->update();
	(*i)->draw();
      }
}

void
Canvas::layout_tree()
{
  if (cs)
    {
      cs->layout_tree_in_screen();
      force_redraw();
    }
}

void 
Canvas::button_press(GdkEventButton* event)
{
  /*std::cerr << "Canvas: Button_press event (" 
    << event->button << ")\n";*/

  switch (event->button) //Distingo entre boton1 y boton3
    {
    case 1: //Seleccion/deseleccion con boton 1
      left_button_press(event);
      break;
    case 2:
      layout_tree();
      break;
    case 3: //Menu pop-up correspondiente.
      right_button_press(event);
      break;
    default:
      break;
    }
}

void 
Canvas::button_release(GdkEventButton* event)
{
  Point        p;
  Text_item*   np; // new_parent
  Text_item*   tis; //text_item selected
  bool         shift = event->state & GDK_SHIFT_MASK;
  bool         control = event->state & GDK_CONTROL_MASK;

  //std::cerr << " Sueltas el boton.\n";

  if ((action == kClearList) and (cs))
    {
      deselect_all_items();
      compute_selected_items();
    }
  else
    if ((not has_moved) and (cs))
      {
	switch (action)
	  {
	  case kNothing:
	    break;
	  case kAppend:
	    if (cis)
	      {
		cis->set_selected(true);
	      }
	    compute_selected_items();
	    break;
	  case kRemoveThis:
	    if (cis)
	      cis->set_selected(false);
	    compute_selected_items();
	    break;
	  case kOnlyThis:
	    deselect_all_items();
	    if (cis)
	      cis->set_selected(true);
	    compute_selected_items();
	    break;
	  case kClearList:
	    deselect_all_items();
	    compute_selected_items();
	    break;
	  default:
	    break;
	  }
      }
    else
      {
	//compute_selected_items();

	if (cs) // Si hay frase, el usario intenta poner hijos a un nodo.
	  {
	    p.x = static_cast<int>(event->x);
	    p.y = static_cast<int>(event->y);

	    /* Busco item bajo el punto 'p' que no sea el actualmente
	     * seleccionado: 'cis'.*/
	    np = dynamic_cast<Text_item*>(get_item_at(p, cis));

	    //if (np and np->is_tag ()) // Hay item abajo que puede ser padre
	    if (np and np->can_have_sons())
	      {
		if (not dynamic_cast<Tag_item*>(np)->is_triangle())
		  {
		    /*Si el receptor del hijo _no_ es un triangulo...
		     * El usuario puede modificar la accion de soltar
		     * el boton pulsando SHIFT o CONTROL, pero no
		     * ambas */

		    if ((not shift) and (not control)) //Queremos colgar un hijo a un nodo
		      {
			cis->restore_x_coord();
			tis = dynamic_cast<Text_item*>(cis);
			np->get_model()->add_child(tis->get_model());
		      }
		    else if ((shift) and (not control))  //Queremos un adjunto...
		      {
			Tag_item* nti = dynamic_cast<Tag_item*>(np);

			cis->restore_x_coord();
			tis = dynamic_cast<Text_item*>(cis);
			cs->create_adjoint(tis, nti);
		      }
		    else if ((not shift) and (control)) //Queremos una traza
		      {
			cis->restore_x_coord();
			tis = dynamic_cast<Text_item*>(cis);
			create_trace(np, tis);
		      }
		  }
		else
		  {
		    //El receptor es un triangulo

		    if (not dynamic_cast<Text_item*>(cis)->can_have_sons())
		      {
			//A un triangulo solo puedo añadir hojas.
			cis->restore_x_coord();
			tis = dynamic_cast<Text_item*>(cis);
			np->get_model()->add_child(tis->get_model());
		      }
		    else
		      {
			//Y si no es asi, avisamos al usuario.
			acb_gui::Info_dialog d(_("Warning:"), 
					       _("Only leaves can be added to a triangle."));
			d.show();
		      }
		  }


		//reparent_node_option(np, shift);
		layout_tree();
	      }
	    else
	      if (has_moved)
		layout_tree();
	  }
      }
}

void
Canvas::right_button_press(GdkEventButton *ev)
{
  Point p;

  p.x = static_cast<int>(ev->x);
  p.y = static_cast<int>(ev->y);

  //Si hay frase activa en este momento...
  if ((cs) and (not cs->is_finished()))
    {
      cis = get_item_at(p);
      if ((cis) and (not cis->is_selected()))
	{
	  cis->save_x_coord(); //FIXME: 

	  deselect_all_items();
	  cis->set_selected(true);
	  compute_selected_items();
	  
	  ctv->cm_enable_items();
	}
      else if (count_selected_items() > 0)
	{
	  ctv->cm_enable_items();
	}
      else //No hay items selec., pero miro si alguno bajo el raton.
	{
	  ctv->cm_disable_items();
	}
    }
  else
    {
      ctv->cm_disable_items();
    }

  ctv->show_popup_menu(ev);
  /*gtk_menu_popup (GTK_MENU(cm), NULL, NULL, NULL, NULL, 
    ev->button, ev->time);*/
}

void
Canvas::left_button_press(GdkEventButton *event)
{
  Point  p;
  bool   shift   = event->state & GDK_SHIFT_MASK;
  bool   control = event->state & GDK_CONTROL_MASK;

  if ((cs) and cs->is_finished())
    {
      acb_gui::Info_dialog d(_("Warning:"), 
			     _("This sentence can't be modified as is marked as finished."));
      d.show();
      
      action = kClearList;
    }
  else
    {
      //Inicialmente no hay movimiento...
      has_moved = false;

      //Ajustamos 'search-point' (sp) para el 'find_if'...
      sp.x = p.x = static_cast<int>(event->x);
      sp.y = p.y = static_cast<int>(event->y);

      /*std::cerr << "p.x= " << p.x << " p.y= " << p.y 
	<< " xr=" << x_root << " yr=" << y_root << std::endl;*/

      // canvas_item_selected...
      cis = get_item_at(p);
      if (cis)
	cis->save_x_coord(); //FIXME: 

      /* Ajustamos las coordenadas de cada item respecto al ultimo click
       * del ratón.*/
      set_xy_click_all_items(p);

      if (control) // Si se pulsa CONTROL quiero mover todos los items
	{
	  //#if 0
	  if (cis)
	    {
	      double x_root;
	      double y_root;

	      //Debo obtener las coordenadas globales del ratón para el tooltip...
	      gdk_event_get_root_coords (reinterpret_cast<GdkEvent*>(event), 
					 &x_root, &y_root);

	      cis->show_properties(static_cast<int>(x_root),
				   static_cast<int>(y_root));
	    }
	  //#endif
	  action = kAllItems;
	}
      else if (shift)
	{
	  action = kNothing;
	  if (cis)
	    if (not cis->is_selected())
	      action = kAppend;
	    else
	      action = kRemoveThis;
	}
      else
	{
	  if (cis)
	    action = kOnlyThis;
	  else
	    action = kClearList;
	}
    }
}

void 
Canvas::motion_notify(GdkEventMotion* event)
{
  int  x       = static_cast<int>(event->x);
  int  y       = static_cast<int>(event->y);
  bool shift   = event->state & GDK_SHIFT_MASK;
  bool control = event->state & GDK_CONTROL_MASK;

  has_moved = true;

  if (event->is_hint)
    {
      gtk_widget_get_pointer(the_widget, &x, &y);
    }

  if (action == kAllItems)
    {
      /* Caso especial, el usuario mueve todos los items pq a lo mejor
       * alguno se ha salido de la pantalla por arriba y/o a la
       * izquierda. */
      //move_all_items(x - sp.x, y - sp.y);
      move_all_items(x, y);
      sp.x = x; sp.y = y;
    }
  else if (cis)
    {
      switch (action)
	{
	case kNothing:
	  break;
	case kAppend:
	  //t->as->add_to_selected(t->is);
	  cis->set_selected(true);
	  compute_selected_items();
	  break;
	case kRemoveThis:
	  //t->as->remove_from_selected(t->is);
	  cis->set_selected(false);
	  compute_selected_items();
	  break;
	case kOnlyThis:
	  //Al mover el raton, tenemos un caso especial...
	  if (not cis->is_selected())
	    {
	      deselect_all_items();
	      //t->as->add_to_selected(t->is);
	      cis->set_selected(true);
	    }
	  compute_selected_items();
	  break;
	case kClearList:
	  deselect_all_items();
	  compute_selected_items();
	  break;
	default:
	  action = kNothing;
	  break;
	}
      action = kNothing;

      /*if (cis->is_draggable())
	{*/
      //move_selected_items(x - sp.x, y - sp.y);
      move_selected_items(x, y);
      sp.x = x; sp.y = y;

      show_feedback(shift, control);
      //}
    }
}

void
Canvas::show_feedback(bool shift, bool control)
{
  Canvas_item* di;
  int          x, y;
  static int   count = 0;
  

  if ((++count % 5) == 0)
    {
      count = 0;

      //std::cout << "FEEDBACK\n";
      gtk_widget_get_pointer(the_widget, &x, &y);
      di = get_item_at(x, y, cis);

      if (di)
	{
	  //ctv->show_statusbar_message("Tenemos item abajo", 1000);
	  di->show_feedback_for(cis, shift, control);
	}
    }
}

void
Canvas::configure(GdkEventConfigure* event)
{
  clear();
}

void
Canvas::set_line_mode_xor() const
{
  GdkColor     gdk_color;
  GdkColormap* colormap;

  colormap = gtk_widget_get_colormap(the_widget);
  gdk_color_parse("white", &gdk_color);
  gdk_color_alloc(colormap, &gdk_color);
  gdk_gc_set_foreground(line_gc, &gdk_color);

  gdk_gc_set_function(line_gc, GDK_XOR);
}

void
Canvas::set_line_mode_copy() const
{
  GdkColor     gdk_color;
  GdkColormap* colormap;

  colormap = gtk_widget_get_colormap(the_widget);
  gdk_color_parse("black", &gdk_color);
  gdk_color_alloc(colormap, &gdk_color);
  gdk_gc_set_foreground(line_gc, &gdk_color);
 
  gdk_color_alloc(colormap, &gdk_color);
  gdk_gc_set_foreground(line_gc, &gdk_color);

  gdk_gc_set_function(line_gc, GDK_COPY);
}

void
Canvas::set_line_mode_on_off() const
{
  gdk_gc_set_line_attributes(line_gc, 2, GDK_LINE_ON_OFF_DASH,
                             GDK_CAP_NOT_LAST,GDK_JOIN_MITER);
}

void
Canvas::set_line_mode_on() const
{
  gdk_gc_set_line_attributes(line_gc, 1, GDK_LINE_SOLID,
                             GDK_CAP_NOT_LAST,GDK_JOIN_MITER);
}

void
Canvas::draw_point(int x, int y, int w) const
{
  if (w == 1)
    gdk_draw_point (the_widget->window, line_gc, x, y);
  else
    {
      gdk_draw_rectangle (the_widget->window, line_gc, true,
			  x, y,
			  w, w);
    }
}

void
Canvas::draw_point(const Point& p, int w) const
{
  draw_point (p.x, p.y, w);
}

void
Canvas::draw_line(int x1, int y1, int x2, int y2) const
{
  gdk_draw_line(the_widget->window, line_gc, x1, y1, x2, y2);
}

void
Canvas::draw_rectangle(const Rectangle& r, bool filled) const
{
  if (not filled)
    gdk_draw_rectangle (the_widget->window, rect_gc, false,
			r.x, r.y,
			r.width, r.height);
  else
    gdk_draw_rectangle (the_widget->window, rect_gc, true,
			r.x, r.y,
			r.width, r.height);
}

void
Canvas::draw_arc(const Rectangle& r, gint a1, gint a2, bool filled) const
{
  gdk_draw_arc(the_widget->window, line_gc, filled,
	       r.x, r.y, r.width, r.height,
	       a1*64, a2*64);
}

void
Canvas::draw_text(int  x, int y, const std::string& str) const
{
  gchar*    cstr;
  gsize     br, bw;
  int       w, h;
  Rectangle r2;

  cstr = g_locale_to_utf8 (str.c_str(), str.length(), &br, &bw, NULL);
  pango_layout_set_text(pl, cstr, bw);
  g_free(cstr);
  gdk_draw_layout(the_widget->window, text_gc, x, y, pl);

  pango_layout_get_pixel_size(pl, &w, &h);
  r2.x = x; r2.y = y; r2.width = w; r2.height = h;

  /*std::cout << "x= " << r2.x << ", y= " << r2.y << ", w= " 
    << r2.width << ", h= " << r2.height << std::endl;*/

  draw_rectangle(r2);
}

void
Canvas::draw_text_item(const Text_item* ti) const
{
  //El texto
  gdk_draw_layout(the_widget->window, text_gc,
		  ti->get_x(), ti->get_y(),
		  ti->get_text_pango_layout());

  //El indice
  if (ti->has_index())
    gdk_draw_layout(the_widget->window, text_gc,
		    ti->get_x()+ti->get_w(), ti->get_y()+5,
		    ti->get_index_pango_layout());
}

void
Canvas::create_font_data(const std::string& font_name)
{
  if (pl==NULL)
    //g_free(pl);
    pl = gtk_widget_create_pango_layout(the_widget, "");

  if (pfd==NULL)
    //g_free(pfd);
  pfd = pango_font_description_from_string(font_name.c_str());

  pango_layout_set_font_description(pl, pfd);
}

Canvas_item*
Canvas::get_item_at(int x, int y, const Canvas_item* not_this)
{
  std::list<Canvas_item*>::iterator i;
  Canvas_item* result = NULL;
  bool         end = false;

  if (cil)
    {
      for (i = cil->begin(); ((i != cil->end()) and not end); i++)
	{
	  // Si el punto 'p' esta dentro de un hijo y este no es el no_buscado
	  if (((*i)->is_point_inside(x, y)) and ((*i) != not_this))
	    {
	      end = true;
	      result = *i;
	      result->set_xy_click(x, y);
	    }
	  /* ESPERAMOS AUN A ESTO
	     else // miramos en los hijos del hijo actual
	     {
	     result = (*i)->get_item_at(p, not_this);
	     end = (result != NULL);
	     }*/
	}
    }
  return result;
}

void
Canvas::add_item(Canvas_item* child)
{
  if (child != NULL)
    if (cil != NULL)
      cil->push_back(child);
    else
      {
	//std::cerr << "Canvas::add_item: no existe la lista de items.\n";
      }
}

void
Canvas::remove_item(Canvas_item* child)
{
  if (child != NULL)
    if (cil != NULL)
      cil->remove(child);
    else
      {
	//std::cerr << "Canvas::remove_item: no existe la lista de items.\n";
      }
}

void
Canvas::set_sentence(Sentence* s)
{
  if (s)
    {
      cs = s;
      set_item_list(cs->get_item_list());
    }
  else
    empty();
}

void
Canvas::select_all_items()
{
  if (cil)
    for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
      {
	(*i)->set_selected(true);
      }
}

void
Canvas::deselect_all_items()
{
  if (cil)
    for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
      {
	(*i)->set_selected(false);
      }
}

void
Canvas::invert_selection()
{
  if (cil)
    for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
      {
	(*i)->toggle_selected();
      }
}

int
Canvas::count_selected_items()
{
  int sc = 0;

  for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
    {
      if ((*i)->is_selected())
	sc++;
    }
  return sc;
}

Canvas::Itemlist&
Canvas::get_selected_items()
{

  compute_selected_items(false);
  return cisl;
}

void
Canvas::compute_selected_items(bool recursive)
{
  cisl.clear();

  for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
    {
      if ((*i)->is_selected())
	{
	  cisl.push_back(*i);
	  //std::cout << "Item seleccionado\n";
	  if (recursive)
	    (*i)->compute_selected_items(cisl);
	}
    }
}

void
Canvas::move_selected_items(int dx, int dy)
{

#if 0
  for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
    {
      if ((*i)->is_selected())
	{
	  (*i)->mouse_drag(dx, dy);
	  //force_redraw(*i);
	}
      else
	{
	  //(*i)->clear_from_canvas();
	  //(*i)->update();
	}
      //force_redraw(*i);
    }
#endif

  //compute_selected_items();
  for(std::list<Canvas_item*>::iterator i = cisl.begin(); i != cisl.end(); 
      i++)
    {
      (*i)->mouse_drag(dx, dy);
    }

  force_redraw();
}

void
Canvas::move_all_items(int dx, int dy)
{
  if (cil)
    {
      for(std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); i++)
	{
	  (*i)->mouse_drag(dx, dy, true);
	  //force_redraw(*i);
	}
      force_redraw();
    }
}

void
Canvas::set_xy_click_all_items(Point p)
{
  if (cil)
    {
      for (std::list<Canvas_item*>::iterator i = cil->begin(); i != cil->end(); 
	   i++)
	{
	  (*i)->set_xy_click(p.x, p.y);
	}
    }
}

void
Canvas::create_trace(Text_item* dest, Text_item* orig)
{
  dest->create_trace(orig);
}
