// -*- C++ -*-

/*
 * Creado:     Thu Mar  6 17:18:16 CET 2003
 * Modificado: Thu Mar  6 17:18:16 CET 2003
 * Autor:      Antonio-M. Corbi Bellot
 * Email:      acorbi@peurl.dlsi.ua.es
 */

#include "sentence.h"
#include <algorithm>
#include <sstream>
#include <text_item.h>
#include <tag_item.h>

/*
 * Constructor por defecto.
 */
Sentence::Sentence(Tagger_view* tv, const std::string& s, bool only_root) :
  sentence(s), tagger_view(tv)
{
  std::stringstream ss;
  std::string       w;
  Text_item*        tti;

  max_label    = static_cast<ulong>(0);
  tagger_model = tv->get_model();

  //0 Creamos el nodo raiz de esta frase, este no tiene padre.
  //root_node = new Tag_model("ROOT", NULL); // Sin texto ni vista asociados.
  //root_node->set_tag("root");
  root_node = NULL;
  create_root_node();

  if (sentence.length() > 0)
    {
      if (not only_root)
	{
	  //1º Separamos los simbolos de puntuacion
	  tokenize();

	  //2º 'Troceamos' la frase.
	  ss << sentence;
	  while (not ss.eof())
	    {
	      ss >> w;
#if 0
	      // Inicialmente el padre de todos los nodos es root_node
	      tm = new Text_model(w, root_node);
	      // Este item _no_ se auto-añade a la lista del canvas.
	      tti = new Text_item(*tm, tagger_view->get_canvas());
	      // Lo añadimos a la lista de items de la frase.
	      item_list.push_back(tti);
	      tti->set_selected(true);
#endif
	      tti = add_text_item(w, root_node, true);

	      // Situamos el item separado del anterior.
	      //--- tti->set_x(last_x);
	      //--- last_x += tti->get_w() + Text_item::get_pad_w();

	      //std::cout << "La altura del item[" << w << "] es: " << tm->get_level() << "\n";
	      //std::cout << "tiene por encima " << tm->get_up_levels() << " niveles.\n";
	    }

	  //layout_tree();
	  layout_tree_in_screen();
	  // Al principio la frase no esta 'terminada'.
	  finished = false;
	}
    }
}

void
Sentence::create_root_node()
{
  root_node = new Tag_model("ROOT", NULL); // Sin texto ni vista asociados.
  root_node->set_tag("root");
}

Text_item*
Sentence::add_text_item(const std::string& w, Text_model* parent, 
			int x, int y)
{
  Text_model* tm;
  Text_item*  tti;

  tm = new Text_model(w, parent);
  tm->set_sentence(this);

  // Este item _no_ se auto-añade a la lista del canvas.
  tti = new Text_item(*tm, tagger_view->get_canvas());
  tti->set_x(x); tti->set_y(y);
  // Lo añadimos a la lista de items de la frase.
  item_list.push_back(tti);

  //tti->set_selected(selected);
  return tti;
}

Tag_item*
Sentence::add_tag_item(const std::string& w, Tag_model* parent,
		       int x, int y)
{
  Tag_model* tm;
  Tag_item*  tti;

  tm = new Tag_model(w, parent);
  tm->set_sentence(this);

  // Este item _no_ se auto-añade a la lista del canvas.
  tti = new Tag_item(*tm, tagger_view->get_canvas());
  tti->set_x(x); tti->set_y(y);
  // Lo añadimos a la lista de items de la frase.
  item_list.push_back(tti);

  //tti->set_selected(selected);
  return tti;
}

Text_item*
Sentence::add_trace(Text_model* dest, Text_model* orig)
{
  std::string label = get_next_label_str();
  Text_item*  ti;
  Text_model* tm;

  dest->set_label(label);

  // Creacion del hijo '$t$' en 'orig'.
  ti = add_text_item("t", orig);
  tm = ti->get_model();
  tm->set_reference(label);
  tm->set_trace_destination(dest);
  ti->set_colors(); //Para q aplique el color al ser una traza

  return ti;
}

