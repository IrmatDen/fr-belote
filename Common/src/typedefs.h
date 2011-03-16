#ifndef BELOTE_COMMON_TYPEDEFS_H
#define BELOTE_COMMON_TYPEDEFS_H

// Allows use of std::tr1::shared_ptr<> on statically allocated objects
struct null_deleter
{
    void operator()(void const *) const
    {
    }
};

namespace sf
{
	class TcpSocket;
	typedef std::shared_ptr<TcpSocket>	TcpSocketPtr;
}

#endif
