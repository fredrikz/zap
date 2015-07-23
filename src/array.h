#include <stdlib.h>
#include <string.h>

#define ARRAY_STRUCT( TYPE )                  \
typedef struct Array##TYPE__                  \
{                                             \
  TYPE*         _data;                        \
  unsigned int  _size;                        \
  unsigned int  _capacity;                    \
} Array##TYPE;                                \



#define ARRAY_CREATE( TYPE )                  \
static inline void                            \
ArrayCreate##TYPE( Array##TYPE* arr )         \
{                                             \
  arr->_data = 0;                             \
  arr->_size = 0;                             \
  arr->_capacity = 0;                         \
}



#define ARRAY_DESTROY( TYPE )                 \
static inline void                            \
ArrayDestroy##TYPE( Array##TYPE* arr )        \
{                                             \
  if ( arr->_data )                           \
  {                                           \
    free( arr->_data );                       \
  }                                           \
  arr->_data = 0;                             \
  arr->_size = 0;                             \
  arr->_capacity = 0;                         \
}



#define ARRAY_SETCAPACITY( TYPE )             \
static inline void                            \
ArraySetCapacity##TYPE( Array##TYPE* arr, unsigned int size ) \
{                                             \
  if ( size )                                 \
  {                                           \
    arr->_data = (TYPE*)realloc( arr->_data, sizeof(TYPE) * size ); \
    arr->_capacity = size;                    \
  }                                           \
  else                                        \
  {                                           \
    ArrayDestroy##TYPE( arr );                \
  }                                           \
}                                             



#define ARRAY_SETSIZE( TYPE )                 \
static inline void                            \
ArraySetSize##TYPE( Array##TYPE* arr, unsigned int size ) \
{                                             \
  ArraySetCapacity##TYPE( arr, size );        \
  arr->_size = size;                          \
}                                             
                                             


#define ARRAY_ADDRANGE( TYPE )                \
static inline void                            \
ArrayAddRange##TYPE( Array##TYPE* arr, TYPE* elems, unsigned int count )\
{                                             \
  if ( arr->_size + count > arr->_capacity )  \
  {                                           \
    if ( arr->_capacity == 0 )                \
    {                                         \
      unsigned int num = count < 16 ? 16 : count; \
      ArraySetCapacity##TYPE( arr, num );     \
    }                                         \
    else                                      \
    {                                         \
      unsigned int num = arr->_size + count > arr->_size * 2 ? arr->_size + count : arr->_size * 2;\
      ArraySetCapacity##TYPE( arr, num );     \
    }                                         \
  }                                           \
  memcpy( arr->_data + arr->_size, elems, sizeof(TYPE) * count ); \
  arr->_size += count;                        \
}



#define ARRAY_ADD( TYPE )                     \
static inline void                            \
ArrayAdd##TYPE( Array##TYPE* arr, TYPE* elem )\
{                                             \
  ArrayAddRange##TYPE( arr, elem, 1 );        \
}




#define ARRAYTYPE( TYPE )   \
  ARRAY_STRUCT( TYPE )      \
  ARRAY_CREATE( TYPE )      \
  ARRAY_DESTROY( TYPE )     \
  ARRAY_SETCAPACITY( TYPE ) \
  ARRAY_SETSIZE( TYPE )     \
  ARRAY_ADDRANGE( TYPE )    \
  ARRAY_ADD( TYPE )         