void
Sentence::create_adjoint(Text_item* txi, Tag_item* tgi)
{
  Tag_model*  tgm = tgi->get_model();
  Text_model* txm = txi->get_model();
  Tag_model*  old_parent = dynamic_cast<Tag_model*>(tgm->get_parent());
  Tag_item*   new_parent_item;
  Tag_model*  new_parent_model;
  std::string tag_txt = tgm->get_tag();
  
  new_parent_item = add_tag_item(tag_txt, old_parent);
  new_parent_model = new_parent_item->get_model();
  new_parent_item->set_x(tgi->get_x());

  //1) al nuevo padre le cuelgo el txt_model q origina el adjunto
  new_parent_model->add_child(txm);
  //2) El tag_model q crea el adjunto, lo quito del padre antiguo y lo
  //2) coloco en el nuevo
  new_parent_model->add_child(tgm);
}

void
Sentence::remove_item(Text_item* ti)
{
  item_list.remove(ti);
}

/*
 * Destructor.
 */
Sentence::~Sentence()
{
  Text_model* tm;

  for(Canvas::Itemlist::iterator i = item_list.begin(); 
      i != item_list.end(); i++)
    {
      // Liberamos el modelo de la palabra
      tm = dynamic_cast<Text_model*>((*i)->get_model());
      delete tm;
      // Y ahora la vista sobre el
      delete *i;
    }
  //std::cout << "Sentence: Destructor\n";
}

void
Sentence::tokenize()
{
  std::string            sep = ",;.:-_¿?!#%&^`'";
  std::string::size_type last = 0;
  int                    pos;

  //1) Busco y quito espacios en blanco al final
  pos = sentence.find_last_not_of(' ');
  //std::cout << "Ultimo no-blanco es [" << sentence[pos] << "] en " << pos << std::endl;
  sentence.erase(pos+1);

  // Buscamos los caracteres de puntuacion...
  while (last != std::string::npos)
    {
      last = sentence.find_first_of(sep, last);

      if (last != std::string::npos)
	{
	  if (last > 0) // Esta en posicion superior a la 1ª
	    {
	      // Inserto blanco antes
	      sentence.insert(last, " ");
	      last += 2;
	    }
	  else
	    last++;

	  // Siempre inserto blanco despues
	  sentence.insert(last, " ");
	}
    }
}

std::ostream&
operator<< (std::ostream& os, const Sentence& s)
{
  os << s.sentence;

  return os;
}

std::list<Text_item*>*
Sentence::get_text_item_list_of_level(int l)
{
  std::list<Text_item*>* til = NULL;
  Text_item*             ti;
  Text_model*            tm;

  if (l > 0) // Comenzamos en el nivel 1, la raiz ocupa el 0.
    {
      til = new std::list<Text_item*>();
      for(std::list<Canvas_item*>::iterator i = item_list.begin();
	  i != item_list.end(); i++)
	{
	  ti = dynamic_cast<Text_item*>(*i);
	  tm = dynamic_cast<Text_model*>(ti->get_model());
	  if (tm->get_level() == l) // Si este nodo es del nivel pedido: 'l'.
	    til->push_back(ti);
	}
    }
  return til;
}

std::list<Text_model*>*
Sentence::get_text_model_list_of_level(int l)
{
  std::list<Text_model*>* tml = NULL;
  Text_item*              ti;
  Text_model*             tm;

  if (l > 0) // Comenzamos en el nivel 1, la raiz ocupa el 0.
    {
      tml = new std::list<Text_model*>();
      for(std::list<Canvas_item*>::iterator i = item_list.begin();
	  i != item_list.end(); i++)
	{
	  ti = dynamic_cast<Text_item*>(*i);
	  tm = dynamic_cast<Text_model*>(ti->get_model());
	  if (tm->get_level() == l) // Si este nodo es del nivel pedido: 'l'.
	    tml->push_back(tm);
	}
    }
  return tml;
}

int
Sentence::get_level_real_nodes(int l)
{
  std::list<Text_item*>* til = NULL;
  int                    rn = 0;

  if (l == 0)
    return 1; // En el nivel 0 siempre esta la raiz.
  else
    {
      til = get_text_item_list_of_level(l);
      if (til)
	{
	  rn = til->size();
	  delete til;
	}
      return rn;
    }
}

int
Sentence::get_level_virtual_nodes(int l)
{
  if (l == 0)
    return 0; // Caso base, en la raiz no hay nodos virtuales.
  else
    return  get_tree_width() - get_level_real_nodes(l-1);
}

