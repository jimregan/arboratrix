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
 * Creado:     Thu Dec 26 17:13:50 CET 2002
 * Modificado: Thu Dec 26 17:13:50 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

//#include <time.h>
#include <tag_list.h>
#include <canvas.h>
#include <sentence.h>
#include <tagger_view.h>
#include <tag_item.h>

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Tag_list::Tag_list(GtkWidget* ww, const std::string& wln, 
		   const std::vector<std::string>& vt, Tagger_view* ptv):
  List_manager(ww, wln, vt), tv(ptv)
{
  GtkTargetEntry target_entry[1];

  build();
  init_widget();
  catch_enter_key();
  update_interface();

  reading_from_file = false;

  target_entry[0].target = "text/plain";
  target_entry[0].flags  = 0;
  target_entry[0].info   = 0;

  gtk_drag_dest_set(tv->get_canvas()->get_widget(),
		    static_cast<GtkDestDefaults>(GTK_DEST_DEFAULT_MOTION | 
						 GTK_DEST_DEFAULT_HIGHLIGHT |
						 GTK_DEST_DEFAULT_DROP),
		    target_entry,
		    1, //sizeof(target_entry) / sizeof(GtkTargetEntry),
		    static_cast<GdkDragAction>(GDK_ACTION_MOVE | GDK_ACTION_COPY));

  gtk_drag_source_set(the_list,
		      static_cast<GdkModifierType>(GDK_BUTTON1_MASK | GDK_BUTTON2_MASK),
		      target_entry,
		      sizeof(target_entry) / sizeof(GtkTargetEntry),
		      static_cast<GdkDragAction>(GDK_ACTION_MOVE | GDK_ACTION_COPY));
}

/*
 * Destructor.
 */
Tag_list::~Tag_list()
{
  //std::cout << "SL: destructor.\n";
}

bool
Tag_list::add_tag(Tag_info* nt)
{
  //save_row_data(ns);
  //row_data = ns;
  bool added;
 
  added = add_element(nt->get_tag(), nt);

  return added;
}

bool
Tag_list::can_be_added(const std::string& s)
{
  if (reading_from_file)
    return true;
  else
    return not tv->get_model()->exists_tag(s);
}

/*
 * add_item
 */
void
Tag_list::add_item()
{
  std::string tstr = gtk_entry_get_text(GTK_ENTRY(entry));
  Tag_info*   pti;

  // Ahora al modelo
  pti = tv->get_model()->add_tag(tstr);
  // Y guardamos un ptro. al tag_info creado en add_tag
  //save_row_data(ns);
  row_data = pti;
  //std::cout << "Tag_list: Boton añadir pulsado\n";
}

/*
 * remove_item
 */
void
Tag_list::remove_item()
{
  Tag_info* pti;

  // plir contiene el puntero al ultimo elemento borrado de la lista.
  pti = static_cast<Tag_info*>(plir);
  if (pti)
    {
      //std::cout << "SL: borramos tag [" << *ptm << "]\n";
      tv->get_model()->remove_tag(pti);
    }
  else
    {
      std::cout << "Error, tag NULO\n";
    }

  //std::cout << "Tag_list: Boton remove pulsado\n";
}

/*
 * init_subwidgets
 */
void
Tag_list::init_subwidgets()
{
  
  add_button    = Widget::lookup_widget(parent_window, "tag_add_button");
  remove_button = Widget::lookup_widget(parent_window, "tag_delete_button");
  entry         = Widget::lookup_widget(parent_window, "tag");
  the_list      = Widget::lookup_widget(parent_window, "tag_list");
}

void
Tag_list::connect_callbacks()
{
  // Para los botones add y remove.
  List_manager::connect_callbacks();


  gtk_signal_connect(GTK_OBJECT(the_list), "drag_begin",
		     GTK_SIGNAL_FUNC(dnd_begin_cb), this);
  gtk_signal_connect(GTK_OBJECT(the_list), "drag_end",
		     GTK_SIGNAL_FUNC(dnd_end_cb), this);
  gtk_signal_connect(GTK_OBJECT(the_list), "drag_motion",
		     GTK_SIGNAL_FUNC(dnd_dragmotion_cb), this);
  gtk_signal_connect(GTK_OBJECT(tv->get_canvas()->get_widget()), "drag_data_received",
		     GTK_SIGNAL_FUNC(dnd_data_received_cb), this);
  gtk_signal_connect(GTK_OBJECT(the_list), "drag_data_get",
		     GTK_SIGNAL_FUNC(dnd_data_request_cb), this);
}

/*
 *	DND "drag_begin" handler, this is called whenever a drag starts.
 */
void
Tag_list::dnd_begin_cb(GtkWidget* widget, GdkDragContext* dc, gpointer data)
{
  //Tag_list* tl = static_cast<Tag_list*>(data);

  //std::cout << "Comienza el arrastre...\n";

  /* Put any needed drag begin setup code here. */
  gtk_drag_set_icon_stock(dc, GTK_STOCK_BOLD, 10, 10);
}

/*
 *      DND "drag_end" handler, this is called when a drag and drop has
 *	completed. So this function is the last one to be called in
 *	any given DND operation.
 */
void
Tag_list::dnd_end_cb(GtkWidget* widget, GdkDragContext* dc, gpointer data)
{
  //Tag_list* tl = static_cast<Tag_list*>(data);

  //std::cout << "FIN del arrastre...\n";

  /* Put any needed drag begin setup code here. */
}

