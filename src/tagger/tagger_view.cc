// -*- C++ -*-

/*
 * Creado:     Mon Dec 23 17:03:17 CET 2002
 * Modificado: Mon Dec 23 17:03:17 CET 2002
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 */

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <iostream>
#include <vector>
#include <string>
#include <gdk/gdkkeysyms.h>
#include <gui/acb_gui.h>
#include <sentence.h>
#include <tagger_view.h>
#include <tagger_model.h>
#include <text_item.h>
#include <tag_item.h>

using namespace acb_mvc;
using namespace acb_gui;

/*
 * Constructor por defecto.
 */
Tagger_view::Tagger_view(Tagger_model& m):
  View(m)
{
  std::vector<std::string> t(1);

  build();

  // Aseguramos q el modelo sabe quien esta vista.
  m.set_tagger_view(this);

  // El canvas donde se pinta la frase actual
  canvas = new Canvas(this, "drawingarea");
  g_return_if_fail ((sl != NULL) or (tl != NULL) or (canvas != NULL));

  // La pestaña con el gui de las frases
  t[0] = "Sentence";
  sl = new Sentence_list(the_widget, "sentence_page", t, this);
  // La pestaña con el gui de las etiquetas
  t[0] = "Tag";
  tl = new Tag_list(the_widget, "tag_page", t, this);

  // Y tambien en la lista de frases
  //sl->set_canvas(canvas);

  /*sl->change_mouse_shape(crArrow_width, crArrow_height, crArrow_m, crArrow_b);
  tl->change_mouse_shape(GDK_CROSSHAIR);*/

  /*tl->hide ();
    sl->hide ();*/

  //El widget para la barra de estado
  statusbar = lookup_widget("statusbar");

  //El selector de ficheros
  file_selector = create_file_selector();

  //El menu pop-up
  cpm = create_canvas_menu();

  //El nombre del documento inicial
  doc_name = _("Untitled");
}

/*
 * Destructor.
 */
Tagger_view::~Tagger_view()
{
  // Destruimos los subwidgets.
  delete sl;
  delete tl;
  delete canvas;
}

void
Tagger_view::add_sentence_to_list(Sentence* s)
{
  sl->add_sentence(s);
}

void
Tagger_view::add_tag_to_list(Tag_info* t)
{
  bool success;

  success = tl->add_tag(t);
  /*if (success)
    std::cout << "Añado el tag: " << t->get_tag() << std::endl;
  else
  std::cout << "No Añado el tag: " << t->get_tag() << std::endl;*/
}

Sentence*
Tagger_view::create_sentence(const std::string& s)
{
  Sentence* ns;

  //Pido al modelo q la cree
  ns = get_model()->add_sentence(s);

  //sl->add_sentence(ns);

  return ns;
}

void
Tagger_view::load_data_from_model()
{
  const std::list<Sentence*>* psl = get_model()->get_sentence_list();
  const std::list<Tag_info*>* ptl = get_model()->get_tag_list();
  Progress_dialog             pd(_("Loading sentences..."));

  pd.show();

  //1) FRASES
  for(std::list<Sentence*>::const_iterator i = psl->begin(); i != psl->end(); i++)
    {
      pd.pulse();
      add_sentence_to_list(*i);
      //std::cout << "Frase añadida\n";
    }

  //2) TAGS
  tl->set_reading_from_file(true);
  for(std::list<Tag_info*>::const_iterator i = ptl->begin(); i != ptl->end(); i++)
    {
      pd.pulse();
      add_tag_to_list(*i);
      //std::cout << "Tag añadida\n";
    }
  tl->set_reading_from_file(false);

  pd.hide();
}

void
Tagger_view::load_tags_from_model()
{
  const std::list<Tag_info*>* ptl = get_model()->get_tag_list();

  //2) TAGS
  tl->set_reading_from_file(true);
  for(std::list<Tag_info*>::const_iterator i = ptl->begin(); i != ptl->end(); i++)
    {
      add_tag_to_list(*i);
    }
  tl->set_reading_from_file(false);
}

/*
 * Update pedido por el modelo.
 */
void
Tagger_view::update()
{
  // El Canvas
  canvas->force_redraw();
  // La lista de frases y la de tags
  sl->update_interface();
  tl->update_interface();
}

/*
 * update_model, debe actualizar su modelo
 */
void
Tagger_view::update_model()
{
  // 1) actualizamos el modelo


  // 2) Implementacion por defecto
  View::update_model();
}

GtkWidget*
Tagger_view::create_file_selector(void)
{
  GtkWidget *file_selector;
  GtkWidget *ok_button;
  GtkWidget *cancel_button;

  file_selector = gtk_file_selection_new (_("Select a file:"));
  gtk_container_set_border_width (GTK_CONTAINER (file_selector), 5);
  gtk_window_set_modal (GTK_WINDOW (file_selector), TRUE);

  ok_button = GTK_FILE_SELECTION (file_selector)->ok_button;
  gtk_widget_show (ok_button);
  GTK_WIDGET_SET_FLAGS (ok_button, GTK_CAN_DEFAULT);

  cancel_button = GTK_FILE_SELECTION (file_selector)->cancel_button;
  gtk_widget_show (cancel_button);
  GTK_WIDGET_SET_FLAGS (cancel_button, GTK_CAN_DEFAULT);

  g_signal_connect ((gpointer) ok_button, "clicked",
                    G_CALLBACK (fselector_accept_cb),
                    this);
  g_signal_connect ((gpointer) cancel_button, "clicked",
                    G_CALLBACK (fselector_cancel_cb),
                    this);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (file_selector, file_selector, "file_selector");
  GLADE_HOOKUP_OBJECT_NO_REF (file_selector, ok_button, "ok_button");
  GLADE_HOOKUP_OBJECT_NO_REF (file_selector, cancel_button, "cancel_button");

  return file_selector;
}

