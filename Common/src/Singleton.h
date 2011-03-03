#ifndef BELOTE_SINGLETON_H
#define BELOTE_SINGLETON_H

#include <assert.h>

template <typename T>
class Singleton
{
protected:
    static T* ms_Singleton;

public:
    Singleton( void )
    {
        assert( !ms_Singleton );
        ms_Singleton = static_cast<T*>(this);
    }
   ~Singleton( void )
        {  assert( ms_Singleton );  ms_Singleton = 0;  }
    static T& getSingleton( void )
        {  assert( ms_Singleton );  return ( *ms_Singleton );  }
    static T* getSingletonPtr( void )
        {  return ( ms_Singleton );  }

private:
    Singleton& operator=(const Singleton&) { return this; }
    Singleton(const Singleton&) {}
};

#endif
