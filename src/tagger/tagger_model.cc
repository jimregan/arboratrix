// -*- C++ -*-

/*
 * Creado:     Mon Dec 23 16:46:58 CET 2002
 * Modificado: Mon Dec 23 16:46:58 CET 2002
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <iostream>
#include <algorithm>

#include <text_model.h>
#include <tag_item.h>
#include <tagger_model.h>
#include <tagger_view.h>
#include <sentence.h>
#include <canvas_item.h>
#include <tag_info.h>

using namespace acb_mvc;

/*
 * Constructor por defecto.
 */
Tagger_model::Tagger_model()
{
  //std::cout << "Creado modelo TAGGER.\n";

  //Inicialmente el tagger_view es nulo.
  tv = NULL;
}

/*
 * Destructor.
 */
Tagger_model::~Tagger_model()
{
  clear_lists();
  //std::cout << "Destruido modelo TAGGER.\n";
}

/*
 * Add sentence
 */
Sentence*
Tagger_model::add_sentence(const std::string& s, bool only_root)
{
  Sentence* ns = NULL;

  if ((tv) and (is_valid_string(s)))
    {
      //La creamos...
      ns = new Sentence(tv, s, only_root);
      //Y la añadimos a la lista de frases
      add_sentence(ns);

      //std::cout << "Creada la frase [" << s << "]\n";
    }
  else
    {
      //std::cerr << "tv es NULO!, no deberia ocurrir.\n";
      //std::cout << "Frase descartada [" << s << "]\n";
    }

  return ns;
}

bool
Tagger_model::is_valid_string(const std::string& s)
{
  bool result = true;
  uint count  = 0;

  if (s.length() == 0)
    result = false;

  for (uint i = 0; i < s.length(); i++)
    {
      char c = s[i];
      if ((c == ' ') or (c == '\n') or 
	  (c == '\t') or (c == '\r'))
	count++;
    }

  if (count == s.length())
    result = false;

  return result;
}

/*
 * Add tag
 */
Tag_info* 
Tagger_model::add_tag(const std::string& s)
{
  Tag_info* pti = NULL;

  if (not exists_tag(s))
    {
      pti = new Tag_info(s);
      
      //std::cout << "No existe tag-en-modelo " << s << std::endl;
      // La añadimos a la lista de frases de este tagger
      tl.push_back(pti);
    }

  return pti;
}

void
Tagger_model::save_document_qtree(std::ofstream& ofs)
{
  //Generamos cada frase en formato qtree
  for (std::list<Sentence*>::iterator s = sl.begin(); s != sl.end(); s++)
    {
      (*s)->to_qtree(ofs);
      ofs << std::endl;
    }
}

void
Tagger_model::remove_sentence(Sentence* s)
{ 
  //std::cout << "Borro del modelo la frase: " << *s << std::endl;
  sl.remove(s); 
}

void
Tagger_model::save_document_xml(const std::string& fn)
{
  xmlDocPtr   doc;
  xmlNodePtr  root;

  doc  = xmlNewDoc(reinterpret_cast<const xmlChar *>("1.0"));
  root = xmlNewDocNode(doc, NULL, 
		       reinterpret_cast<const xmlChar *>("tagged_sentences"),
		       NULL);
  xmlDocSetRootElement(doc, root);

  //Generamos el resto de nodos XML para cada frase...
  for (std::list<Sentence*>::iterator s = sl.begin(); s != sl.end(); s++)
    {
      //std::cout << "Guardo la frase: " << *s << std::endl;
      (*s)->to_xml(root);
    }

  //xmlSaveFormatFile(fn.c_str(), doc, true);
  xmlSaveFormatFileEnc(fn.c_str(), doc, "UTF-8", true);
  xmlFreeDoc(doc);
}

bool
Tagger_model::exists_tag(const std::string& s)
{
  bool found = false;

  //std::cout << "Lista de tags con " << tl.size() << " elementos\n";

#if 0
  for(std::list<Tag_info*>::iterator i = tl.begin(); 
      (i != tl.end()) and not found; i++)
    {
      found = (s == (*i)->get_tag());
      //std::cout << "Comparo [" << s << "] con [" << (*i)->get_tag() << "]\n";
    }
#endif

  found = find(tl.begin(), tl.end(), s) != tl.end();

  return found;
}

