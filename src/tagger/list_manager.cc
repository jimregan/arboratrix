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
 * Creado:     Thu Dec 26 12:53:35 CET 2002
 * Modificado: Thu Dec 26 12:53:35 CET 2002
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "list_manager.h"

using namespace acb_gui;

/*
 * Constructor por defecto.
 */
List_manager::List_manager(GtkWidget* wwindow,
			   const std::string& wln,
			   const std::vector<std::string>& vt)
{
  titulo = vt;

  parent_window = wwindow;
  g_return_if_fail(parent_window != NULL);

  the_widget = Widget::lookup_widget(parent_window, wln);

  g_return_if_fail(the_widget != NULL);
  // Inicialmente los widgets no estan 'iniciados'.
  add_button = remove_button = entry = the_list = NULL;
  // Y los punteros q usa la lista tampoco
  plis = plir = plvi = row_data = NULL;
}

/*
 * init_list
 */
void
List_manager::init_list()
{
  GtkTreeSelection* selection;

  model = create_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(the_list), model);
  g_object_unref(G_OBJECT(model));

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(the_list));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);
}

/*
 * Destructor.
 */
List_manager::~List_manager()
{
  clear();
  //std::cerr << "Eliminado List_manager.\n";
}

GtkWidget*
List_manager::create_gui()
{
  return the_widget;
}

void
List_manager::save_row_data(gpointer p)
{
  GtkListStore* store;

  store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(the_list)));
  gtk_list_store_set(store, &last_item, 1, p, -1);
}

bool
List_manager::add_element(const std::string& str, gpointer p)
{
  GtkTreeView*      treeview  = GTK_TREE_VIEW(the_list);
  GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);
  //GtkTreeModel*     model     = gtk_tree_view_get_model(treeview);
  GtkListStore*     store;
  GtkTreeIter       iter;
  gchar*            cstr;
  //gsize             br, bw;

  /*if (can_be_added(str))
    std::cout << "Añado " << str << std::endl;
  else
  std::cout << "No se puede añadir " << str << std::endl;*/

  if ((str.length() > 0) and (can_be_added(str)))
    {
      store = GTK_LIST_STORE(gtk_tree_view_get_model(treeview));

      gtk_list_store_append(store, &iter);
      //cstr = g_locale_to_utf8 (str.c_str(), str.length(), &br, &bw, NULL);
      cstr = const_cast<gchar*>(str.c_str());
      gtk_list_store_set(store, &iter, 0, cstr, -1);
      //g_free(cstr);

      // Por si queremos guardar algo en esta fila
      last_item = iter;

      if (p == NULL)
	{
	  // A la clase derivada, para q proporcione puntero
	  // interno al item representado por el texto
	  add_item(); 
	  // row_data debe tener valor dado en 'add_item'
	}
      else
	row_data = p;

      //Aqui guardamos el puntero al objeto en la aplicacion
      //representado por el texto en esta lista.
      gtk_list_store_set(store, &iter, 1, row_data, -1);      

      /*gtk_tree_model_get(model, &iter, 1, &kk, -1);
	std::cout << "List_manager::add_callback: row_data: " << lm->row_data
	<< " kk: " << kk << std::endl;*/

      gtk_tree_selection_select_iter(selection, &iter);
      update_interface();

      return true;
    }
  else
    return false;
}

void
List_manager::add_callback(GtkWidget* widget, gpointer data)
{
  List_manager* lm  = reinterpret_cast<List_manager*>(data);
  std::string   str = gtk_entry_get_text(GTK_ENTRY(lm->entry));

  if (lm->add_element(str))
    {
      // Y vaciamos el text_entry
      lm->empty_text_entry();
    }
  else
    {
      lm->show_add_warning();
    }
}

void
List_manager::clear()
{
  GtkTreeView*      treeview  = GTK_TREE_VIEW(the_list);
  GtkTreeModel*     model     = gtk_tree_view_get_model(treeview);

  gtk_list_store_clear(GTK_LIST_STORE(model));
  empty_text_entry();
}

void
List_manager::remove_callback(GtkWidget* widget, gpointer data)
{
  List_manager*     lm = reinterpret_cast<List_manager*>(data);
  GtkTreeIter       iter;
  GtkTreeView*      treeview  = GTK_TREE_VIEW(lm->the_list);
  GtkTreeModel*     model     = gtk_tree_view_get_model(treeview);
  GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);

  if (gtk_tree_selection_get_selected(selection, NULL, &iter))
    {
      /*gint i;
      GtkTreePath *path;

      path = gtk_tree_model_get_path(model, &iter);
      i = gtk_tree_path_get_indices(path)[0];*/

      // Guardamos el ultimo elemento que borrare de la lista.
      gtk_tree_model_get(model, &iter, 1, &lm->plir, -1);
      //std::cout << "LM remove_callback plir= " << lm->plir << std::endl;

      /* actualizo la lista borrandolo y guardo ptro. al siguente item
       * valido.*/
      if (gtk_list_store_remove(GTK_LIST_STORE(model), &iter))
	{
	  //std::cout << "RC tras store_remove\n";
	  gtk_tree_model_get(model, &iter, 1, &lm->plvi, -1);
	  lm->plis = lm->plvi;

	  /*std::cout << "LM remove_callback plir= " << lm->plir 
	    << " plvi= " << lm->plvi << std::endl;*/

	  lm->remove_item();

	  lm->update_interface();
	  gtk_tree_selection_select_iter(selection, &iter);
	}
      else
	{
	  lm->plvi = NULL; //No queda item valido
	  //std::cout << "Oops...no quedan items validos.\n";
	}
    }
}

