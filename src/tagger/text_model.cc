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
 * Creado:     Fri Jan 10 16:46:36 CET 2003
 * Modificado: Fri Jan 10 16:46:36 CET 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Emai
l:      acorbi@hal9000
 */

#include <algorithm>
#include <iostream>
#include <text_model.h>
#include <text_item.h>
#include <sentence.h>

using namespace acb_mvc;

//const std::string Text_model::empty_set  = g_locale_to_utf8 ("�", 1, NULL, NULL, NULL);
std::string Text_model::empty_set; // vacia inicialmente

const std::string&
Text_model::get_empty_set()
{
  return empty_set;
}

/*
 * Constructor por defecto.
 */
Text_model::Text_model(const std::string& nt, Text_model* pparent)
{
  //EMPTY_SET
  if (empty_set == "")
    {
      //gsize w, h;
      // Representacion UTF8 de '�'.
      //empty_set = g_locale_to_utf8 ("ø", 2, &w, &h, NULL);
      empty_set = "ø";
    }

  the_sentence      = NULL;
  trace_destination = NULL;
  adjoint_son       = NULL;
  parent            = NULL; //Necesario por si llamamos a add_child

  if (pparent)
    pparent->add_child(this);
  else
    parent = pparent;

  text = nt;
  tag  = "ut";
  children.clear();

  label     = "";
  reference = "";
}

/*
 * Destructor.
 */
Text_model::~Text_model()
{
  //std::cout << "Destruido Text_model.\n";
}

std::ostream&
operator<< (std::ostream& os, const Text_model& tm)
{
  os << tm.text;

  return os;
}

bool
Text_model::is_direct_son(Text_model* child)
{
  bool result = false;

  if (child)
    {
      if (child->parent == this) // Hijo directo
	result = true;
    }

  return result;
}

bool
Text_model::is_son(Text_model* child)
{
  bool result = false;

  if (child)
    {
      if (child->parent == this) // Hijo directo
	result = true;
      else // Hijo indirecto
	result = result or is_son(child->parent);
      return result;
    }
  else
    return result;
}

bool
Text_model::is_son_of(Text_model* parent)
{
  return parent->is_son(this);
}

void
Text_model::add_child(Text_model* child)
{
  if (can_have_sons())
    {
      if (child and                    // Si hay posible hijo
	  (child != this) and          // Si no soy yo
	  not is_direct_son(child) and // Si aun no es mi hijo
	  not is_son_of(child))        // Y yo no soy su hijo
	{
	  //Lo quito de su padre?
	  child->remove_from_parent();
	  //Lo a�ado al nuevo
	  children.push_back(child);
	  child->set_parent(this);
	}
    }
}

void
Text_model::remove_child(Text_model* child)
{
  if ((child) and
      (child != this) and
      is_son(child))
      //and (find(children.begin(), children.end(), child) != children.end()))
    {
      children.remove(child);
      child->set_parent(NULL);
    }
}

void
Text_model::remove_from_parent()
{
  if (parent)
    {
      parent->remove_child(this);
    }
}

void
Text_model::remove_from_parent_and_adopt()
{
  std::list<Text_model*>*          cl; //children list
  std::list<Text_model*>           tl; //temp children list
  Text_model*                      op; //old parent
  std::list<Text_model*>::iterator j;
 
  if (parent)
    {
      op = parent;

      // REMOVE
      remove_from_parent();
      // ADOPT
      if (get_num_children() > 0)
	{
	  //std::cout << "Y tiene " << m->get_num_children() << " hijos.\n";
	  //Obtengo sus hijos para...
	  cl = get_children();
	  //Colgarselos al que era su padre.

	  /* Debo usar una lista
	   * auxiliar pq perdia los iteradores.*/
	  for(j = cl->begin(); j != cl->end(); j++)
	    {
	      tl.push_back(*j);
	    }
	  for(j = tl.begin(); j != tl.end(); j++)
	    {
	      /*std::cout << "Pasamos....\n";
		std::cout << "Reparentizo: " << **j << std::endl;*/
	      op->add_child(*j);
	    }
	}
    }
}

void
Text_model::set_parent(Text_model* new_parent)
{
  //sanity-check
  if ((new_parent != this) and (new_parent != parent))
    {
      parent = new_parent;
    }
}

void
Text_model::set_text(const std::string& nt)
{
  text = nt;
  update_views();
}

void
Text_model::set_index(const std::string& nidx)
{
  index = nidx;
  update_views();
}

void 
Text_model::update_children()
{
  std::list<Text_model*>::iterator i;

  for(i = children.begin(); i != children.end(); i++)
    {
      (*i)->update_views();
    }
}

int
Text_model::get_level()
{
  if (is_root())
    {
      return 0;
    }
  else
    {
      return 1 + parent->get_level();
    }
}

Text_item*
Text_model::get_first_view()
{ 
  return dynamic_cast<Text_item*>(lv.front()); 
}

std::list<Text_item*>*
Text_model::get_children_text_items()
{
  std::list<Text_item*>* cl = NULL;
  Text_item*             ti;

  if (get_num_children() != 0)
    {
      cl = new std::list<Text_item*>();
      for(std::list<Text_model*>::iterator i = children.begin(); 
	  i != children.end(); i++)
	{
	  ti = (*i)->get_first_view();
	  cl->push_back(ti);
	}
    }

  return cl;
}