GtkWidget*
Tagger_view::create_canvas_menu (void)
{
  GtkWidget *canvas_menu;
  GtkWidget *reorder_tree1;
  GtkWidget *separator10;
  GtkWidget *empty;
  GtkWidget *add_index;
  GtkWidget *delete_index;
  GtkWidget *separator11;
  GtkWidget *deadjoint;
  GtkWidget *delete1;
  GtkWidget *unparent;
  GtkWidget *separator8;
  GtkWidget *untriangle1;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  canvas_menu = gtk_menu_new ();
  gtk_container_set_border_width (GTK_CONTAINER (canvas_menu), 2);

  reorder_tree1 = gtk_menu_item_new_with_mnemonic (_("Reorder tree"));
  gtk_widget_show (reorder_tree1);
  gtk_container_add (GTK_CONTAINER (canvas_menu), reorder_tree1);
  gtk_tooltips_set_tip (tooltips, reorder_tree1, _("Tries to 'beautify' your tree."), NULL);

  separator10 = gtk_menu_item_new ();
  gtk_widget_show (separator10);
  gtk_container_add (GTK_CONTAINER (canvas_menu), separator10);
  gtk_widget_set_sensitive (separator10, FALSE);

  empty = gtk_menu_item_new_with_mnemonic (_("Add empty category"));
  gtk_widget_show (empty);
  gtk_container_add (GTK_CONTAINER (canvas_menu), empty);
  gtk_tooltips_set_tip (tooltips, empty, _("Adds an empty category node as a child."), NULL);

  add_index = gtk_menu_item_new_with_mnemonic (_("Add index"));
  gtk_widget_show (add_index);
  gtk_container_add (GTK_CONTAINER (canvas_menu), add_index);
  gtk_tooltips_set_tip (tooltips, add_index, _("Adds an index to this node."), NULL);

  delete_index = gtk_menu_item_new_with_mnemonic (_("Delete index"));
  gtk_widget_show (delete_index);
  gtk_container_add (GTK_CONTAINER (canvas_menu), delete_index);
  gtk_tooltips_set_tip (tooltips, delete_index, _("Deletes the index from this node."), NULL);

  separator11 = gtk_menu_item_new ();
  gtk_widget_show (separator11);
  gtk_container_add (GTK_CONTAINER (canvas_menu), separator11);
  gtk_widget_set_sensitive (separator11, FALSE);

  deadjoint = gtk_menu_item_new_with_mnemonic (_("Deadjoint"));
  gtk_widget_show (deadjoint);
  gtk_container_add (GTK_CONTAINER (canvas_menu), deadjoint);
  gtk_tooltips_set_tip (tooltips, deadjoint, _("Deadjoints this node's parent."), NULL);

  delete1 = gtk_menu_item_new_with_mnemonic (_("Delete"));
  gtk_widget_show (delete1);
  gtk_container_add (GTK_CONTAINER (canvas_menu), delete1);
  gtk_tooltips_set_tip (tooltips, delete1, _("Deletes the selected items if they have sons."), NULL);

  unparent = gtk_menu_item_new_with_mnemonic (_("Unparent"));
  gtk_widget_show (unparent);
  gtk_container_add (GTK_CONTAINER (canvas_menu), unparent);
  gtk_tooltips_set_tip (tooltips, unparent, _("The selected nodes will be orphaned."), NULL);

  separator8 = gtk_menu_item_new ();
  gtk_widget_show (separator8);
  gtk_container_add (GTK_CONTAINER (canvas_menu), separator8);
  gtk_widget_set_sensitive (separator8, FALSE);

  untriangle1 = gtk_menu_item_new_with_mnemonic (_("Untriangle"));
  gtk_widget_show (untriangle1);
  gtk_container_add (GTK_CONTAINER (canvas_menu), untriangle1);

  g_signal_connect ((gpointer) reorder_tree1, "activate",
                    G_CALLBACK (reorder_tree_option),
                    this);
  g_signal_connect ((gpointer) empty, "activate",
                    G_CALLBACK (empty_category_node_option),
                    this);
  g_signal_connect ((gpointer) add_index, "activate",
                    G_CALLBACK (add_index_option),
                    this);
  g_signal_connect ((gpointer) delete_index, "activate",
                    G_CALLBACK (delete_index_option),
                    this);
  g_signal_connect ((gpointer) deadjoint, "activate",
                    G_CALLBACK (deadjoint_node_option),
                    this);
  g_signal_connect ((gpointer) delete1, "activate",
                    G_CALLBACK (delete_node_option),
                    this);
  g_signal_connect ((gpointer) unparent, "activate",
                    G_CALLBACK (unparent_node_option),
                    this);
  g_signal_connect ((gpointer) untriangle1, "activate",
                    G_CALLBACK (untriangle_node_option),
                    this);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (canvas_menu, canvas_menu, "canvas_menu");
  GLADE_HOOKUP_OBJECT (canvas_menu, reorder_tree1, "reorder_tree1");
  GLADE_HOOKUP_OBJECT (canvas_menu, separator10, "separator10");
  GLADE_HOOKUP_OBJECT (canvas_menu, empty, "empty");
  GLADE_HOOKUP_OBJECT (canvas_menu, add_index, "add_index");
  GLADE_HOOKUP_OBJECT (canvas_menu, delete_index, "delete_index");
  GLADE_HOOKUP_OBJECT (canvas_menu, separator11, "separator11");
  GLADE_HOOKUP_OBJECT (canvas_menu, deadjoint, "deadjoint");
  GLADE_HOOKUP_OBJECT (canvas_menu, delete1, "delete1");
  GLADE_HOOKUP_OBJECT (canvas_menu, unparent, "unparent");
  GLADE_HOOKUP_OBJECT (canvas_menu, separator8, "separator8");
  GLADE_HOOKUP_OBJECT (canvas_menu, untriangle1, "untriangle1");
  GLADE_HOOKUP_OBJECT_NO_REF (canvas_menu, tooltips, "tooltips");

  return canvas_menu;
}

void
Tagger_view::select_file_name(const std::string& title) const
{ 
  if (file_selector)
    {
      gtk_window_set_title(GTK_WINDOW(file_selector), title.c_str());
      gtk_widget_show(file_selector);
      gtk_main();
    }
}

void
Tagger_view::fselector_accept_cb(GtkWidget* widget, gpointer user_data)
{
  Tagger_view* t = reinterpret_cast<Tagger_view*>(user_data);

  t->file_selected = gtk_file_selection_get_filename (GTK_FILE_SELECTION(t->file_selector));

  gtk_widget_hide(t->file_selector);
  gtk_main_quit();
}

void
Tagger_view::fselector_cancel_cb(GtkWidget* widget, gpointer user_data)
{
  Tagger_view* t = reinterpret_cast<Tagger_view*>(user_data);

  t->file_selected = "";
  gtk_widget_hide(t->file_selector);
  gtk_main_quit();
}

void
Tagger_view::open_option(GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*           t = reinterpret_cast<Tagger_view*>(user_data);
  Tagger_model*          m = t->get_model();
  acb_gui::Yes_no_dialog d(_("Please answer:"), 
			   _("Opening a new document will erase the current one.\nContinue?"));

  t->select_file_name(_("Select a file to Open:"));
  if (t->file_selected.length() > 0)
    {
      d.set_transient_for(t);
      d.show ();
      if (d.is_accepted ())
	{
	  t->new_document();
	  if (m->load_document_xml(t->file_selected))
	    {
	      t->doc_name = t->file_selected;
	      t->show_statusbar_message(_("File loaded."));
	      t->set_title(t->doc_name);

	      t->load_data_from_model();
	      t->set_canvas_sentence(m->get_last_read_sentence());
	    }
	  else 
	    {
	      acb_gui::Info_dialog d(_("Warning:"), _("This is not an arboratrix file."));
	      d.show();
	      t->doc_name = _("Untitled");
	      t->set_title(_("Untitled"));
	    }
	}
    }
}

void
Tagger_view::clean_views()
{
  //tambien el canvas
  canvas->empty();
  //Y las listas de tags y frases
  sl->clear();
  tl->clear();
}

void
Tagger_view::new_document()
{
  //Vaciamos el modelo
  get_model()->new_document();
  //Y las vistas
  clean_views();

  doc_name = _("Untitled");
  set_title(_("Untitled"));
}
 
void
Tagger_view::close_option (GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*  t = reinterpret_cast<Tagger_view*>(user_data);
  acb_gui::Yes_no_dialog d(_("Please answer:"), 
			   _("If you close the current document you'll lose"
			     " unsaved changes.\nContinue?"));

  d.set_transient_for(t);
  d.show ();
  if (d.is_accepted ())
    {
      t->new_document();
    }
}

