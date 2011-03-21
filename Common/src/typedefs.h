#ifndef BELOTE_COMMON_TYPEDEFS_H
#define BELOTE_COMMON_TYPEDEFS_H

#include <iterator>

#include <boost/array.hpp>

// Allows use of std::shared_ptr<> on statically allocated objects
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

// Output iterator for boost.array
template <class Container>
class BoostArrayOutputIterator :
	public std::iterator<std::output_iterator_tag,void,void,void,void>
{
protected:
	Container* container;
	typename Container::iterator it;

public:
	typedef Container container_type;

	explicit BoostArrayOutputIterator (Container& x, typename Container::iterator startIter)
		: container(&x), it(startIter)
	{}
	BoostArrayOutputIterator(const BoostArrayOutputIterator &other) : container(other.container), it(other.it) {}

	BoostArrayOutputIterator<Container>& operator= (typename Container::const_reference value)
	{
		assert(it != container->end());
		*it = value;
		++it;
		return *this;
	}

	BoostArrayOutputIterator<Container>& operator*()	{ return *this; }
	BoostArrayOutputIterator<Container>& operator++()	{ return *this; }
	BoostArrayOutputIterator<Container> operator++(int)	{ return *this; }
};

namespace std
{
	template<class Container>
	struct _Is_checked_helper<BoostArrayOutputIterator<Container> > : public tr1::true_type
	{	// mark BoostArrayOutputIterator as checked
	};
}

template<class Container> inline
BoostArrayOutputIterator<Container> boost_array_iterator(Container& array)
{
	return BoostArrayOutputIterator<Container>(array, array.begin());
}

template<class Container> inline
BoostArrayOutputIterator<Container> boost_array_iterator(Container& array, typename Container::iterator startIter)
{
	return BoostArrayOutputIterator<Container>(array, startIter);
}

#endif