int
Sentence::get_max_text_height_of_level(int l)
{
  int                    max_height = 0;
  std::list<Text_item*>* til        = get_text_item_list_of_level(l);
  std::list<int>         hl; // list of heights
 
  if (til)
    {
      for(std::list<Text_item*>::iterator i = til->begin();
	  i != til->end(); i++)
	{
	  hl.push_back((*i)->get_h());
	}
      max_height = *std::max_element(hl.begin(), hl.end());
      delete til;
    }

  return max_height + (l*Text_item::get_pad_h());
}

int 
Sentence::get_tree_total_levels()
{
  int         l = 0;
  Text_item*  ti;
  Text_model* tm;
  int         al;

  for(std::list<Canvas_item*>::iterator i = item_list.begin();
      i != item_list.end(); i++)
    {
      ti = dynamic_cast<Text_item*>(*i);
      tm = dynamic_cast<Text_model*>(ti->get_model());
      al = tm->get_level();
      l = (al > l) ? al : l;
    }

  return l;
}

int
Sentence::get_tree_width()
{
  int         w = item_list.size();
  Text_item*  ti;
  Text_model* tm;

  if (w > 0)
    {
      for(std::list<Canvas_item*>::iterator i = item_list.begin();
	  i != item_list.end(); i++)
	{
	  ti = dynamic_cast<Text_item*>(*i);
	  tm = dynamic_cast<Text_model*>(ti->get_model());
	  if (tm->has_odd_children())
	    w--;
	}
    }
  return w;
}

void
Sentence::correct_negative_offset()
{
  std::list<Canvas_item*>::iterator min;

  min = min_element(item_list.begin(), item_list.end(), Canvas_item::citem_less);
  //std::cout << "X minima es: " << (*min)->get_x() << std::endl;
  if ((*min)->get_x() < 0)
    {
      int m = abs((*min)->get_x());

      for(std::list<Canvas_item*>::iterator i = item_list.begin(); 
	  i != item_list.end(); i++)
	{
	  (*i)->set_x((*i)->get_x() + m);
	}
    }
}

void
Sentence::correct_overlap()
{
  std::list<Text_item*>* pil = NULL;
  int                    th  = get_tree_height(); // Numero de niveles del arbol
  Text_item*             p1; //Parent-1
  Text_item*             p2; //Parent-2
  Text_item*             lc; //p1-Last child
  Text_item*             fc; //p2-First child
  int                    nxp;
  int                    ndxp;

  // Recorremos el arbol por niveles de abajo a arriba.
  for(int l = th; l > 1; l--)
    {
      // Obtengo la lista de text_items del nivel 'l-1', los 'padres'.
      pil = get_text_item_list_of_level(l-1);
      if (pil)
	{
	  /* Ordeno la lista por menor coordenada X (asi es como los ve
	   * el usuario).*/
	  pil->sort(Canvas_item::citem_less);
	  //Recorro todos los padres de nivel l.
	  for(std::list<Text_item*>::iterator i = pil->begin(); 
	      i != pil->end(); i++)
	    {
	      p1 = *i; //Primer padre
	      if (p1 != pil->back()) // Si hay mas a su derecha
		{
		  std::list<Text_item*>::iterator j = i;

		  p2 = *(++j);
		  /*std::cout << "P1[" << *p1 << "]\n";
		    std::cout << "P2[" << *p2 << "]\n";*/

		  lc = p1->get_last_child();
		  fc = p2->get_first_child();

		  
		  if ((lc) and (fc))
		    {
		      std::cout << "lc[" << *lc << "]\n";
		      std::cout << "fc[" << *fc << "]\n";

		      if (lc->intersects_with(fc)) //Si hay interseccion
			{
			  //std::cout << "Intersectan: " << *lc << " con: " << *fc << std::endl;
			  nxp = lc->get_x() + lc->get_w() + Text_item::get_pad_w();
			  ndxp = nxp - fc->get_x();
			  p2->offset_by_x_with_children(ndxp);
			}
		    }
		}
	    }
	  delete pil;
	}
    }
}

void
Sentence::layout_tree_in_matrix()
{
  int offset = 0;

  layout_children_in_matrix(root_node, offset);
}