void
Tagger_view::ensure_file_extension(std::string& fn, const std::string& ext)
{
  int         pos;
  std::string dn;
  std::string ofn;

  //Separo nombre de directorio
  pos = fn.find_last_of('/');
  if (pos > 0)
    {
      dn = fn.substr(0, pos);
      ofn = fn.substr(pos+1);
    }
  else
    ofn = fn;

  //std::cout << "Directorio original [" << dn << "]\n";
  //std::cout << "Fichero original [" << ofn << "]\n";

  if (ofn.length() < 4)
    {
      ofn += '.';
      ofn += ext;
    }
  else
    {
      /*if (ofn.compare(ofn.length()-4, 4, ".xml") != 0)
	{
	  ofn += '.';
	  ofn += ext;
	  }*/
      pos = ofn.find_last_of('.');
      if (pos > 0)
	ofn.erase(pos);
      ofn += '.';
      ofn += ext;
    }

  fn = dn + "/" + ofn;
  //fn = ofn;
  //std::cout << "El nombre es " << fn << std::endl;
}

void
Tagger_view::save_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view*  t = reinterpret_cast<Tagger_view*>(user_data);
  Tagger_model* m = t->get_model();
  std::string   fne;

  if (t->doc_name == _("Untitled"))
    {
      t->select_file_name(_("Select a file to Save:"));

      if (t->file_selected.length() > 0)
	{
	  /*ofs.open(t->file_selected.c_str());
	    t->save_document_xml(ofs);
	    ofs.close();*/

	  fne = t->file_selected;
	  t->ensure_file_extension(fne, "xml");

	  m->save_document_xml(fne);

	  t->doc_name = fne;
	  t->show_statusbar_message(_("File saved."));
	  t->set_title(t->doc_name);
	}
    }
  else
    {
      m->save_document_xml(t->doc_name);
      t->show_statusbar_message(_("File saved."));
    }
}

void
Tagger_view::save_as_option (GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*  t = reinterpret_cast<Tagger_view*>(user_data);
  Tagger_model* m = t->get_model();

  t->select_file_name(_("Select a file to Save:"));
  if (t->file_selected.length() > 0)
    {
      /*ofs.open(t->file_selected.c_str());
      t->save_document_xml(ofs);
      ofs.close();*/
      m->save_document_xml(t->file_selected);
      t->show_statusbar_message(_("File saved."));
    }
}

void
Tagger_view::import_sentences_option(GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*  t = reinterpret_cast<Tagger_view*>(user_data);
  Tagger_model* m = t->get_model();

  t->select_file_name(_("Select a file to import sentences from:"));
  if (t->file_selected.length() > 0)
    {
      if (m->import_sentences(t->file_selected))
	{
	  t->clean_views();
	  t->load_data_from_model();
	}
    }
}

void
Tagger_view::import_tags_option(GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*  t = reinterpret_cast<Tagger_view*>(user_data);
  Tagger_model* m = t->get_model();

  t->select_file_name(_("Select a file to import tags from:"));
  if (t->file_selected.length() > 0)
    {
      if (m->import_tags(t->file_selected))
	{
	  t->tl->clear();
	  t->load_tags_from_model();
	}
    }
}

void
Tagger_view::export_tags_option(GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*  t = reinterpret_cast<Tagger_view*>(user_data);
  Tagger_model* m = t->get_model();
  bool          result = false;

  t->select_file_name(_("Select a file to import tags from:"));
  if (t->file_selected.length() > 0)
    {
      result = m->export_tags(t->file_selected);
    }
}

void
Tagger_view::export_qtree_option(GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*  t = reinterpret_cast<Tagger_view*>(user_data);
  Tagger_model* m = t->get_model();
  std::ofstream ofs;

  t->select_file_name(_("Select a file to Export:"));

  if (t->file_selected.length() > 0)
    {
      ofs.open(t->file_selected.c_str());
      m->save_document_qtree(ofs);
      ofs.close();

      t->show_statusbar_message(_("File exported."));
    }
}

void
Tagger_view::exit_option (GtkMenuItem* menuitem, gpointer user_data)
{
  //Tagger_view* t = reinterpret_cast<Tagger_view*>(user_data);
  acb_gui::Yes_no_dialog d(_("Please answer:"),
			   _("Do you want to leave Arboratrix?"));

  d.show();
  if (d.is_accepted())
    gtk_main_quit();
}

void
Tagger_view::about_option (GtkMenuItem *menuitem, 
			  gpointer user_data)
{
  std::string           title = "About ";
  std::string           m;
  acb_gui::Info_dialog* d;
  
  m += _("Version: ");
  m += VERSION;
  m += "\n";
  m += _("Compiled on: ");
  m += DATE;
  m += "\n";
  m += _("(C) Antonio Corbí: 2001, 2002, 2003");

  title += PACKAGE;
  d = new Info_dialog(_(title.c_str()), m);
  d->show();
  delete d;
}

gint
Tagger_view::statusbar_message_remover(gpointer data)
{
  Tagger_view* tv = reinterpret_cast<Tagger_view*>(data);

  gtk_statusbar_pop(GTK_STATUSBAR(tv->statusbar), 1);
  return false;
}

void
Tagger_view::show_statusbar_message(const std::string& message, int tout)
{
  gtk_statusbar_push(GTK_STATUSBAR(statusbar), 1, message.c_str());
  gtk_timeout_add(tout, statusbar_message_remover, this);
}

void
Tagger_view::select_all_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view* t = reinterpret_cast<Tagger_view*>(user_data);

  t->canvas->select_all_items();
}

void
Tagger_view::deselect_all_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view* t = reinterpret_cast<Tagger_view*>(user_data);

  t->canvas->deselect_all_items();
}

void
Tagger_view::invert_selection_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view* t = reinterpret_cast<Tagger_view*>(user_data);

  t->canvas->invert_selection();
}

void
Tagger_view::reorder_tree_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view* t = reinterpret_cast<Tagger_view*>(user_data);

  t->canvas->layout_tree();
}

