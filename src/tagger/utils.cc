// -*- C++ -*-

/*
 * Creado:     Mon Jul 21 15:03:26 CEST 2003
 * Modificado: Mon Jul 21 15:03:26 CEST 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 */

#include "utils.h"


/*
 * Constructor por defecto.
 */
Utils::Utils()
{

}

/*
 * Destructor.
 */
Utils::~Utils()
{

}

//! Convierte un numero en una cadena.
std::string
Utils::itos(int i)
{
  std::stringstream s;
  
  s << i;
  return s.str();
}

//! Convierte un numero en una cadena.
std::string
Utils::ultos(ulong i)
{
  std::stringstream s;
  
  s << i;
  return s.str();
}
