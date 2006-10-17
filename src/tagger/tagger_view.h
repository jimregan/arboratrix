// -*- C++ -*-

#ifndef _TAGGER_VIEW_H_
#define _TAGGER_VIEW_H_

#include <mvc/view.h>
#include <gui/window.h>
#include <gui/glade2.h>
#include <gui/yes_no_dialog.h>
#include <sentence_list.h>
#include <canvas.h>
#include <tag_list.h>
#include <tag_info.h>
#include <index_dialog.h>

//#include "pixmaps.h"

#define _(x) x

/*
 * Clase:      Tagger_view
 * Hereda:     View Window 
 * Version:    1.0
 * Creada:     Mon Dec 23 17:03:17 CET 2002
 * Modificada: Mon Dec 23 17:03:17 CET 2002
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Vista del etiquetador sobre una ventana.
 */

class Tagger_model;

/*!
 * \class Tagger_view
 * \brief Vista del etiquetador sobre una ventana.
 */
class Tagger_view : public acb_mvc::View , public acb_gui::Window
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  /*! Constructor por defecto. 
   *
   * No deberia emplearse otro constructor con objetos de esta clase.
   */
  Tagger_view(Tagger_model& m);

  //! Destructor.
  ~Tagger_view();

  //! El modelo asociado pide actualizar esta vista.
  void update();

  //! Actualizacion del modelo ordenada por la vista.
  void update_model();
  //! Devuelve el modelo con el tipo apropiado.
  Tagger_model* get_model() { return dynamic_cast<Tagger_model*>(model); }
  //! Devuelve el modelo con el tipo apropiado.
  Tagger_model* get_model() const { return dynamic_cast<Tagger_model*>(model); }

  //! Crea una frase a partir de una cadena.
  Sentence* create_sentence(const std::string& s);
  //! Lee los datos (frases, etiquetas) desde el modelo.
  void load_data_from_model();
  //! Lee solo las etiquetas desde el modelo.
  void load_tags_from_model();

  //! Devuelve el canvas donde dibujamos.
  Canvas* get_canvas() { return canvas; }
  //! Indica la frase a pintar en el canvas
  void set_canvas_sentence(Sentence* s) { canvas->set_sentence(s); }
  //! Devuelve el widget lista de frases.
  Sentence_list* get_sentence_list() { return sl; }
  //! Añade una frase al widget lista_de_frases.
  void add_sentence_to_list(Sentence* s);
  //! Devuelve el widget lista de etiquetas.
  Tag_list* get_tag_list() { return tl; }
  //! Añade una etiqueta al widget lista_de_etiquetas.
  void add_tag_to_list(Tag_info* t);

  //! Crea un nuevo documento vacio en la vista y en el modelo.
  void new_document();
  //! Limpia las vistas (canvas, lista de frases y tags).
  void clean_views();

  //! Muestra el selector de ficheros.
  void select_file_name(const std::string& title = _("Select a file:")) const;

  /*! Muestra un mensaje en la barra de estado.  \param message La
   * cadena a mostrar.  \param tout El tiempo en msg. que tarda en
   * desaparecer, por defecto 3sg.*/
  void show_statusbar_message(const std::string& message, int tout = 3000);

  /*! El usuario ha pulsado el boton derecho, quiere mostrar el menu
   * popup.*/
  void show_popup_menu(GdkEventButton* event);
  //! Deshabilita los items no validos del pop-up usado en el canvas.
  void cm_disable_items();
  //! Habilita los items validos del pop-up usado en el canvas.
  void cm_enable_items();


  /*-------------------CALLBACKS------------------*/

  //! Callback opcion leer documento.
  static void open_option (GtkMenuItem *menuitem, gpointer user_data);
  //! Callback opcion guardar documento.
  static void save_option(GtkMenuItem* menuitem, gpointer user_data);
  //! Callback opcion cerrar documento.
  static void close_option(GtkMenuItem* menuitem, gpointer user_data);

  //! Callback opcion importar frases de un documento en formato ascii.
  static void import_sentences_option (GtkMenuItem *menuitem, gpointer user_data);
  //! Callback opcion importar tags de un documento en formato ascii.
  static void import_tags_option (GtkMenuItem *menuitem, gpointer user_data);
  //! Callback opcion exportar tags de un documento en formato ascii.
  static void export_tags_option (GtkMenuItem *menuitem, gpointer user_data);

  //! Callback opcion exportar documento en formato qtree.
  static void export_qtree_option (GtkMenuItem *menuitem, gpointer user_data);
  //! Callback opcion exportar documento en formato postscript.
  static void export_ps_option (GtkMenuItem *menuitem, gpointer user_data);
  //! Callback opcion exportar documento en formato pdf.
  static void export_pdf_option (GtkMenuItem *menuitem, gpointer user_data);

  //! Callback opcion guardar documento con otro nombre.
  static void save_as_option (GtkMenuItem *menuitem, gpointer user_data);

  //! Callback invocado al seleccionar un nombre de documento.
  static void fselector_accept_cb(GtkWidget* widget, gpointer user_data);

  //! Callback invocado al no seleccionar un nombre de documento.
  static void fselector_cancel_cb(GtkWidget* widget, gpointer user_data);

  //! Finaliza la aplicacion
  static void exit_option(GtkMenuItem* menuitem, gpointer user_data);

  //! Acerca_de la aplicacion
  static void about_option(GtkMenuItem* menuitem, gpointer user_data);

  //! Selecciona todos los items de la frase actual
  static void select_all_option(GtkMenuItem* menuitem, gpointer user_data);

  //! Deselecciona todos los items de la frase actual
  static void deselect_all_option(GtkMenuItem* menuitem, gpointer user_data);

  //! Alterna la seleccion de todos los items de la frase actual
  static void invert_selection_option(GtkMenuItem* menuitem, gpointer user_data);

  static void reorder_tree_option(GtkMenuItem *menuitem, gpointer user_data);
  static void delete_node_option(GtkMenuItem *menuitem, gpointer user_data);
  static void unparent_node_option(GtkMenuItem *menuitem, gpointer user_data);
  static void untriangle_node_option(GtkMenuItem *menuitem, gpointer user_data);
  static void deadjoint_node_option(GtkMenuItem *menuitem, gpointer user_data);
  static void empty_category_node_option(GtkMenuItem *menuitem, gpointer user_data);
  static void add_index_option(GtkMenuItem *menuitem, gpointer user_data);
  static void delete_index_option(GtkMenuItem *menuitem, gpointer user_data);
  /*-------------------CALLBACKS------------------*/

  // Parte protegida.
protected:
  //! Subwidget que representa la lista de frases.
  Sentence_list* sl;
  //! Subwidget que representa la lista de tags.
  Tag_list* tl;
  //! Subwidget que representa el canvas.
  Canvas* canvas;

  //! La barra de estado de la aplicacion.
  GtkWidget* statusbar;
  //! El selector de ficheros.
  GtkWidget* file_selector;
  //! Define como sera el gui del tagger. Su codigo lo genera glade.
  GtkWidget* create_gui();
  //! El menu pop-up empleado en el canvas.
  GtkWidget* cpm;
  //! El nombre del fichero donde se guarda el documento.
  std::string doc_name;
  /*! Nos dice el nombre del fichero elegido por el usuario tras
   * mostrarle el ultimo selector de ficheros.*/
  std::string file_selected;


  //! Callback para eliminar los mensajes de la barra de estado.
  static gint statusbar_message_remover(gpointer data);
  //! Crea el selector de ficheros.
  GtkWidget* create_file_selector();
  //! Crea el menu pop-up usado en el canvas.
  GtkWidget* create_canvas_menu();

  //! Garantiza una extension para el nombre de un fichero.
  void ensure_file_extension(std::string& fn, const std::string& ext);

  // Parte privada.
private:

};

#endif