void 
Tagger_view::cm_enable_items()
{
  GtkWidget*        w;
  Canvas::Itemlist& tl = canvas->get_selected_items();
  char*             item[] = {"reorder_tree1", "deadjoint", "delete1",
			      "unparent", "untriangle1", "empty", 
			      "add_index", "delete_index", NULL};
  bool              enable_delete       = false;
  bool              enable_unparent     = false;
  bool              enable_untriangle   = false;
  bool              enable_deadjoint    = false;
  bool              enable_empty        = false;
  bool              enable_add_index    = false;
  bool              enable_delete_index = false;
  Text_model*       ri = canvas->get_sentence()->get_root();
  Text_model*       ap;
  Text_model*       as;
  Text_model*       si;
  Text_item*        txi;
  Tag_item*         tgi;

  //1: Todos los items
  for (int iw = 0; item[iw] != NULL; iw++)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), item[iw]));
      gtk_widget_set_sensitive(w, true);
    }

  //---------------------------------------------------
  /*2a: Caso particular: 'delete' solo tiene sentido si hay un solo item
   *   y este es 'borrable'.*/
  if (tl.size() == 1)
    {
      enable_delete = tl.front()->is_erasable();
    }
  if (not enable_delete)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), "delete1"));
      gtk_widget_set_sensitive(w, false);
    }

  /*2b: Caso particular: 'deadjoint' solo tiene sentido si hay un solo item
   *   y su padre es 'adjunto'.*/
  if (tl.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(tl.front());
      si = txi->get_model();
      ap = si->get_parent();
      if (ap->is_adjoint())
	{
	  as = ap->get_adjoint_son();
	  //El nodo adjunto no puede ser el q esta seleccionado ahora.
	  enable_deadjoint = (as != si);
	}
    }
  if (not enable_deadjoint)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), "deadjoint"));
      gtk_widget_set_sensitive(w, false);
    }


  /*2c: Caso particular: 'unparent' solo tiene sentido si algun item
   *   es hijo de un nodo que no sea ROOT y no es una categoria
   *   nula.*/
  for(std::list<Canvas_item*>::iterator i = tl.begin(); 
      ((i != tl.end()) and (not enable_unparent)); i++)
    {
      txi = dynamic_cast<Text_item*>(*i);
      ap = txi->get_model()->get_parent();

      enable_unparent = (ap != ri) and (not  txi->is_empty_category());
    }
  if (not enable_unparent)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), "unparent"));
      gtk_widget_set_sensitive(w, false);
    }


  /*2d: Caso particular: 'empty' solo tiene sentido si hay un solo item
   *   y este es puede tener hijos y no tiene.*/
  if (tl.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(tl.front());

      if (txi)
	{
	  ap = txi->get_model();
	  enable_empty = ((txi->can_have_sons()) and
			  (ap->get_num_children() == 0));
	}
    }
  if (not enable_empty)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), "empty"));
      gtk_widget_set_sensitive(w, false);
    }

  //---------------------------------------------------

  /*3: Caso particular: 'untriangle' solo tiene sentido si algun item
   *   es triangulo.*/
  for(std::list<Canvas_item*>::iterator i = tl.begin(); 
      ((i != tl.end()) and (not enable_untriangle)); i++)
    {
      txi = dynamic_cast<Text_item*>(*i);
      if (txi->can_have_sons()) //puede ser triangulo
	{
	  tgi = dynamic_cast<Tag_item*>(*i);
	  if (tgi and tgi->is_triangle())
	    enable_untriangle = true;
	}
    }
  if (not enable_untriangle)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), "untriangle1"));
      gtk_widget_set_sensitive(w, false);
    }


  //---------------------------------------------------
  //4: Hacer bonito el arbol en pantalla.
  if (canvas->get_sentence())
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), 
					  "reorder_tree1"));
      gtk_widget_set_sensitive(w, true);
    }
  //---------------------------------------------------

  //---------------------------------------------------
  /*5a: Caso particular: 'add_index' solo tiene sentido si hay un solo
   *   item y este no tiene indice y no es categoria nula.*/
  if (tl.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(tl.front());
      enable_add_index = ((not txi->has_index()) and 
			  (not txi->is_empty_category()));
    }
  if (not enable_add_index)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), "add_index"));
      gtk_widget_set_sensitive(w, false);
    }
  /*5b: Caso particular: 'delete_index' solo tiene sentido si hay un
   *   solo item y este tiene indice.*/
  if (tl.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(tl.front());
      enable_delete_index = txi->has_index();
    }
  if (not enable_delete_index)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), "delete_index"));
      gtk_widget_set_sensitive(w, false);
    }
}

void
Tagger_view::cm_disable_items()
{
  GtkWidget* w;
  char*      item[] = {"reorder_tree1", "deadjoint","delete1", "unparent",
		       "untriangle1", "empty", "add_index", "delete_index",
		       NULL};

  for (int i = 0; item[i] != NULL; i++)
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm), item[i]));
      gtk_widget_set_sensitive(w, false);
    }

  if (canvas->get_sentence())
    {
      w = GTK_WIDGET(gtk_object_get_data (GTK_OBJECT (cpm),
					  "reorder_tree1"));
      gtk_widget_set_sensitive(w, true);
    }

}

void
Tagger_view::delete_node_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view*               t   = reinterpret_cast<Tagger_view*>(user_data);
  Canvas::Itemlist&          csi = t->canvas->get_selected_items();
  //Text_model*                root_node = t->canvas->get_sentence()->get_root();
  Text_model*                m;
  Text_item*                 ti;
  Text_model*                np; //new parent
  Canvas::Itemlist::iterator i;
  //std::list<Text_model*>*    cl; //children list
  std::list<Text_model*>     tl; //temp children list
  //std::list<Text_model*>::iterator j;

  //t->canvas->invert_selection();
  //std::cout << "delete_node_option\n";
  for(i = csi.begin(); i != csi.end(); i++)
    {
      if ((*i)->is_erasable())
	{
	  ti = dynamic_cast<Text_item*>(*i);
	  //Item a eliminar
	  m = ti->get_model();
	  //std::cout << "Borramos: " << *m << std::endl;

	  //Su padre actual
	  np = m->get_parent();
	  //std::cout << "Su padre es: " << *np << std::endl;

	  // Lo quito de la lista de hijos del padre y este adopta
	  // a sus nietos
	  m->remove_from_parent_and_adopt();

	  //Elimino la vista de la lista de items de la frase
	  t->canvas->get_sentence()->remove_item(dynamic_cast<Text_item*>(*i));

	  //Y, finalmente, borro de la memoria...el modelo
	  delete m;
	  //...y la vista sobre el
	  delete *i;
	}
    }
  t->canvas->layout_tree();
}

void
Tagger_view::unparent_node_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view*               t   = reinterpret_cast<Tagger_view*>(user_data);
  Canvas::Itemlist&          csi = t->canvas->get_selected_items();
  Text_model*                root_node = t->canvas->get_sentence()->get_root();
  Text_model*                m;
  Canvas::Itemlist::iterator i;

  //std::cout << "unparent_node_option\n";
  for( i = csi.begin(); i != csi.end(); i++)
    {
      m = (dynamic_cast<Text_item*>(*i))->get_model();
      root_node->add_child(m);
    }
  t->canvas->layout_tree();
}

void
Tagger_view::untriangle_node_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view*               t   = reinterpret_cast<Tagger_view*>(user_data);
  Canvas::Itemlist&          csi = t->canvas->get_selected_items();
  //Text_model*                root_node = t->canvas->get_sentence()->get_root();
  Tag_item*                  tgi;
  Text_item*                 txi;
  Canvas::Itemlist::iterator i;

  for( i = csi.begin(); i != csi.end(); i++)
    {
      txi = dynamic_cast<Text_item*>(*i);
      if (txi->can_have_sons()) //puede ser triangulo
	{
	  tgi = dynamic_cast<Tag_item*>(*i);
	  if (tgi and tgi->is_triangle())
	    tgi->set_triangle(false);
	}
    }

  t->canvas->layout_tree();
}

void
Tagger_view::deadjoint_node_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view*      t   = reinterpret_cast<Tagger_view*>(user_data);
  Canvas::Itemlist& csi = t->canvas->get_selected_items();
  Text_model*       root_node = t->canvas->get_sentence()->get_root();
  Text_model*       m;
  Text_model*       parent;
  Text_model*       as;
  Text_item*        txi;

  if (csi.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(*csi.begin());
      m = txi->get_model();
      parent = m->get_parent();

      /*Evito el siguiente 'if...' pq se comprueba al mostrar el
	menu-popup del canvas.*/

      //if (parent->is_adjoint())
      //{
      //deadjoint...
      as = parent->get_adjoint_son();
      as->remove_from_parent_and_adopt();

      //Elimino la vista del nodo-adjunto de la frase y de la
      //memoria
      txi = as->get_first_view();
      t->canvas->get_sentence()->remove_item(txi);
      delete txi;

      //unparent...
      root_node->add_child(m);

      //Redibujamos el arbol
      t->canvas->layout_tree();
      //}
    }
}

