/*
 * Depuracion de 'new', 'new[]', 'delete' y 'delete[]'
 * (c) Antonio M. Corbi Bellot, Abril 1997
 */

#include <stdlib.h>
#include <new>

void *
operator new(size_t n)
{
  return malloc(n);
}

void *
operator new[](size_t n)
{
  return malloc(n);
}

void
operator delete(void *cp)
{
  free(cp);
}

void
operator delete[](void *cp)
{
  free(cp);
}
