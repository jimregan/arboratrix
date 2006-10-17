// -*- C++ -*-

#ifndef _UTILS_H_
#define _UTILS_H_

#ifdef HAVE_CONFIG_H
#include <sishitra_config.h>
#endif

//#include <iostream>
#include <sstream>
#include <string>

/*
 * Clase:      Utils
 * Hereda:     De nadie, es clase base.
 * Version:    1.0
 * Creada:     Mon Jul 21 15:03:26 CEST 2003
 * Modificada: Mon Jul 21 15:03:26 CEST 2003
 * Autor:      Antonio-Miguel Corbi Bellot
 * Email:      acorbi@hal9000
 * Proposito:  Sirve de agrupador de utilidades diversas.
 */

/*!
 * \class Utils
 * \brief Sirve de agrupador de utilidades diversas.
 */
class Utils
{
  // Declaraciones friend --si las hubiera--.
  
  // Parte publica.
public:

  //! Constructor por defecto.
  Utils();
  //! Destructor.
  ~Utils();

  //-------------- UTILS ----------------

  //! Convierte un entero en una cadena.
  static std::string itos(int i);
  //! Convierte un entero largo sin signo en una cadena.
  static std::string ultos(ulong i);

  // Parte protegida.
protected:

  // Parte privada.
private:

};

#endif
