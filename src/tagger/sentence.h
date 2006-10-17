// -*- C++ -*-

#ifndef _SENTENCE_H_
#define _SENTENCE_H_

#if HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#define _(x) x

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <iostream>
#include <tagger_view.h>
#include <text_model.h>
#include <canvas.h>
#include <exporter.h>
#include <string>
#include <utils.h>

/*
 * Clase:      Sentence
 * Hereda:     De nadie, es clase base.
 * Version:    1.0
 * Creada:     Thu Mar  6 17:18:16 CET 2003
 * Modificada: Thu Mar  6 17:18:16 CET 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@peurl.dlsi.ua.es
 * Proposito:  Representa una frase con la que se trabaja.
 */

class Tag_model;
class Tag_item;

/*!
 * \class Sentence
 * \brief Representa una frase con la que se trabaja. Pertenece al modelo.
 */
class Sentence : public Exporter
{
  // Declaraciones friend --si las hubiera--.
  friend std::ostream& operator<< (std::ostream& os, const Sentence& s);

  // Parte publica.
public:

  /*! Constructor por defecto.  \param tv El Tagger_view sobre el que
   * se muestra la frase.  \param s El texto de la frase.  \param
   * only_root Si es cierto solo crea el nodo raiz y no crea un nodo
   * para cada elemento de la frase.*/
  Sentence(Tagger_view* tv, const std::string& s, bool only_root = false);

  //! Destructor.
  virtual ~Sentence();

  //! Devuelve el texto de la frase
  const std::string& get_text() { return sentence; }

  //! Obtiene la lista de items de esta frase
  const Canvas::Itemlist* get_item_list() const { return &item_list; }
  Canvas::Itemlist* get_item_list() { return &item_list; }

  //! El nodo raiz de los items de esta frase.
  Text_model* get_root() { return root_node; }

  //! Otro algoritmo de dibujado del arbol.
  void layout_tree_in_screen();
  //! Calcula la matriz que engloba al arbol.
  void layout_tree_in_matrix();

  //! Modifica el atributo de frase 'terminada'.
  void set_finished(bool f) { finished = f; }
  //! Inidica si esta frase ya ha sido etiquetada.
  bool is_finished() { return finished; }

  //! Representación XML de una frase con nodo padre 'parent'.
  void to_xml(xmlNodePtr parent);
  /*! Convierte el modelo de la frase en formato qtree, dejando la
   * representacion en el stream \param os.*/
  void to_qtree(std::ostream& os);
  //! Una frase no puede convertirse por si sola a postscript.
  void to_ps(std::ostream& os) {}
  //! Una frase no puede convertirse por si sola a PDF.
  void to_pdf(std::ostream& os) {}

  /*! Permite añadir un item con texto 'w' en las coordenadas (x,y) a
   * una frase ya creada. */
  Text_item* add_text_item(const std::string& w, Text_model* parent, 
			   int x = 0, int y = 0);
  /*! Permite añadir un item 'tag' con texto 'w' en las coordenadas
   * (x,y) a una frase ya creada.*/
  Tag_item* add_tag_item(const std::string& w, Tag_model* parent,
			 int x = 0, int y = 0);

  /*! Permite añadir una traza generada al arrastar el usuario el item
   * 'orig' sobre 'dest'*/
  Text_item* add_trace(Text_model* dest, Text_model* orig);

  //! Crea un adjunto a partir de un text_item y un tag_item
  void create_adjoint(Text_item* txi, Tag_item* tgi);

  //! Elimina un item (text o tag) de la lista de los de la frase.
  void remove_item(Text_item* ti);

  //! Devuelve la siguiente etiqueta valida.
  ulong get_next_label() { return ++max_label; }
  //! Devuelve la siguiente etiqueta valida como cadena.
  std::string get_next_label_str() { return Utils::ultos(++max_label); }

  // Parte protegida.
protected:
  //! Lista de todos los items a dibujar en el canvas.
  Canvas::Itemlist item_list;
  //! La raiz del arbol de nodos del arbol de la frase.
  Text_model* root_node;
  //! La frase completa.
  std::string sentence;
  //! Indica si la frase ya esta marcada como 'terminada'.
  bool finished;
  //! El etiquetador usado con esta frase (taggerdialog)
  const Tagger_model* tagger_model;
  //! Y la vista del etiquetador.
  Tagger_view* tagger_view;

  /*! Devuelve la lista de text_items de un nivel dado del arbol.  El
   * receptor de la lista es responsable de liberar la memoria que
   * ocupa.*/
  std::list<Text_item*>* get_text_item_list_of_level(int l);
  /*! Devuelve la lista de text_models de un nivel dado del arbol.  El
   * receptor de la lista es responsable de liberar la memoria que
   * ocupa.*/
  std::list<Text_model*>* get_text_model_list_of_level(int l);

  //! Lleva la cuenta de las etiquetas usadas en las trazas.
  ulong max_label;

  /*! Devuelve el numero de nodos reales de un nivel, sin contar los
   * del nivel anterior.*/
  int get_level_real_nodes(int l);

  //! Devuelve el numero de nodos 'virtuales' de un nivel.
  int get_level_virtual_nodes(int l);

  //! Devuelve la altura maxima en pixeles de los items del nivel 'l';
  int get_max_text_height_of_level(int l);

  /*! Devuelve el numero de nodos del arbol de la frase.
   * Incluye la raiz-no-visible del arbol.*/
  int get_tree_total_nodes() { return item_list.size() + 1; }

  /*! Devuelve el numero de niveles del arbol.
   * La raiz-no-visible del arbol ocupa el nivel 0.*/
  int get_tree_total_levels();

  //! Similar a \sa get_tree_total_levels.
  int get_tree_height() { return get_tree_total_levels(); }

  //! Devuelve la anchura maxima del arbol medida en 'nodos'.
  int get_tree_width();

  //! Trocea la frase por caracteres de puntuacion.
  void tokenize();

  //! Hace que el arbol no se salga por la izquierda del canvas.
  void correct_negative_offset();
  /*! Para todo el arbol, hace que los nodos de un mismo bivel no se
   * solapen.*/
  void correct_overlap();
  /*! Calcula la celda de cada hijo del padre q se pasa como parametro.
   * Debe ser llamada desde Sentence::layout_tree_in_matrix.*/
  void layout_children_in_matrix(Text_model* parent, int& offset);

  //Parte privada.
private:
  //! Crea el nodo raiz de la frase.
  void create_root_node();
};

#endif