void
Tagger_view::empty_category_node_option(GtkMenuItem* menuitem, gpointer user_data)
{
  Tagger_view*      t   = reinterpret_cast<Tagger_view*>(user_data);
  Canvas::Itemlist& csi = t->canvas->get_selected_items();
  Sentence*         cs = t->canvas->get_sentence();
  //Text_model*       root_node = cs->get_root();
  Text_model*       m;
  Text_item*        txi;

  if (csi.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(*csi.begin());
      m = txi->get_model();

      cs->add_text_item(Text_item::get_empty_set(), m);
      t->canvas->layout_tree();
    }
}

void
Tagger_view::add_index_option(GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*      t   = reinterpret_cast<Tagger_view*>(user_data);
  Canvas::Itemlist& csi = t->canvas->get_selected_items();
  //Sentence*         cs = t->canvas->get_sentence();
  Text_model*       m;
  Text_item*        txi;
  Index_dialog      id;
  std::string       str_idx;

  if (csi.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(*csi.begin());
      m = txi->get_model();

      id.show();
      if (id.is_accepted())
	{
	  str_idx = id.get_index();
	  m->set_index(str_idx);
	  t->canvas->layout_tree();
	}
    }
}

void
Tagger_view::delete_index_option(GtkMenuItem *menuitem, gpointer user_data)
{
  Tagger_view*      t   = reinterpret_cast<Tagger_view*>(user_data);
  Canvas::Itemlist& csi = t->canvas->get_selected_items();
  //Sentence*         cs = t->canvas->get_sentence();
  Text_model*       m;
  Text_item*        txi;

  if (csi.size() == 1)
    {
      txi = dynamic_cast<Text_item*>(*csi.begin());
      m = txi->get_model();
      if (txi->has_index())
	m->set_index("");
    }
}

void
Tagger_view::show_popup_menu(GdkEventButton* event)
{
  gtk_menu_popup (GTK_MENU(cpm), 
		  NULL, NULL, NULL, NULL, 
		  event->button,
		  gdk_event_get_time(reinterpret_cast<GdkEvent*>(event)));
}

