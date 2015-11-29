/*
 * pod_linear_map.h
 *
 *  Created on: Aug 27, 2015
 *      Author: dierkes
 */

#ifndef INCLUDE_CCC_POD_LINEAR_MAP_H_
#define INCLUDE_CCC_POD_LINEAR_MAP_H_

#include <utility>

#include <ccc/pod_array.h>
#include <map>

namespace ccc
{

#pragma pack(push, 16)

template<typename KeyType, typename T, KeyType Capacity, KeyType Offset = 0,
        std::size_t Alignment = 8, bool StaticStorage = true>
struct PodLinearMap
{
    typedef KeyType key_type;
    typedef T mapped_type;
    typedef std::pair<const KeyType, T> value_type;
    typedef KeyType size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;

    struct BidirectionalIterator
    {
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef BidirectionalIterator iterator_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        bool* m_Valid;
        mapped_type* m_Value;

        BidirectionalIterator(bool* Valid, mapped_type* Value)
                : m_Valid(Valid), m_Value(Value)
        {
        }

        reference operator*() const
        {
            return *m_Value;
        }

        pointer operator->() const
        {
            return ccc::addressof<pointer>(m_Container->m_Values[m_Node - 1]);
        }

        iterator_type& operator++()
        {
            m_Node = m_Container->m_Nodes[m_Node].m_Next;
            return *this;
        }

        iterator_type operator++(int)
        {
            iterator_type tmp = *this;
            m_Node = m_Container->m_Nodes[m_Node].m_Next;
            return tmp;
        }

        iterator_type&
        operator--()
        {
            m_Node = m_Container->m_Nodes[m_Node].m_Prev;
            return *this;
        }

        iterator_type operator--(int)
        {
            iterator_type tmp = *this;
            m_Node = m_Container->m_Nodes[m_Node].m_Prev;
            return tmp;
        }

        bool operator==(const iterator_type& rhs) const
        {
            return (m_Container == rhs.m_Container) and (m_Node == rhs.m_Node);
        }

        bool operator!=(const iterator_type& rhs) const
        {
            return (m_Container != rhs.m_Container) or (m_Node != rhs.m_Node);
        }

        void operator++()
        {
            ++m_Valid;
            ++m_Value;
        }
    };

    struct ConstBidirectionalIterator
    {

    };

    typedef BidirectionalIterator iterator;
    typedef ConstBidirectionalIterator const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    PODArray<MappedType, std::size_t, static_cast<std::size_t>(Capacity - Offset), Alignment, false,
            StaticStorage> m_Values;
    PODArray<bool, std::size_t, static_cast<std::size_t>(Capacity - Offset), Alignment, false,
            StaticStorage> m_Valid;

    bool has_key(KeyType Key)
    {
        return m_Valid[Key - Offset];
    }

    value_type insert(const value_type& Value);

};

#pragma pack(pop)

}

#endif /* INCLUDE_CCC_POD_LINEAR_MAP_H_ */