/*
 *	DND "drag_motion" handler, this is called whenever the 
 *	pointer is dragging over the target widget.
 */
gboolean 
Tag_list::dnd_dragmotion_cb(GtkWidget* widget, GdkDragContext* dc,
			    gint x, gint y, guint t,
			    gpointer data)
{
  bool          same_widget;
  GdkDragAction suggested_action;
  GtkWidget*    src_widget;
  GtkWidget*    tar_widget;
  //Tag_list*     tl = static_cast<Tag_list*>(data);

  if (dc == NULL)
    return false;

  //std::cout << "DND motion\n";

  /* Get source widget and target widget. */
  src_widget = gtk_drag_get_source_widget(dc);
  tar_widget = widget;

  /* Note if source widget is the same as the target. */
  same_widget = (src_widget == tar_widget) ? true : false;

  /* If this is the same widget, our suggested action should be
   * move.  For all other case we assume copy.
   */
  if(same_widget)
    {
      //std::cout << "DND motion sobre MISMO widget\n";
      suggested_action = GDK_ACTION_MOVE;
    }
  else
    suggested_action = GDK_ACTION_COPY;

  /* Respond with default drag action (status). First we check
   * the dc's list of actions. If the list only contains
   * move, copy, or link then we select just that, otherwise we
   * return with our default suggested action.
   * If no valid actions are listed then we respond with 0.
   */

  /* Only move? */
  if(dc->actions == GDK_ACTION_MOVE)
    gdk_drag_status(dc, GDK_ACTION_MOVE, t);
  /* Only copy? */
  else if(dc->actions == GDK_ACTION_COPY)
    gdk_drag_status(dc, GDK_ACTION_COPY, t);
  /* Only link? */
  else if(dc->actions == GDK_ACTION_LINK)
    gdk_drag_status(dc, GDK_ACTION_LINK, t);
  /* Other action, check if listed in our actions list? */
  else if(dc->actions & suggested_action)
    gdk_drag_status(dc, suggested_action, t);
  /* All else respond with 0. */
  else
    gdk_drag_status(dc, GDK_ACTION_DEFAULT, t);

  return false;
}

void
Tag_list::dnd_data_received_cb(GtkWidget* widget, GdkDragContext* dc,
			       gint x, gint y, 
			       GtkSelectionData* selection_data,
			       guint info, guint t, gpointer data)
{
  bool          same_widget;
  GtkWidget*    src_widget;
  GtkWidget*    tar_widget;
  Tag_list*     tl = static_cast<Tag_list*>(data);

  //std::cout << "DND data_received\n";

  /* Important, check if we actually got data.  Sometimes errors
   * occure and selection_data will be NULL.
   */
  if(selection_data == NULL)
    {
      //std::cout << "DND data_received es NULL\n";
      return;
    }
  if(selection_data->length < 0)
    {
      //std::cout << "DND data_received->length < 0\n";
      return;
    }

  /* Get source widget and target widget. */
  src_widget = gtk_drag_get_source_widget(dc);
  tar_widget = widget;

  /* Note if source widget is the same as the target. */
  same_widget = (src_widget == tar_widget) ? true : false;

  if (same_widget)
    {
      //std::cout << "DND data_received sobre el mismo widget\n";
    }
  else // Añadimos tag_item
    {
      /*std::cout << "Nuevo Tag_Item: " << tl->lst 
	<< " en x: " << x << " en y: " << y << std::endl;*/

      if (not tl->are_items_selected())
	{
	  //std::cout << "Abortamos DND.\n";
	  //gdk_drag_abort(dc, dc->start_time);
	  //gdk_drop_reply(dc, false, dc->start_time);

	  acb_gui::Info_dialog d(_("Warning:"), _("There's no tag to add."));
	  d.show();

	  gtk_drag_finish(dc, FALSE, FALSE, dc->start_time);
	}
      else
	{
	  Sentence* s = tl->tv->get_canvas()->get_sentence();

	  if (s)
	    {
	      int             mx, my;
	      GdkModifierType state;
	      bool            create_triangle;  //Creamos triangulo si se pulsa CONTROL
	      Tag_item*       ti = s->add_tag_item(tl->lst,
						   dynamic_cast<Tag_model*>(s->get_root()),
						   x, y);

	      gdk_window_get_pointer (widget->window, &mx, &my, &state);
	      create_triangle = state & GDK_CONTROL_MASK;
	      if (create_triangle)
		ti->set_triangle(create_triangle);
	    }
	  else
	    {
	      acb_gui::Info_dialog d(_("Warning:"), _("There's no sentence to add tags to."));
	      d.show();
	    }
	}
    }
}

void
Tag_list::dnd_data_request_cb(GtkWidget* widget, GdkDragContext* dc,
			      GtkSelectionData* selection_data, 
			      guint info, guint t, gpointer data)
{
  //Tag_list*     tl = static_cast<Tag_list*>(data);
  char* cstrptr = "tl_dnd";

  gtk_selection_data_set(selection_data,
			 GDK_SELECTION_TYPE_STRING,
			 8,	/* 8 bits per character. */
			 reinterpret_cast<const guchar*>(cstrptr), 6);
}

void
Tag_list::selection_changed()
{
  //std::cout << "Ultimo TAG seleccionado: " << lst << std::endl;
}

void
Tag_list::show_add_warning()
{
  acb_gui::Info_dialog d(_("Warning:"), _("Tags can't be empty or duplicated."));
  d.show();
}