/*---------------------------------------------------------------------------------*/
GtkWidget*
Tagger_view::create_gui(void)
{
  GtkWidget *tagger;
  GtkWidget *vbox1;
  GtkWidget *menubar;
  GtkWidget *fichero1;
  GtkWidget *fichero1_menu;
  GtkWidget *open2;
  GtkWidget *close1;
  GtkWidget *separator6;
  GtkWidget *save2;
  GtkWidget *save_as2;
  GtkWidget *separator3;
  GtkWidget *importar_frases;
  GtkWidget *import_tags1;
  GtkWidget *export_tags1;
  GtkWidget *separator7;
  GtkWidget *export_qtree1;
  GtkWidget *export_postscript1;
  GtkWidget *export_pdf1;
  GtkWidget *separator1;
  GtkWidget *exit2;
  GtkWidget *edicion1;
  GtkWidget *edicion1_menu;
  GtkWidget *seleccionar_todo;
  GtkWidget *deseleccionar_todo1;
  GtkWidget *invertir_seleccion1;
  GtkWidget *help1;
  GtkWidget *help1_menu;
  GtkWidget *acerca_de_arboratrix1;
  GtkWidget *hbox4;
  GtkWidget *scrolledwindow1;
  GtkWidget *viewport1;
  GtkWidget *drawingarea;
  GtkWidget *vseparator2;
  GtkWidget *notebook1;
  GtkWidget *sentence_page;
  GtkWidget *scrolledwindow2;
  GtkWidget *sentence_list;
  GtkWidget *frame1;
  GtkWidget *table1;
  GtkWidget *sentence_delete_button;
  GtkWidget *sentence;
  GtkWidget *sentence_add_button;
  GtkWidget *finish_button;
  GtkWidget *label7;
  GtkWidget *label5;
  GtkWidget *tag_page;
  GtkWidget *scrolledwindow3;
  GtkWidget *tag_list;
  GtkWidget *frame2;
  GtkWidget *table2;
  GtkWidget *tag;
  GtkWidget *tag_add_button;
  GtkWidget *tag_delete_button;
  GtkWidget *label8;
  GtkWidget *label6;
  GtkWidget *statusbar;
  GtkAccelGroup *accel_group;
  GtkTooltips *tooltips;

  tooltips = gtk_tooltips_new ();

  accel_group = gtk_accel_group_new ();

  tagger = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (tagger), 2);
  gtk_window_set_title (GTK_WINDOW (tagger), _("Syntactic Tagger"));
  gtk_window_set_default_size (GTK_WINDOW (tagger), 700, 400);

  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (tagger), vbox1);

  menubar = gtk_menu_bar_new ();
  gtk_widget_show (menubar);
  gtk_box_pack_start (GTK_BOX (vbox1), menubar, FALSE, FALSE, 0);

  fichero1 = gtk_menu_item_new_with_mnemonic (_("_File"));
  gtk_widget_show (fichero1);
  gtk_container_add (GTK_CONTAINER (menubar), fichero1);
  gtk_tooltips_set_tip (tooltips, fichero1, _("Main menu."), NULL);

  fichero1_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (fichero1), fichero1_menu);

  open2 = gtk_menu_item_new_with_mnemonic (_("_Open..."));
  gtk_widget_show (open2);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), open2);
  gtk_widget_add_accelerator (open2, "activate", accel_group,
                              GDK_O, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  close1 = gtk_menu_item_new_with_mnemonic (_("_Close"));
  gtk_widget_show (close1);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), close1);
  gtk_tooltips_set_tip (tooltips, close1, _("Closes the current document."), NULL);
  gtk_widget_add_accelerator (close1, "activate", accel_group,
                              GDK_W, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  separator6 = gtk_menu_item_new ();
  gtk_widget_show (separator6);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), separator6);
  gtk_widget_set_sensitive (separator6, FALSE);

  save2 = gtk_menu_item_new_with_mnemonic (_("_Save"));
  gtk_widget_show (save2);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), save2);
  gtk_widget_add_accelerator (save2, "activate", accel_group,
                              GDK_S, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  save_as2 = gtk_menu_item_new_with_mnemonic (_("Save _As"));
  gtk_widget_show (save_as2);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), save_as2);
  gtk_widget_add_accelerator (save_as2, "activate", accel_group,
                              GDK_G, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  separator3 = gtk_menu_item_new ();
  gtk_widget_show (separator3);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), separator3);
  gtk_widget_set_sensitive (separator3, FALSE);

  export_qtree1 = gtk_menu_item_new_with_mnemonic (_("_Export qtree"));
  gtk_widget_show (export_qtree1);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), export_qtree1);
  gtk_tooltips_set_tip (tooltips, export_qtree1, _("Exports to a file in 'qtree' format."), NULL);
  gtk_widget_add_accelerator (export_qtree1, "activate", accel_group,
                              GDK_E, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  export_postscript1 = gtk_menu_item_new_with_mnemonic (_("Export _postscript"));
  gtk_widget_show (export_postscript1);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), export_postscript1);
  gtk_tooltips_set_tip (tooltips, export_postscript1, _("Exports to a file in 'PS' format ready to print. You need a LaTeX environment and the Qtree package."), NULL);
  gtk_widget_add_accelerator (export_postscript1, "activate", accel_group,
                              GDK_P, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  export_pdf1 = gtk_menu_item_new_with_mnemonic (_("Export p_df"));
  gtk_widget_show (export_pdf1);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), export_pdf1);
  gtk_tooltips_set_tip (tooltips, export_pdf1, _("Exports to a file in PDF format."), NULL);
  gtk_widget_add_accelerator (export_pdf1, "activate", accel_group,
                              GDK_D, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  separator7 = gtk_menu_item_new ();
  gtk_widget_show (separator7);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), separator7);
  gtk_widget_set_sensitive (separator7, FALSE);

  importar_frases = gtk_menu_item_new_with_mnemonic (_("_Import sentences"));
  gtk_widget_show (importar_frases);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), importar_frases);
  gtk_tooltips_set_tip (tooltips, importar_frases, _("Imports sentences from plain ascii file."), NULL);
  gtk_widget_add_accelerator (importar_frases, "activate", accel_group,
                              GDK_I, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  import_tags1 = gtk_menu_item_new_with_mnemonic (_("Import _tags"));
  gtk_widget_show (import_tags1);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), import_tags1);
  gtk_tooltips_set_tip (tooltips, import_tags1, _("Imports tags from plain ascii file."), NULL);
  gtk_widget_add_accelerator (import_tags1, "activate", accel_group,
                              GDK_T, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);


  export_tags1 = gtk_menu_item_new_with_mnemonic (_("export tags"));
  gtk_widget_show (export_tags1);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), export_tags1);
  gtk_tooltips_set_tip (tooltips, export_tags1, _("Exports tags to a plain ascii file."), NULL);

  separator1 = gtk_menu_item_new ();
  gtk_widget_show (separator1);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), separator1);
  gtk_widget_set_sensitive (separator1, FALSE);

  exit2 = gtk_menu_item_new_with_mnemonic (_("_Quit"));
  gtk_widget_show (exit2);
  gtk_container_add (GTK_CONTAINER (fichero1_menu), exit2);
  gtk_widget_add_accelerator (exit2, "activate", accel_group,
                              GDK_Q, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  edicion1 = gtk_menu_item_new_with_mnemonic (_("_Edit"));
  gtk_widget_show (edicion1);
  gtk_container_add (GTK_CONTAINER (menubar), edicion1);

  edicion1_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (edicion1), edicion1_menu);

  seleccionar_todo = gtk_menu_item_new_with_mnemonic (_("Select All"));
  gtk_widget_show (seleccionar_todo);
  gtk_container_add (GTK_CONTAINER (edicion1_menu), seleccionar_todo);
  gtk_widget_add_accelerator (seleccionar_todo, "activate", accel_group,
                              GDK_A, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  deseleccionar_todo1 = gtk_menu_item_new_with_mnemonic (_("Deselect All"));
  gtk_widget_show (deseleccionar_todo1);
  gtk_container_add (GTK_CONTAINER (edicion1_menu), deseleccionar_todo1);
  gtk_widget_add_accelerator (deseleccionar_todo1, "activate", accel_group,
                              GDK_W, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  invertir_seleccion1 = gtk_menu_item_new_with_mnemonic (_("Invert Selection"));
  gtk_widget_show (invertir_seleccion1);
  gtk_container_add (GTK_CONTAINER (edicion1_menu), invertir_seleccion1);
  gtk_widget_add_accelerator (invertir_seleccion1, "activate", accel_group,
                              GDK_R, GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);

  help1 = gtk_menu_item_new_with_mnemonic (_("_Help"));
  gtk_widget_show (help1);
  gtk_container_add (GTK_CONTAINER (menubar), help1);

  help1_menu = gtk_menu_new ();
  gtk_menu_item_set_submenu (GTK_MENU_ITEM (help1), help1_menu);

  acerca_de_arboratrix1 = gtk_menu_item_new_with_mnemonic (_("About Arboratrix"));
  gtk_widget_show (acerca_de_arboratrix1);
  gtk_container_add (GTK_CONTAINER (help1_menu), acerca_de_arboratrix1);

  hbox4 = gtk_hbox_new (FALSE, 0);
  gtk_widget_show (hbox4);
  gtk_box_pack_start (GTK_BOX (vbox1), hbox4, TRUE, TRUE, 0);

  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (hbox4), scrolledwindow1, TRUE, TRUE, 0);
  gtk_widget_set_size_request (scrolledwindow1, 300, 100);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow1), 2);
  GTK_WIDGET_UNSET_FLAGS (scrolledwindow1, GTK_CAN_FOCUS);

  viewport1 = gtk_viewport_new (NULL, NULL);
  gtk_widget_show (viewport1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), viewport1);
  gtk_viewport_set_shadow_type (GTK_VIEWPORT (viewport1), GTK_SHADOW_ETCHED_IN);

  drawingarea = gtk_drawing_area_new ();
  gtk_widget_show (drawingarea);
  gtk_container_add (GTK_CONTAINER (viewport1), drawingarea);
  gtk_widget_set_size_request (drawingarea, 1280, 1024);
  GTK_WIDGET_SET_FLAGS (drawingarea, GTK_CAN_FOCUS);
  gtk_widget_set_events (drawingarea, GDK_POINTER_MOTION_HINT_MASK | GDK_BUTTON1_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_KEY_PRESS_MASK | GDK_ENTER_NOTIFY_MASK);

  vseparator2 = gtk_vseparator_new ();
  gtk_widget_show (vseparator2);
  gtk_box_pack_start (GTK_BOX (hbox4), vseparator2, FALSE, TRUE, 0);

  notebook1 = gtk_notebook_new ();
  gtk_widget_show (notebook1);
  gtk_box_pack_start (GTK_BOX (hbox4), notebook1, FALSE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (notebook1), 2);

  sentence_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (sentence_page);
  gtk_container_add (GTK_CONTAINER (notebook1), sentence_page);
  gtk_container_set_border_width (GTK_CONTAINER (sentence_page), 1);

  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow2);
  gtk_box_pack_start (GTK_BOX (sentence_page), scrolledwindow2, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow2), 1);

  sentence_list = gtk_tree_view_new ();
  gtk_widget_show (sentence_list);
  gtk_container_add (GTK_CONTAINER (scrolledwindow2), sentence_list);
  gtk_tooltips_set_tip (tooltips, sentence_list, _("List of all sentences."), NULL);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (sentence_list), TRUE);
  gtk_tree_view_set_reorderable (GTK_TREE_VIEW (sentence_list), TRUE);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (sentence_page), frame1, FALSE, FALSE, 0);

  table1 = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (table1);
  gtk_container_add (GTK_CONTAINER (frame1), table1);
  gtk_container_set_border_width (GTK_CONTAINER (table1), 2);

  sentence_delete_button = gtk_button_new_from_stock ("gtk-remove");
  gtk_widget_show (sentence_delete_button);
  gtk_table_attach (GTK_TABLE (table1), sentence_delete_button, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (sentence_delete_button), 2);
  gtk_tooltips_set_tip (tooltips, sentence_delete_button, _("Deletes the current sentence from the list."), NULL);

  sentence = gtk_entry_new ();
  gtk_widget_show (sentence);
  gtk_table_attach (GTK_TABLE (table1), sentence, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, sentence, _("New sentence."), NULL);

  sentence_add_button = gtk_button_new_from_stock ("gtk-add");
  gtk_widget_show (sentence_add_button);
  gtk_table_attach (GTK_TABLE (table1), sentence_add_button, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (sentence_add_button), 2);
  gtk_tooltips_set_tip (tooltips, sentence_add_button, _("Adds the sentence to the left to the list."), NULL);

  finish_button = gtk_check_button_new_with_mnemonic (_("Finished"));
  gtk_widget_show (finish_button);
  gtk_table_attach (GTK_TABLE (table1), finish_button, 0, 1, 0, 1,
                    (GtkAttachOptions) (0),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (finish_button), 2);
  gtk_tooltips_set_tip (tooltips, finish_button, _("Marks the current sentence as finished."), NULL);

  label7 = gtk_label_new (_("<b>Possible actions</b>"));
  gtk_widget_show (label7);
  gtk_label_set_use_markup (GTK_LABEL (label7), TRUE);
  gtk_frame_set_label_widget (GTK_FRAME (frame1), label7);
  gtk_label_set_justify (GTK_LABEL (label7), GTK_JUSTIFY_LEFT);

  label5 = gtk_label_new (_("<b>Sentences</b>"));
  gtk_widget_show (label5);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 0), label5);
  gtk_label_set_use_markup (GTK_LABEL (label5), TRUE);
  gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);

  tag_page = gtk_vbox_new (FALSE, 0);
  gtk_widget_show (tag_page);
  gtk_container_add (GTK_CONTAINER (notebook1), tag_page);
  gtk_container_set_border_width (GTK_CONTAINER (tag_page), 1);

  scrolledwindow3 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_show (scrolledwindow3);
  gtk_box_pack_start (GTK_BOX (tag_page), scrolledwindow3, TRUE, TRUE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (scrolledwindow3), 1);

  tag_list = gtk_tree_view_new ();
  gtk_widget_show (tag_list);
  gtk_container_add (GTK_CONTAINER (scrolledwindow3), tag_list);
  gtk_tooltips_set_tip (tooltips, tag_list, _("List of tags. Hint-1: Drag a tag to the canvas to obtain an item. Hint-2: Control+drag a tag to the canvas to obtain a 'triangle'."), NULL);
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (tag_list), TRUE);
  gtk_tree_view_set_reorderable (GTK_TREE_VIEW (tag_list), TRUE);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_show (frame2);
  gtk_box_pack_start (GTK_BOX (tag_page), frame2, FALSE, FALSE, 0);

  table2 = gtk_table_new (2, 2, FALSE);
  gtk_widget_show (table2);
  gtk_container_add (GTK_CONTAINER (frame2), table2);
  gtk_container_set_border_width (GTK_CONTAINER (table2), 2);

  tag = gtk_entry_new ();
  gtk_widget_show (tag);
  gtk_table_attach (GTK_TABLE (table2), tag, 0, 1, 1, 2,
                    (GtkAttachOptions) (GTK_EXPAND | GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_tooltips_set_tip (tooltips, tag, _("New tag."), NULL);

  tag_add_button = gtk_button_new_from_stock ("gtk-add");
  gtk_widget_show (tag_add_button);
  gtk_table_attach (GTK_TABLE (table2), tag_add_button, 1, 2, 1, 2,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tag_add_button), 2);
  gtk_tooltips_set_tip (tooltips, tag_add_button, _("Adds the tag to the left to the list."), NULL);

  tag_delete_button = gtk_button_new_from_stock ("gtk-remove");
  gtk_widget_show (tag_delete_button);
  gtk_table_attach (GTK_TABLE (table2), tag_delete_button, 1, 2, 0, 1,
                    (GtkAttachOptions) (GTK_FILL),
                    (GtkAttachOptions) (0), 0, 0);
  gtk_container_set_border_width (GTK_CONTAINER (tag_delete_button), 2);
  gtk_tooltips_set_tip (tooltips, tag_delete_button, _("Deletes the current tag."), NULL);

  label8 = gtk_label_new (_("<b>Possible actions</b>"));
  gtk_widget_show (label8);
  gtk_label_set_use_markup (GTK_LABEL (label8), TRUE);
  gtk_frame_set_label_widget (GTK_FRAME (frame2), label8);
  gtk_label_set_justify (GTK_LABEL (label8), GTK_JUSTIFY_LEFT);

  label6 = gtk_label_new (_("<b>Tags</b>"));
  gtk_widget_show (label6);
  gtk_notebook_set_tab_label (GTK_NOTEBOOK (notebook1), gtk_notebook_get_nth_page (GTK_NOTEBOOK (notebook1), 1), label6);
  gtk_label_set_use_markup (GTK_LABEL (label6), TRUE);
  gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);

  statusbar = gtk_statusbar_new ();
  gtk_widget_show (statusbar);
  gtk_box_pack_start (GTK_BOX (vbox1), statusbar, FALSE, FALSE, 0);

  /*
  g_signal_connect ((gpointer) open2, "activate",
                    G_CALLBACK (open_option),
                    NULL);
  g_signal_connect ((gpointer) close1, "activate",
                    G_CALLBACK (close_option),
                    NULL);
  g_signal_connect ((gpointer) save2, "activate",
                    G_CALLBACK (save_option),
                    NULL);
  g_signal_connect ((gpointer) save_as2, "activate",
                    G_CALLBACK (save_as_option),
                    NULL);
  g_signal_connect ((gpointer) importar1, "activate",
                    G_CALLBACK (import_option),
                    NULL);
  g_signal_connect ((gpointer) export_qtree1, "activate",
                    G_CALLBACK (export_qtree_option),
                    NULL);
  g_signal_connect ((gpointer) export_postscript1, "activate",
                    G_CALLBACK (export_ps_option),
                    NULL);
  g_signal_connect ((gpointer) export_pdf1, "activate",
                    G_CALLBACK (export_pdf_option),
                    NULL);
  g_signal_connect ((gpointer) exit2, "activate",
                    G_CALLBACK (exit_option),
                    NULL);
  g_signal_connect ((gpointer) seleccionar_todo, "activate",
                    G_CALLBACK (select_all_option),
                    NULL);
  g_signal_connect ((gpointer) deseleccionar_todo1, "activate",
                    G_CALLBACK (deselect_all_option),
                    NULL);
  g_signal_connect ((gpointer) invertir_seleccion1, "activate",
                    G_CALLBACK (invert_selection_option),
                    NULL);
  g_signal_connect ((gpointer) acerca_de_arboratrix1, "activate",
                    G_CALLBACK (about_option),
                    NULL);
  g_signal_connect ((gpointer) drawingarea, "button_press_event",
                    G_CALLBACK (da_button_press_event),
                    NULL);
  g_signal_connect ((gpointer) drawingarea, "motion_notify_event",
                    G_CALLBACK (da_motion_notify_event),
                    NULL);
  g_signal_connect ((gpointer) drawingarea, "expose_event",
                    G_CALLBACK (da_expose_event),
                    NULL);
  g_signal_connect ((gpointer) drawingarea, "button_release_event",
                    G_CALLBACK (da_button_release_event),
                    NULL);
  g_signal_connect_swapped ((gpointer) sentence_delete_button, "clicked",
                            G_CALLBACK (sentence_delete_button_clicked),
                            GTK_OBJECT (sentence_list));
  g_signal_connect ((gpointer) sentence_add_button, "clicked",
                    G_CALLBACK (sentence_add_clicked),
                    NULL);
  */

  g_signal_connect ((gpointer) open2, "activate",
                    G_CALLBACK (open_option),
                    this);

  g_signal_connect ((gpointer) save2, "activate",
                    G_CALLBACK (save_option),
                    this);

  g_signal_connect ((gpointer) save_as2, "activate",
                    G_CALLBACK (save_as_option),
                    this);

  g_signal_connect ((gpointer) importar_frases, "activate",
                    G_CALLBACK (import_sentences_option),
                    this);

  g_signal_connect ((gpointer) import_tags1, "activate",
                    G_CALLBACK (import_tags_option),
                    this);

  g_signal_connect ((gpointer) export_tags1, "activate",
                    G_CALLBACK (export_tags_option),
                    this);

  g_signal_connect ((gpointer) export_qtree1, "activate",
                    G_CALLBACK (export_qtree_option),
                    this);

  g_signal_connect ((gpointer) close1, "activate",
                    G_CALLBACK (close_option),
                    this);

  g_signal_connect ((gpointer) exit2, "activate",
                    G_CALLBACK (exit_option),
                    this);
  g_signal_connect ((gpointer) acerca_de_arboratrix1, "activate",
                    G_CALLBACK (about_option),
                    this);
  g_signal_connect ((gpointer) seleccionar_todo, "activate",
                    G_CALLBACK (select_all_option),
                    this);
  g_signal_connect ((gpointer) deseleccionar_todo1, "activate",
                    G_CALLBACK (deselect_all_option),
                    this);
  g_signal_connect ((gpointer) invertir_seleccion1, "activate",
                    G_CALLBACK (invert_selection_option),
                    this);

  /* Store pointers to all widgets, for use by lookup_widget(). */
  GLADE_HOOKUP_OBJECT_NO_REF (tagger, tagger, "tagger");
  GLADE_HOOKUP_OBJECT (tagger, vbox1, "vbox1");
  GLADE_HOOKUP_OBJECT (tagger, menubar, "menubar");
  GLADE_HOOKUP_OBJECT (tagger, fichero1, "fichero1");
  GLADE_HOOKUP_OBJECT (tagger, fichero1_menu, "fichero1_menu");
  GLADE_HOOKUP_OBJECT (tagger, open2, "open2");
  GLADE_HOOKUP_OBJECT (tagger, close1, "close1");
  GLADE_HOOKUP_OBJECT (tagger, separator6, "separator6");
  GLADE_HOOKUP_OBJECT (tagger, save2, "save2");
  GLADE_HOOKUP_OBJECT (tagger, save_as2, "save_as2");
  GLADE_HOOKUP_OBJECT (tagger, separator3, "separator3");
  GLADE_HOOKUP_OBJECT (tagger, importar_frases, "importar_frases");
  GLADE_HOOKUP_OBJECT (tagger, import_tags1, "import_tags1");
  GLADE_HOOKUP_OBJECT (tagger, separator7, "separator7");
  GLADE_HOOKUP_OBJECT (tagger, export_qtree1, "export_qtree1");
  GLADE_HOOKUP_OBJECT (tagger, export_postscript1, "export_postscript1");
  GLADE_HOOKUP_OBJECT (tagger, export_pdf1, "export_pdf1");
  GLADE_HOOKUP_OBJECT (tagger, separator1, "separator1");
  GLADE_HOOKUP_OBJECT (tagger, exit2, "exit2");
  GLADE_HOOKUP_OBJECT (tagger, edicion1, "edicion1");
  GLADE_HOOKUP_OBJECT (tagger, edicion1_menu, "edicion1_menu");
  GLADE_HOOKUP_OBJECT (tagger, seleccionar_todo, "seleccionar_todo");
  GLADE_HOOKUP_OBJECT (tagger, deseleccionar_todo1, "deseleccionar_todo1");
  GLADE_HOOKUP_OBJECT (tagger, invertir_seleccion1, "invertir_seleccion1");
  GLADE_HOOKUP_OBJECT (tagger, help1, "help1");
  GLADE_HOOKUP_OBJECT (tagger, help1_menu, "help1_menu");
  GLADE_HOOKUP_OBJECT (tagger, acerca_de_arboratrix1, "acerca_de_arboratrix1");
  GLADE_HOOKUP_OBJECT (tagger, hbox4, "hbox4");
  GLADE_HOOKUP_OBJECT (tagger, scrolledwindow1, "scrolledwindow1");
  GLADE_HOOKUP_OBJECT (tagger, viewport1, "viewport1");
  GLADE_HOOKUP_OBJECT (tagger, drawingarea, "drawingarea");
  GLADE_HOOKUP_OBJECT (tagger, vseparator2, "vseparator2");
  GLADE_HOOKUP_OBJECT (tagger, notebook1, "notebook1");
  GLADE_HOOKUP_OBJECT (tagger, sentence_page, "sentence_page");
  GLADE_HOOKUP_OBJECT (tagger, scrolledwindow2, "scrolledwindow2");
  GLADE_HOOKUP_OBJECT (tagger, sentence_list, "sentence_list");
  GLADE_HOOKUP_OBJECT (tagger, frame1, "frame1");
  GLADE_HOOKUP_OBJECT (tagger, table1, "table1");
  GLADE_HOOKUP_OBJECT (tagger, sentence_delete_button, "sentence_delete_button");
  GLADE_HOOKUP_OBJECT (tagger, sentence, "sentence");
  GLADE_HOOKUP_OBJECT (tagger, sentence_add_button, "sentence_add_button");
  GLADE_HOOKUP_OBJECT (tagger, finish_button, "finish_button");
  GLADE_HOOKUP_OBJECT (tagger, label7, "label7");
  GLADE_HOOKUP_OBJECT (tagger, label5, "label5");
  GLADE_HOOKUP_OBJECT (tagger, tag_page, "tag_page");
  GLADE_HOOKUP_OBJECT (tagger, scrolledwindow3, "scrolledwindow3");
  GLADE_HOOKUP_OBJECT (tagger, tag_list, "tag_list");
  GLADE_HOOKUP_OBJECT (tagger, frame2, "frame2");
  GLADE_HOOKUP_OBJECT (tagger, table2, "table2");
  GLADE_HOOKUP_OBJECT (tagger, tag, "tag");
  GLADE_HOOKUP_OBJECT (tagger, tag_add_button, "tag_add_button");
  GLADE_HOOKUP_OBJECT (tagger, tag_delete_button, "tag_delete_button");
  GLADE_HOOKUP_OBJECT (tagger, label8, "label8");
  GLADE_HOOKUP_OBJECT (tagger, label6, "label6");
  GLADE_HOOKUP_OBJECT (tagger, statusbar, "statusbar");
  GLADE_HOOKUP_OBJECT_NO_REF (tagger, tooltips, "tooltips");

  gtk_widget_grab_focus (drawingarea);
  gtk_window_add_accel_group (GTK_WINDOW (tagger), accel_group);

  return tagger;
}
