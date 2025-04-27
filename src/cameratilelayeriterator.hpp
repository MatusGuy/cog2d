#ifndef CAMERATILELAYERITERATOR_HPP
#define CAMERATILELAYERITERATOR_HPP

#include "types.hpp"

COG2D_NAMESPACE_BEGIN_DECL

class TileLayer;

struct CameraTileLayerIterator
{
	using iterator_category = std::random_access_iterator_tag;
	using difference_type = TileIds::difference_type;
	using value_type = TileIds::value_type;
	using pointer = TileIds::pointer;
	using reference = TileIds::reference;

public:
	CameraTileLayerIterator(TileLayer& layer, TileIds::iterator start);

	reference operator*() const { return *m_it; }

	CameraTileLayerIterator advance(difference_type n);
	difference_type layer_index();

	auto& operator++()
	{
		advance(1);
		return *this;
	}

	auto operator++(int)
	{
		auto tmp = *this;
		++(*this);
		return tmp;
	}

	auto& operator--()
	{
		advance(-1);
		return *this;
	}

	auto operator--(int)
	{
		auto tmp = *this;
		--(*this);
		return tmp;
	}

	auto& operator+=(difference_type n)
	{
		advance(n);
		return *this;
	}
	auto& operator-=(difference_type n)
	{
		advance(-n);
		return *this;
	}

	friend bool operator==(const CameraTileLayerIterator& a, const CameraTileLayerIterator& b)
	{
		return a.m_it == b.m_it;
	}
	friend bool operator!=(const CameraTileLayerIterator& a, const CameraTileLayerIterator& b)
	{
		return a.m_it != b.m_it;
	}
	friend bool operator<(const CameraTileLayerIterator& a, const CameraTileLayerIterator& b)
	{
		return a.m_it < b.m_it;
	}
	friend bool operator>(const CameraTileLayerIterator& a, const CameraTileLayerIterator& b)
	{
		return a.m_it > b.m_it;
	}
	friend bool operator<=(const CameraTileLayerIterator& a, const CameraTileLayerIterator& b)
	{
		return a.m_it <= b.m_it;
	}
	friend bool operator>=(const CameraTileLayerIterator& a, const CameraTileLayerIterator& b)
	{
		return a.m_it >= b.m_it;
	}

	friend auto operator+(const CameraTileLayerIterator& it, difference_type n)
	{
		auto temp = it;
		temp += n;
		return temp;
	}
	friend auto operator+(difference_type n, const CameraTileLayerIterator& it) { return it + n; }
	friend auto operator-(const CameraTileLayerIterator& it, difference_type n)
	{
		auto temp = it;
		temp -= n;
		return temp;
	}
	friend difference_type operator-(const CameraTileLayerIterator& lhs,
	                                 const CameraTileLayerIterator& rhs)
	{
		// I don't think this is implemented properly... Eh.
		return std::distance(lhs.m_it, rhs.m_it);
	}

private:
	TileLayer& m_layer;
	TileIds::iterator m_it;
};

COG2D_NAMESPACE_END_DECL

#endif  // CAMERATILELAYERITERATOR_HPP