void
Sentence::layout_children_in_matrix(Text_model* proot, int& offset)
{
  int                     d;  //delta respecto a la celda del padre.
  std::list<Text_model*>* tml;
  std::list<Text_item*>*  til = NULL;
  Text_item*              ti;
  Text_model*             tm;
  int                     nc; //Numero de hijos del padre
  int                     pc; //La celda del padre

  if (proot)
    {
      nc = proot->get_num_children();
      pc = proot->get_cell_in_span() + offset;
      proot->set_column(pc);

      switch (nc)
	{
	case 0:  //Sin hijos
	  offset++;
	  /*std::cout << "\t\nOffset[0]++: " << offset 
	    << " proot=" << *proot << std::endl;*/
	  break;

	case 1:  //Solo un hijo
	  tm = proot->get_first_child();
	  //tm->set_column(proot->get_column());
	  //std::cout << "LCM: 1 hijo\n";
	  //Recursion
	  layout_children_in_matrix(tm, offset);
	  //offset++;
	  break;

	default: //Mas de un hijo
	  //Obtengo las vistas sobre los hijos
	  til = proot->get_children_text_items();
	  // Y los ordeno de menor a mayor por su coordenada x actual.
	  til->sort(Canvas_item::citem_less);

	  //Asi construyo una lista de modelos ordenada por su 'x'.
	  tml = new std::list<Text_model*>();
	  for(std::list<Text_item*>::iterator i = til->begin();
	      i != til->end(); i++)
	    {
	      ti = dynamic_cast<Text_item*>(*i);
	      tm = dynamic_cast<Text_model*>(ti->get_model());
	      tml->push_back(tm);
	    }

	  d = 0; //Lleva la cuenta de los hijos 'tratados'.
	  //Recorremos los 'modelos' de los hijos
	  for(std::list<Text_model*>::iterator i = tml->begin(); 
	      i != tml->end(); i++)
	    {
	      tm = *i;
	      d++;

	      //Recursion
	      layout_children_in_matrix(tm, offset);

	      //Estando en el ultimo hijo no se incrementa el offset
	      if (d < nc)
		{
		  offset++;	
		}
	    }

	  delete til;
	  delete tml;
	}
    }
}

void
Sentence::layout_tree_in_screen()
{
  std::list<Text_item*>* til = NULL;
  Text_item*             ti;
  Text_model*            tm;
  int                    th  = get_tree_height(); // Niveles del arbol
  int                    w;
  int                    c;
  int                    nc;
  int                    offset;

  // Distribuyo los nodos del arbol en celdas.
  layout_tree_in_matrix();

  /* 1) Calculo la posicion x,y de cada nodo _SIN HIJOS_ en base a la
     celda q ocupa.  Lo hago recorriendo el arbol por niveles, desde
     el 1 al N. */
  for(int l = 1; l <= th; l++)
    {
      // Obtengo la lista de text_items del nivel 'l'.
      til = get_text_item_list_of_level(l);
      // Y los ordeno de menor a mayor por su coordenada x.
      til->sort(Canvas_item::citem_less);

      for(std::list<Text_item*>::iterator i = til->begin(); 
	  i != til->end(); i++)
	{
	  ti = *i;
	  tm = dynamic_cast<Text_model*>(ti->get_model());

	  nc = tm->get_num_children();
	  if (nc == 0) // Si no tiene hijos...ya se puede colocar
	    {
	      c = ti->get_column(); //Es la columna del item actual

	      /*std::cout << "\n--------------------[" 
		<< **i << "] C=" << c 
		<< " ---------------------\n"
		<< std::endl;*/

	      //Recorro todos los items buscando los q estan en col < c;
	      offset = 0;
	      for(int col = 1; col < c; col++)
		{
		  w = 0;
		  for(std::list<Canvas_item*>::iterator j = item_list.begin(); 
		      j != item_list.end(); j++)
		    {

		      /*std::cout << "Columna col=" << col << " item-col= " 
			<< dynamic_cast<Text_item*>(*j)->get_column() 
			<< std::endl;*/

		      /* Por cada columna anterior, me quedo con el
		       * item mas ancho */
		      if (dynamic_cast<Text_item*>(*j)->get_column() == col)
			{
			  if ((*j)->get_w() > w)
			    {
			      w = (*j)->get_w();
			      /*std::cout << "   [" 
				<< *dynamic_cast<Text_item*>(*j) 
				<< "] C=" << col 
				<< ", W="   << (*j)->get_w() 
				<< std::endl;*/
			    }
			  else
			    {
			      /*std::cout << "   NO ["
				<< *dynamic_cast<Text_item*>(*j)
				<< "]\n";*/
			    }
			}
		    }
		  offset += w;
		}
	      offset += (c-1) * Text_item::get_pad_w();
	      (*i)->set_x(offset);

	      if ((*i)->can_have_sons()) //no lo bajo al 'fondo'
		(*i)->calc_y_pos();
	      else
		(*i)->set_y(th * Text_item::get_pad_h());
#if 0 //a lo Mikel
	      //(*i)->calc_y_pos();
	      //Mikel lo prefiere asi:
	      (*i)->set_y(th * Text_item::get_pad_h());
#endif
	    }
	}
      if (til)
	delete til;
    }

  /* 2) Calculo la posicion x,y de cada nodo _CON HIJOS_ en base a la
     celda q ocupa.  Lo hago recorriendo el arbol por niveles de abajo
     a arriba, desde el N-1 al 1. */
  for(int l = th-1; l >= 1; l--)
    {
      // Obtengo la lista de text_items del nivel 'l'.
      til = get_text_item_list_of_level(l);
      // Y los ordeno de menor a mayor por su coordenada x.
      til->sort(Canvas_item::citem_less);

      for(std::list<Text_item*>::iterator i = til->begin(); 
	  i != til->end(); i++)
	{
	  ti = *i;
	  tm = dynamic_cast<Text_model*>(ti->get_model());
	  nc = tm->get_num_children();

	  if (nc > 0) //Tiene hijos
	    {
	      ti->calc_xy_pos();
	    }
	}
      if (til)
	delete til;
    }

  /* 3) Aplico (si procede) una compensacion por un offset negativo a
        todos los nodos del arbol.*/
  correct_negative_offset();
}