void 
Tagger_model::clear_lists()
{ 

  for(std::list<Sentence*>::iterator i = sl.begin();
      i != sl.end(); i++)
    delete *i;
  sl.clear();

  for(std::list<Tag_info*>::iterator i = tl.begin();
      i != tl.end(); i++)
    delete *i;
  tl.clear(); 
}

void 
Tagger_model::new_document()
{ 

  clear_lists();
  update_views(); 
}

bool
Tagger_model::import_sentences(const std::string& fn)
{
  std::ifstream ifs;
  std::string   s;
  Sentence*     ls;
  bool          result = true;

  ifs.open(fn.c_str());
  if (ifs)
    {
      while (not ifs.eof())
	{
	  getline(ifs, s);
	  if (s.length() > 0)
	    {
	      //std::cout << "Leo la frase [" << s << "]\n";
	      //Creamos la frase en el modelo
	      ls = add_sentence(s);
	    }
	}
      ifs.close();
    }
  else
    result = false;

  return result;
}

bool
Tagger_model::import_tags(const std::string& fn)
{
  std::ifstream ifs;
  std::string   s;
  Tag_info*     ti;
  bool          result = true;

  ifs.open(fn.c_str());
  if (ifs)
    {
      while (not ifs.eof())
	{
	  //std::cout << "\t>>>> Leida linea-tag " << i++ << std::endl;
	  getline(ifs, s);
	  if (s.length() > 0)
	    {
	      //Creamos la frase en el modelo
	      ti = add_tag(s);
	    }
	}
      ifs.close();
    }
  else
    result = false;

  return result;
}

bool
Tagger_model::export_tags(const std::string& fn)
{
  std::ofstream ofs;
  std::string   s;
  bool          result = true;

  ofs.open(fn.c_str());
  if (ofs)
    {
      for(std::list<Tag_info*>::iterator i = tl.begin();
	  i != tl.end(); i++)
	{
	  ofs << (*i)->get_tag() << std::endl;
	}
      ofs.close();
    }
  else
    result = false;

  return result;
}

bool
Tagger_model::load_document_xml(const std::string& fn)
{
  xmlDocPtr   doc;
  xmlNodePtr  root;
  std::string tipo   = "tagged_sentences";
  bool        result = false;

  //Cargamos en memoria el arbol XML
  doc = xmlParseFile(fn.c_str());
  if (doc)
    {
      root = xmlDocGetRootElement (doc);
      
      if (tipo == reinterpret_cast<char*>(const_cast<xmlChar*>(root->name)))
	{
	  //Borro el actual en memoria.
	  new_document();
	  //ultime frase leida es vacia
	  last_read_sentence = NULL;

	  //Y ahora traduzco el documento xml a 'Sentences'.
	  from_xml_to_sentences(root);

	  result = true;
	}
      else
	{
	  result = false;
	}      
      //Liberamos el arbol XML.
      xmlFreeDoc (doc);
    }
  else
    result = false;

  return result;
}

Tagger_model::Node_xml
Tagger_model::get_xml_node_type(xmlNodePtr node)
{
  std::string phrase_tag   = "phrase";
  std::string text_tag     = "ut";
  std::string triangle_tag = "impl";
  std::string sentence_tag = "sentence";

  //std::cout << "get_xml_node_type\n";

  if (not node)
    return EMPTY;

  if (sentence_tag == reinterpret_cast<const char*>(node->name))
    return SENTENCE;

  if (text_tag == reinterpret_cast<const char*>(node->name))
    return TEXT;

  if (triangle_tag == reinterpret_cast<const char*>(node->name))
    return TRIANGLE;

  if (phrase_tag == reinterpret_cast<const char*>(node->name))
    return TAG;

  // caza todo
  return EMPTY;
}

