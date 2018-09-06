// This file is part of slimesh, a C++ mesh library
//
// Copyright (C) 2018 Sebastian Reiter <s.b.reiter@gmail.com>

#ifndef __H__slimesh_data_buffer
#define __H__slimesh_data_buffer

#include <iterator>
#include <memory>
#include <vector>
#include "unpack.h"
#include "types.h"
#include "annex.h"
#include "custom_exception.h"

namespace slimesh {

DECLARE_CUSTOM_EXCEPTION (BadTupleSizeError, AnnexError);

template <class T>
class ArrayAnnex : public Annex {
public:
	using value_type = T;
	using value_t = value_type;
	using size_type = index_t;
	using iterator = typename std::vector<T>::iterator;
	using const_iterator = typename std::vector<T>::const_iterator;

	ArrayAnnex ()	: m_tupleSize (1) {}
	ArrayAnnex (const index_t tupleSize) : m_tupleSize (tupleSize) {}

	const char* class_name () const override	{return "ArrayAnnex";}

	inline void clear ()					{m_vector.clear();}

	bool empty() const { return m_vector.empty(); }

	/// total number of entries, counting individual components
	inline index_t size () const			{return static_cast<index_t>(m_vector.size());}

	inline index_t num_tuples () const		{return size() / tuple_size();}

	/// number of individual components making up a tuple
	inline index_t tuple_size () const				{return m_tupleSize;}
	inline void set_tuple_size (const index_t ts)	{m_tupleSize = ts;}

	inline T* raw_ptr() { if (size()) return &m_vector[0]; return NULL; }
	inline const T* raw_ptr () const { if (size()) return &m_vector[0]; return NULL; }

	inline void resize (const index_t s)				{m_vector.resize (s);}
	inline void resize (const index_t s, const T& v)	{m_vector.resize (s, v);}
	inline void reserve (const index_t s)				{m_vector.reserve (s);}

	inline void push_back (const T& v)					{m_vector.push_back (v);}

	inline T& operator [] (const index_t i)				{return m_vector[i];}
	inline const T& operator [] (const index_t i) const	{return m_vector[i];}

	inline T& at (const index_t i)					{return m_vector.at(i);}
	inline const T& at (const index_t i) const		{return m_vector.at(i);}

	inline T& back ()						{return m_vector.back();}
	inline const T& back () const			{return m_vector.back();}

	inline iterator begin ()				{return m_vector.begin();}
	inline iterator end ()					{return m_vector.end();}
	inline const_iterator begin () const	{return m_vector.begin();}
	inline const_iterator end () const		{return m_vector.end();}

private:
	std::vector <T>	m_vector;
	index_t			m_tupleSize;
};


using RealArrayAnnex		= ArrayAnnex <real_t>;
using IndexArrayAnnex		= ArrayAnnex <index_t>;

using SPRealArrayAnnex		= std::shared_ptr <RealArrayAnnex>;
using SPIndexArrayAnnex		= std::shared_ptr <IndexArrayAnnex>;
using CSPRealArrayAnnex		= std::shared_ptr <const RealArrayAnnex>;
using CSPIndexArrayAnnex	= std::shared_ptr <const IndexArrayAnnex>;

}//	end of namespace slimesh

#endif	//__H__slimesh_data_buffer
