// -*- C++ -*-

#ifndef _TAGGER_MODEL_H_
#define _TAGGER_MODEL_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

#include <fstream>
#include <mvc/model.h>
#include <list>

#include "tag_model.h"
#include "tag_info.h"

#define _(x) x

/*
 * Clase:      Tagger_model
 * Hereda:     Model
 * Version:    1.0
 * Creada:     Mon Dec 23 16:46:58 CET 2002
 * Modificada: Mon Dec 23 16:46:58 CET 2002
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Modelo del etiquetador.
 */

class Sentence;
class Canvas;
class Text_model;
class Tagger_view;

/*!
 * \class Tagger_model
 * \brief Modelo del etiquetador.
 */
class Tagger_model : public acb_mvc::Model
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto.
  Tagger_model();
  //! Destructor.
  ~Tagger_model();

  /*! A partir de una cadena, crea y añade una frase a la lista del
   * tagger.  \param only_root Si es cierto, la frase solo constara
   * internamente del nodo raiz.  \returns La Frase recien creada.*/
  Sentence* add_sentence(const std::string& s, bool only_root = false);
  //! Añade una frase a la lista del modelo del tagger
  void add_sentence(Sentence* s) { sl.push_back(s); }
  //! Elimina una frase de la lista de frases.
  void remove_sentence(Sentence* s);
  //! Devuelve la cantidad de frases que tiene este modelo.
  int get_sentence_count() { return sl.size(); }
  //! Devuelve la lista con todas las frases.
  const std::list<Sentence*>* get_sentence_list() { return &sl; }
  //! Devuelve la ultima frase leida de un fichero.
  Sentence* get_last_read_sentence() { return last_read_sentence; }
  //! Devuelve la lista con todas las etiquetas.
  const std::list<Tag_info*>* get_tag_list() { return &tl; }

  /*! Crea una etiqueta a partir de una cadena y la añade a la lista
   * del tagger.*/
  Tag_info* add_tag(const std::string& s);
  //! Elimina una etiqueta de la lista de etiquetas.
  void remove_tag(Tag_info* pti) { tl.remove(pti); }
  //! Devuelve la cantidad de etiquetas que tiene modelo.
  int get_tag_count() { return tl.size(); }

  // Suministra al canvas esta frase para pintarla.
  void set_canvas_active_sentence(Sentence* s);

  //! Crea un documento nuevo.
  void new_document();

  /*! Recibe un ofstream abierto y vuelca en el el documento actual en
   * formato qtree. El arbol de cada frase por linea.*/
  void save_document_qtree(std::ofstream& ofs);

  /*! Guarda las frases del modelo. El formato empleado es xml.
   * \param fn El nombre del fichero. */
  void save_document_xml(const std::string& fn);

  /*! Lee las frases del modelo de un fichero. El formato empleado es
   * xml.  \param fn El nombre del fichero. \return Cierto si se puede
   * leer el fichero, falso en cualquier otro caso.*/
  bool load_document_xml(const std::string& fn);

  //! Importa frases desde un fichero ascii, una por linea.
  bool import_sentences(const std::string& fn);
  //! Importa tags desde un fichero ascii, una por linea.
  bool import_tags(const std::string& fn);
  //! Exporta tags desde un fichero ascii, una por linea.
  bool export_tags(const std::string& fn);
  
  //! Nos dice si ya existe este 'tag'
  bool exists_tag(const std::string& s);

  //! Da valor al tagger_view sobre este modelo.
  void set_tagger_view(Tagger_view* ptv) { tv = ptv; }

  // Parte protegida.
protected:
  //! Mi vista principal
  Tagger_view* tv;
  //! La lista de frases con las que trabaja el usuario.
  std::list<Sentence*> sl;
  //! La lista de 'tags' con los que trabaja el usuario.
  std::list<Tag_info*> tl;
  //! Los distintos tipos de nodos que puede haber el fichero XML.
  enum Node_xml { EMPTY, SENTENCE, TAG, TRIANGLE, TEXT };
  //! Dado un nodo-xml, devuelve su tipo \sa Node_xml.
  Node_xml get_xml_node_type(xmlNodePtr node);
  //! Frase empleada en la lectura de un fichero.
  Sentence* last_read_sentence;

  //! Comprueba si la cadena 's' puede generar una frase valida.
  bool is_valid_string(const std::string& s);

  //! Libera la memoria de las listas de frases y etiquetas
  void clear_lists();

  /*! Se encarga de 'generar' el arbol de cada una de las frases
   * leidas desde un fichero.*/
  void from_xml_to_sentences(xmlNodePtr n, Text_model* parent = NULL);

  //Parte privada.
private:

};

#endif
