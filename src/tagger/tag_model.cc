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
 * Creado:     Thu May  8 19:07:28 CEST 2003
 * Modificado: Thu May  8 19:07:28 CEST 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@peurl.dlsi.ua.es
 */

#include "tag_model.h"
#include "tag_item.h"

/*
 * Constructor por defecto.
 */
Tag_model::Tag_model(const std::string& nt, Text_model* pparent):
  Text_model(nt, pparent)
{
  tag = nt;
  as_triangle = false;
}

/*
 * Destructor.
 */
Tag_model::~Tag_model()
{

}

void
Tag_model::to_xml(xmlNodePtr xml_parent)
{
  //xmlNodePtr             root;
  //const xmlChar*         t = reinterpret_cast<const xmlChar*>(tag.c_str());
  std::list<Text_item*>* cl = get_children_text_items();
  xmlNodePtr             newxml;
  xmlChar*               s;
  xmlChar*               idx;
  xmlChar*               node_name;
  gchar*                 utf8_str;
  gsize                  br, bw;

  //std::cerr << "TO_XML: TAG(" << tag << ") TXT(" << text << ")\n";

  //root = xmlDocGetRootElement(xml_parent->doc);

  //std::cerr << "titem_root= " << root << " xml_parent " 
  //    << xml_parent << std::endl;

  //utf8_str = g_locale_to_utf8(text.c_str(), -1, &br, &bw, NULL);
  utf8_str = const_cast<gchar*>(text.c_str());
  s = reinterpret_cast<xmlChar*>(utf8_str);
  //utf8_str = g_locale_to_utf8(index.c_str(), -1, &br, &bw, NULL);
  utf8_str = const_cast<gchar*>(index.c_str());
  idx = reinterpret_cast<xmlChar*>(utf8_str);

  if (as_triangle)
    {
      utf8_str = g_locale_to_utf8("impl", 4, &br, &bw, NULL);
      node_name = reinterpret_cast<xmlChar*>(utf8_str);
    }
  else
    {
      utf8_str = g_locale_to_utf8("phrase", 6, &br, &bw, NULL);
      node_name = reinterpret_cast<xmlChar*>(utf8_str);
    }

  if (is_leaf())
    {
      newxml = xmlNewTextChild(xml_parent, NULL, node_name, NULL);
      //reinterpret_cast<const xmlChar *>("phrase"), NULL);

      // PROPIEDADES del nodo
      // 1º, la clase
      utf8_str = g_locale_to_utf8("class", 5, &br, &bw, NULL);
      xmlSetProp(newxml, reinterpret_cast<const xmlChar*>(utf8_str),
		 reinterpret_cast<const xmlChar*>(s));
      // 2º, el indice
      if (not is_root())
	{
	  utf8_str = g_locale_to_utf8("index", 5, &br, &bw, NULL);
	  xmlSetProp(newxml, reinterpret_cast<const xmlChar*>(utf8_str),
		     reinterpret_cast<const xmlChar*>(idx));
	}
    }
  else
    {
      newxml = xmlNewTextChild(xml_parent, NULL, node_name, NULL);
      //reinterpret_cast<const xmlChar *>("phrase"), NULL);

      // PROPIEDADES del nodo
      // 1º, la clase
      utf8_str = g_locale_to_utf8("class", 5, &br, &bw, NULL);
      xmlSetProp(newxml, reinterpret_cast<const xmlChar*>(utf8_str),
		 reinterpret_cast<const xmlChar*>(s));
      // 2º, el indice
      if (not is_root())
	{
	  utf8_str = g_locale_to_utf8("index", 5, &br, &bw, NULL);
	  xmlSetProp(newxml, reinterpret_cast<const xmlChar*>(utf8_str),
		     reinterpret_cast<const xmlChar*>(idx));
	}
      //newxml = xmlNewTextChild(xml_parent, NULL, t, s);

      //to_xml a cada uno de sus hijos...
      //std::cout << "TAG_MODEL::to_xml a cada uno de sus hijos...\n";
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
Tag_model::to_qtree(std::ostream& os)
{
  std::list<Text_item*>* til = get_children_text_items();
  Text_model*            m;

  if (til)
    {
      if (not as_triangle)
	{
	  os << "[.{\\bf " << text;
	  if (has_index())
	    os << "_{" << index << "}";
	  os << "} ";
	}
      else
	os << "\\qroof { ";

      til->sort(Canvas_item::citem_less);
      // Cada hijo como nodo qtree...
      for(std::list<Text_item*>::iterator i = til->begin (); 
	  i != til->end (); i++)
	{
	  m = (*i)->get_model();
	  m->to_qtree(os);
	  os << "";
	}

      if (not as_triangle)
	{
	  os << " ] ";
	}
      else
	{
	  os << "}.{\\bf " << text;
	  if (has_index())
	    os << "_{" << index << "}";
	  os  << " } ";
	}
      delete til;
    }
  else //sanity-check qtree
    os << "[{\\bf " << text << "} ] ";
}
