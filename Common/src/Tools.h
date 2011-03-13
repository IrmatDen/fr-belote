#ifndef COMMON_TOOLS_H
#define COMMON_TOOLS_H

template <typename T, size_t N>
char ( &_ArraySizeHelper( T (&array)[N] ))[N];
#define countof( array ) (sizeof( _ArraySizeHelper( array ) ))

#endif
