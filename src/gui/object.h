// -*- C++ -*-

#ifndef _OBJECT_H_
#define _OBJECT_H_

namespace acb_gui 
{
  /*
   * Clase:      Object
   * Hereda:     De nadie, es clase base.
   * Version:    1.0
   * Creada:     Thu Jan 31 16:30:40 CET 2002
   * Modificada: Thu Jan 31 16:30:40 CET 2002
   * Autor:      Antonio-M. Corbi Bellot
   * Email:      acorbi@hal9000
   * Proposito:  Clase base de todas.
   */

  /*!
   * \class Object
   * \brief Clase base de todas.
   */
  class Object
  {
    // Declaraciones friend --si las hubiera--.
  
    // Parte publica.
  public:

    //! Constructor por defecto.
    Object();

    //! Destructor.
    virtual ~Object();

    // Parte protegida.
  protected:

    // Parte privada.
  private:

  };

} // Fin namespace gui
#endif
