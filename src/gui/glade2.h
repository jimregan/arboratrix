#ifndef _GLADE2_H_
#define _GLADE2_H_

// Macros necesarias para usar codigo generado por glade2.

// Necesario con glade-2 -- Inicio
#define GLADE_HOOKUP_OBJECT(component,widget,name) \
  g_object_set_data_full (G_OBJECT (component), name, \
    g_object_ref (widget), (GDestroyNotify) g_object_unref)

#define GLADE_HOOKUP_OBJECT_NO_REF(component,widget,name) \
  g_object_set_data (G_OBJECT (component), name, widget)
// Necesario con glade-2 -- Fin

#endif