void
List_manager::selection_changed_callback(GtkWidget* widget, gpointer data)
{
  List_manager*     lm = reinterpret_cast<List_manager*>(data);
  GtkTreeIter       iter;
  GtkTreeView*      treeview = GTK_TREE_VIEW(lm->the_list);
  GtkTreeModel*     model = gtk_tree_view_get_model(treeview);
  //GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);
  GtkTreeSelection* selection = GTK_TREE_SELECTION(widget);
  int               sr;
  char*             plst;

  sr = gtk_tree_selection_count_selected_rows(selection);

  //std::cout << "\tselection_changed_callback(" << sr << ")\n\n";

  if (sr > 0)
    {
      /*gint i;
      GtkTreePath *path;

      path = gtk_tree_model_get_path(model, &iter);
      i = gtk_tree_path_get_indices(path)[0];*/

      gtk_tree_selection_get_selected(selection, NULL, &iter);
      // Guardamos el ultimo elemento seleccionado en la lista.
      gtk_tree_model_get(model, &iter, 1, &lm->plis, -1);

      // Guardamos el ultimo texto seleccionado en la lista.
      gtk_tree_model_get(model, &iter, 0, &plst, -1);
      lm->lst = plst;
      //lm->pliv = lm->plis;

      lm->selection_changed();
    }
  else
    lm->plis = NULL;
  
  lm->update_interface();
}

void
List_manager::connect_callbacks()
{
  GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(the_list));

  g_return_if_fail(add_button != NULL);
  g_signal_connect(G_OBJECT(add_button), "clicked",
		    G_CALLBACK(add_callback), 
		    this);

  g_return_if_fail(remove_button != NULL);
  g_signal_connect(G_OBJECT(remove_button), "clicked",
		    G_CALLBACK(remove_callback), 
		    this);

  g_return_if_fail(selection != NULL);
  g_signal_connect(G_OBJECT(selection), "changed",
		   G_CALLBACK(selection_changed_callback), 
		   this);
}

GtkTreeModel*
List_manager::create_model(void)
{
  //gint i = 0;
  GtkListStore*      store;
  //GtkTreeIter iter;
  GtkCellRenderer*   renderer;
  GtkTreeViewColumn* column;
  GtkTreeSelection*  selection;

  //printf("Creamos el modelo.\n");

  /* create list store */
  store = gtk_list_store_new(titulo.size()+1, G_TYPE_STRING, G_TYPE_POINTER);

  // gtk_list_store_append(store, &iter);
  // gtk_list_store_set(store, &iter, 0, "Frase de prueba", -1);


  renderer = gtk_cell_renderer_text_new();
  for(unsigned int i = 0; i < titulo.size(); i++)
    {
      column = gtk_tree_view_column_new_with_attributes(titulo[i].c_str(),
							renderer, "text",
							i, NULL);
      gtk_tree_view_column_set_sort_column_id(column, i);
      gtk_tree_view_append_column(GTK_TREE_VIEW(the_list), column);
    }
  gtk_tree_view_set_search_column(GTK_TREE_VIEW(the_list), 0);

  selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(the_list));
  gtk_tree_selection_set_mode(selection, GTK_SELECTION_SINGLE);

  return GTK_TREE_MODEL(store);
}

bool
List_manager::are_items_selected()
{
  //GtkTreeIter iter;
  //GtkTreeModel* model = gtk_tree_view_get_model(treeview);
  GtkTreeView*      treeview  = GTK_TREE_VIEW(the_list);
  GtkTreeSelection* selection = gtk_tree_view_get_selection(treeview);
  int               si;

  //if (gtk_tree_selection_get_selected(selection, NULL, NULL))
  si = gtk_tree_selection_count_selected_rows(selection);

  //std::cout << "#si = " << si << std::endl;

  if ( si > 0 )
    {
      return true;
    }
  else
    {
      return false;
    }
}

void
List_manager::update_interface()
{
  if (are_items_selected())
    gtk_widget_set_sensitive(remove_button, true);
  else
    gtk_widget_set_sensitive(remove_button, false);
}

void
List_manager::catch_enter_key()
{
  if (entry)
    g_signal_connect (GTK_OBJECT(entry), "key_press_event",
		      GTK_SIGNAL_FUNC (enter_key_pressed), this);
}

bool 
List_manager::enter_key_pressed(GtkWidget* w, GdkEventKey* e, gpointer p)
{
  //Tagger* t = reinterpret_cast<Tagger*>(p);

  // Si es return...aceptamos
  if (e -> string[0] == static_cast<char>(GDK_Return))
    {
      add_callback(NULL, p);
      return true;
    }
  else
    return false;
}