void 
Sentence::to_xml(xmlNodePtr parent)
{
  xmlNodePtr sroot; //El nodo xml_raiz de cada frase
  xmlChar*   s = reinterpret_cast<xmlChar*>(const_cast<char*>("false"));
  gchar*     utf8_str;
  gchar*     utf8_aux;
  gsize      br, bw;

  utf8_str = g_locale_to_utf8("sentence", 8, &br, &bw, NULL);
  sroot = xmlNewChild(parent, NULL, 
		      reinterpret_cast<const xmlChar *>(utf8_str), NULL);

  // PROPIEDADES del nodo
  // 1º, la frase original
  utf8_str = g_locale_to_utf8("text", 4, &br, &bw, NULL);
  //utf8_aux = g_locale_to_utf8(sentence.c_str(), -1, &br, &bw, NULL);
  utf8_aux = const_cast<gchar*>(sentence.c_str());
  xmlSetProp(sroot, 
	     reinterpret_cast<const xmlChar*>(utf8_str),
	     reinterpret_cast<const xmlChar*>(utf8_aux));

  // 2º, la marca de etiquetado finalizado.
  if (finished)
    {
      utf8_str = g_locale_to_utf8("true", 4, &br, &bw, NULL);
      s = reinterpret_cast<xmlChar*>(utf8_str);
    }
  else
    {
      utf8_str = g_locale_to_utf8("false", 5, &br, &bw, NULL);
      s = reinterpret_cast<xmlChar*>(utf8_str);
    }
  utf8_str = g_locale_to_utf8("finished", 8, &br, &bw, NULL);
  xmlSetProp(sroot, reinterpret_cast<const xmlChar*>(utf8_str),s);

  // 3ª, Comenzando en la raiz, cada nodo se representa en xml
  root_node->to_xml(sroot);
}

void 
Sentence::to_qtree(std::ostream& os)
{
  std::list<Text_item*>* til = root_node->get_children_text_items();
  Text_model*            m;

  if (til)
    {

      //std::cout << "Exportamos [" << *this << "]\n";

      //Antes de guardarla ordeno sus nodos por coordenada 'x'...
      til->sort(Canvas_item::citem_less);

      //El comienzo de una frase qtree...
      os << "\\Tree ";
      if (til->size() > 1)
	os << "[ ";

      //La frase crea un nodo qtree para cada canvas_item suyo
      for(std::list<Text_item*>::iterator i = til->begin();
	  i != til->end(); i++)
	{
	  m = (*i)->get_model();
	  //std::cout << "QT[" << *m << "]\n";
	  m->to_qtree(os);
	}

      if (til->size() > 1)
	os << " ]\n";

      delete til;
    }
}