int
Text_model::get_cell_span()
{
  int nc = get_num_children();
  int cs = 0;

  if (nc == 0)
    return 1;
  else
    {
      for(std::list<Text_model*>::iterator i = children.begin(); 
	  i != children.end(); i++)
	{
	  cs +=  (*i)->get_cell_span();
	}
      if (cs%2 == 0)
	cs++;

      /*std::cout << "\tget_cell_span[" << *this << "] es: " 
	<< cs << std::endl;*/

      return cs;
    }
}

void
Text_model::to_xml(xmlNodePtr xml_parent)
{
  std::list<Text_item*>* cl = get_children_text_items();
  xmlNodePtr             newxml;
  //xmlNodePtr             root;
  const xmlChar*         t; 
  const xmlChar*         s;
  const xmlChar*         idx;
  gchar*                 utf8_str;
  gsize                  br, bw;

  //std::cerr << "TO_XML: TAG(" << tag << ") TXT(" << text << ")\n";

  //root = xmlDocGetRootElement(xml_parent->doc);

  //std::cerr << "titem_root= " << root << " xml_parent " 
  //    << xml_parent << std::endl;

  //utf8_str = g_locale_to_utf8(tag.c_str(), -1, &br, &bw, NULL);
  utf8_str = const_cast<gchar*>(tag.c_str());
  t = reinterpret_cast<const xmlChar*>(utf8_str);
  //utf8_str = g_locale_to_utf8(text.c_str(), -1, &br, &bw, NULL);
  utf8_str = const_cast<gchar*>(text.c_str());
  s = reinterpret_cast<const xmlChar*>(utf8_str);
  //utf8_str = g_locale_to_utf8(index.c_str(), -1, &br, &bw, NULL);
  utf8_str = const_cast<gchar*>(index.c_str());
  idx = reinterpret_cast<const xmlChar*>(utf8_str);

  if (is_leaf())
    {
      newxml = xmlNewTextChild(xml_parent, NULL, t, s);
      //Propiedad INDICE
      utf8_str = g_locale_to_utf8("index", 5, &br, &bw, NULL);
      xmlSetProp(newxml, reinterpret_cast<const xmlChar*>(utf8_str),
		 reinterpret_cast<const xmlChar*>(idx));
    }
  else
    {
      newxml = xmlNewTextChild(xml_parent, NULL, t, NULL);
      //Propiedad INDICE
      utf8_str = g_locale_to_utf8("index", 5, &br, &bw, NULL);
      xmlSetProp(newxml, reinterpret_cast<const xmlChar*>(utf8_str),
		 reinterpret_cast<const xmlChar*>(idx));
      //newxml = xmlNewTextChild(xml_parent, NULL, t, s);

      //to_xml a cada uno de sus hijos...
      //std::cout << "TEXT_MODEL::to_xml a cada uno de sus hijos...\n";
      if (cl)
	{
	  //Antes de guardarla ordeno sus nodos por coordenada 'x'...
	  cl->sort(Canvas_item::citem_less);
	  //La frase crea un nodo xml para cada canvas_item suyo
	  for (std::list<Text_item*>::iterator i = cl->begin();
	       i != cl->end(); i++)
	    {
	      //std::cerr << "to_xml: " << *i << std::endl;
	      (*i)->get_model()->to_xml(newxml);
	    }

	  delete cl;
	}
    }
}

void 
Text_model::to_qtree(std::ostream& os)
{
  std::string t = text;

  if (text == get_empty_set())
    t = "\\phi";

  if (has_index())
    {
      os << t << "_{" << index << "} ";
    }
  else
    os << t << " ";
}

bool
Text_model::is_adjoint()
{
  int count = 0;

  for(std::list<Text_model*>::iterator i = children.begin(); 
      (i != children.end()) and (count < 2); i++)
    {
      if ((*i)->text == text)
	{
	  count++;
	  adjoint_son = *i;
	}
    }

  if (count != 1)
    {
      adjoint_son = NULL;
      return false;
    }
  else
    return true;
}

bool
Text_model::can_create_trace(Text_model* orig)
{
  bool result = false;  //Indica si se puede crear la traza o no.

  //std::cout << "TRAZA:\tDestino [" << *this << "] Origen ["
  //	    << *orig << "]\n";

  //1 respecto al destino.
  result = ((get_num_children() == 1) and 
	    (not get_first_child()->can_have_sons()) and
	    (label == ""));
  //2 respecto al origen
  result = (result and 
	    (orig->get_num_children() == 0) and
	    (orig->get_reference() == ""));

  return result;
}

bool
Text_model::create_trace(Text_model* orig)
{
  bool        result;  //Indica si se puede crear la traza o no.
  std::string label;
  Text_item*  ti;
  Text_model* tm;

  result = can_create_trace(orig);
  
  if (result)
    {
      //std::cout << "Puedes crear TRAZA.\n";
      if (the_sentence)
	{
#if 0
	  // Ajuste de 'label' y referencia a ella
	  label = the_sentence->get_next_label_str();
	  set_label(label);
	  /*orig->set_reference(label);
	    orig->set_trace_destination(this);*/
	  
	  // Creacion del hijo '$t$' en 'orig'.
	  ti = the_sentence->add_text_item("t", orig);
	  tm = ti->get_model();
	  tm->set_reference(label);
	  tm->set_trace_destination(this);
	  ti->set_colors(); //Para q aplique el color al ser una traza
#endif
	  the_sentence->add_trace(this, orig);
	}
      else
	std::cout << "No deberia ocurrir: the_sentence == NULL\n";
    }
  else
    {
      //std::cout << "NO puedes crear TRAZA.\n";
    }

  return result;
}