void
Tagger_model::from_xml_to_sentences(xmlNodePtr n, Text_model* parent)
{
  xmlNodePtr       node;
  std::string      is_finished = "true";
  std::string      status;
  std::string      text;
  std::string      idx_str;
  std::string      tag_str;
  std::string      triangle_str;
  std::string      content;
  Text_item*       text_i;
  Tag_item*        tag_i;
  Tag_info*        tag_info;

  //std::cerr << "\t\tPongo hijos a: " << n->name << std::endl;

  //root = xmlDocGetRootElement (doc);
  node = n->xmlChildrenNode;

  while (node)
    {
      switch (get_xml_node_type(node))
	{
	case SENTENCE: //hemos leido una frase
	  //std::cout << "Leo una frase\n";

	  // 1) Propiedad 'text'
	  if (xmlHasProp(node, reinterpret_cast<const xmlChar*>("text")))
	    {
	      text = reinterpret_cast<char*>
		(xmlGetProp (node, reinterpret_cast<const xmlChar*> 
			     ("text")));

	      last_read_sentence = add_sentence(text, true); //Solo le creamos el nodo raiz.
	      //tv->add_sentence_to_list(s);
	    }
	  // 2) Propiedad 'finished'
	  if (xmlHasProp (node, reinterpret_cast<const xmlChar*>("finished")))
	    {
	      status = reinterpret_cast<char*>
		(xmlGetProp (node, reinterpret_cast<const xmlChar*> 
			     ("finished")));
	    }
	  else
	    status = "false";
	  if (is_finished == status)
	    last_read_sentence->set_finished (true);
	  else
	    last_read_sentence->set_finished (false);

	  // ..RECURSIVO..
	  from_xml_to_sentences(node, last_read_sentence->get_root());

	  // Posicionamos los nodos en sus coordenadas.
	  last_read_sentence->layout_tree_in_screen();
	  break;

	case TEXT:     //hemos leido una palabra
	  //std::cout << "Leo un texto\n";
	  content = reinterpret_cast<char*>(xmlNodeGetContent(node));
	  text_i = last_read_sentence->add_text_item(content, parent);
	  // 1) Propiedad 'index'
	  if (xmlHasProp(node, reinterpret_cast<const xmlChar*>("index")))
	    {
	      idx_str = reinterpret_cast<char*>
		(xmlGetProp (node, reinterpret_cast<const xmlChar*> 
			     ("index")));
	      
	      text_i->set_index(idx_str);
	    }
	  break;

	case TAG:      //hemos leido una etiqueta
	  //std::cout << "Leo una etiqueta\n";
	  tag_str = reinterpret_cast<char*>(xmlGetProp(node, 
						       reinterpret_cast<const xmlChar*>("class")));

	  // Debemos dar trato especial al nodo raiz...no se crea pues
	  // ya existe al crear la frase.
	  if (tag_str == "ROOT")
	    // ..RECURSIVO..
	    from_xml_to_sentences(node, parent);
	  else
	    {
	      //Creamos el tag_item...
	      tag_i = last_read_sentence->add_tag_item(tag_str, 
						       dynamic_cast<Tag_model*>(parent));
	      // 1) Propiedad 'index'
	      if (xmlHasProp(node, reinterpret_cast<const xmlChar*>("index")))
		{
		  idx_str = reinterpret_cast<char*>
		    (xmlGetProp (node, reinterpret_cast<const xmlChar*> 
				 ("index")));
	      
		  tag_i->set_index(idx_str);
		}
	      //Y añadimos el tag a la lista de tags
	      tag_info = add_tag(tag_str);

	      // ..RECURSIVO..
	      from_xml_to_sentences(node, tag_i->get_model());
	    }
	  break;

	case TRIANGLE: //hemos leido un triangulo
	  //std::cout << "Leo un triangulo\n";
	  tag_str = reinterpret_cast<char*>(xmlGetProp(node, 
						       reinterpret_cast<const xmlChar*>("class")));

	  // Debemos dar trato especial al nodo raiz...no se crea pues
	  // ya existe al crear la frase.
	  //Creamos el tag_item...
	  tag_i = last_read_sentence->add_tag_item(tag_str, 
						   dynamic_cast<Tag_model*>(parent));
	  // 1) Propiedad 'index'
	  if (xmlHasProp(node, reinterpret_cast<const xmlChar*>("index")))
	    {
	      idx_str = reinterpret_cast<char*>
		(xmlGetProp (node, reinterpret_cast<const xmlChar*> 
			     ("index")));
	      
	      tag_i->set_index(idx_str);
	    }
	  //E indicamos q es un triangulo
	  tag_i->set_triangle(true);
	  //Y añadimos el tag a la lista de tags
	  tag_info = add_tag(tag_str);

	  // ..RECURSIVO..
	  from_xml_to_sentences(node, tag_i->get_model());
	  break;

	case EMPTY:    // se trata de un nodo vacio
	default:
	  //std::cout << "NODO VACIO!\n";
	  break;
	}
      node = node->next;
    }
}
