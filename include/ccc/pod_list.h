/**
 *
 * @file This file contains the PODList container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_POD_LIST_H_
#define CCC_POD_LIST_H_

#include <ccc/compat.h>
#include <ccc/iterator.h>
#include <ccc/pod_vector.h>

namespace ccc
{

#pragma pack(push, 16)

template <class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8, bool Uninitialized = false, bool Runtime = false>
struct PodList
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef size_type node_index_type;

    struct ListNode
    {
        node_index_type m_Prev;
        node_index_type m_Next;
    };

    typedef ListNode node_type;

    typedef PodVector<node_index_type, size_type, Capacity + 1, Alignment, false, false, Runtime> deallocated_storage_type;
    typedef typename Storage<node_type, size_type, Capacity + 1, Alignment, Uninitialized, Runtime>::type nodes_storage_type;
    typedef typename Storage<value_type, size_type, Capacity, Alignment, Uninitialized, Runtime>::type values_storage_type;

#if (CCC_ALIGNAS_AVAILABLE)
    alignas(Alignment) size_type m_Size;
#elif CCC_ALIGNED_AVAILABLE
    typename ccc::Aligned<size_type, Alignment>::type m_Size;
#else
    PaddedValue<size_type, Alignment> m_Size;
#endif
    nodes_storage_type m_Nodes;
    values_storage_type m_Values;
    deallocated_storage_type m_Deallocated;
    static const node_index_type m_Anchor = 0;

    template <class T_>
    struct BidirectionalIterator
    {
        typedef T_ value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef BidirectionalIterator<T_> iterator_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        PodList* m_Container;
        node_index_type m_Node;

        BidirectionalIterator()
                : m_Container(), m_Node()
        {
        }

        explicit BidirectionalIterator(PodList* Container, node_index_type Node)
                : m_Container(Container), m_Node(Node)
        {
        }

        reference operator*() const
        {
            return m_Container->m_Values[m_Node - 1];
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
    };

    template <class T_>
    struct BidirectionalConstIterator
    {
        typedef T_ value_type;
        typedef const value_type* pointer;
        typedef const value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef BidirectionalConstIterator<T_> iterator_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        const PodList* m_Container;
        node_index_type m_Node;

        BidirectionalConstIterator()
                : m_Container(), m_Node()
        {
        }

        explicit BidirectionalConstIterator(const PodList* Container, node_index_type Node)
                : m_Container(Container), m_Node(Node)
        {
        }

        BidirectionalConstIterator(const BidirectionalIterator<T_>& NonConst)
                : m_Container(NonConst.m_Container), m_Node(NonConst.m_Node)
        {
        }

        reference operator*() const
        {
            return m_Container->m_Values[m_Node - 1];
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
    };

    typedef BidirectionalIterator<value_type> iterator;
    typedef BidirectionalConstIterator<value_type> const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // Assign:

    void assign(size_type Count, const value_type& Value)
    {
        clear();
        for (size_type i = 0; i < Count; ++i)
        {
            push_back(Value);
        }
    }

    template <typename IteratorType>
    void assign(IteratorType First, IteratorType Last)
    {
        clear();
        for (; First != Last; ++First)
        {
            push_back(*First);
        }
    }

    // Iterators:

    iterator begin() CCC_NOEXCEPT
    {
        return iterator(this, m_Nodes[m_Anchor].m_Next);
    }

    const_iterator begin() const CCC_NOEXCEPT
    {
        return const_iterator(this, m_Nodes[m_Anchor].m_Next);
    }

    iterator end() CCC_NOEXCEPT
    {
        return iterator(this, m_Anchor);
    }

    const_iterator end() const CCC_NOEXCEPT
    {
        return const_iterator(this, m_Anchor);
    }

    reverse_iterator rbegin() CCC_NOEXCEPT
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const CCC_NOEXCEPT
    {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() CCC_NOEXCEPT
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const CCC_NOEXCEPT
    {
        return const_reverse_iterator(begin());
    }

    CCC_CONSTEXPR
    size_type size() const CCC_NOEXCEPT
    {
        return m_Size;
    }

    CCC_CONSTEXPR
    size_type max_size() const CCC_NOEXCEPT
    {
        return m_Values.max_size();
    }

    CCC_CONSTEXPR
    bool empty() const CCC_NOEXCEPT
    {
        return 0 == size();
    }

    reference front()
    {
        return *begin();
    }

    const_reference front() const
    {
        return *begin();
    }

    reference back()
    {
        return *(prev(end()));
    }

    const_reference back() const
    {
        return *(prev(end()));
    }

    // Assumes there are free nodes
    node_index_type _private_allocate_node()
    {
        node_index_type Allocated;
        if (not m_Deallocated.empty())
        {
            // Prefer a node, that was already used
            Allocated = m_Deallocated.back();
            m_Deallocated.pop_back();
        }
        else
        {
            Allocated = m_Size + 1;
        }
        m_Size = m_Size + 1;
        return Allocated;
    }

    void _private_deallocate_node(node_index_type Node)
    {
        m_Deallocated.push_back(Node);
        m_Size = m_Size - 1;
    }

    void clear() CCC_NOEXCEPT
    {
        while (not empty())
        {
            pop_front();
        }
        m_Deallocated.clear();
    }

    void push_front(const_reference Value)
    {
        if (size() < max_size())
        {
            node_index_type NewFront = _private_allocate_node();
            m_Nodes.construct_default(&m_Nodes[NewFront]);
            m_Values.construct_and_assign(&m_Values[NewFront - 1], Value);
//            m_Values[NewFront - 1] = Value;
            node_index_type OldFront = m_Nodes[m_Anchor].m_Next;
            m_Nodes[NewFront].m_Prev = m_Anchor;
            m_Nodes[NewFront].m_Next = OldFront;
            m_Nodes[OldFront].m_Prev = NewFront;
            m_Nodes[m_Anchor].m_Next = NewFront;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void push_back(const_reference Value)
    {
        if (size() < max_size())
        {
            node_index_type NewBack = _private_allocate_node();
            m_Nodes.construct_default(&m_Nodes[NewBack]);
            m_Values.construct_and_assign(&m_Values[NewBack - 1], Value);
//            m_Values[NewBack - 1] = Value;
            node_index_type OldBack = m_Nodes[m_Anchor].m_Prev;
            m_Nodes[NewBack].m_Prev = OldBack;
            m_Nodes[NewBack].m_Next = m_Anchor;
            m_Nodes[m_Anchor].m_Prev = NewBack;
            m_Nodes[OldBack].m_Next = NewBack;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void emplace_front()
    {
        if (size() < max_size())
        {
            node_index_type NewFront = _private_allocate_node();
//            m_Values[NewFront - 1] = value_type(); // ToDo: necessary?
            m_Nodes.construct_default(&m_Nodes[NewFront]);
            m_Values.construct_default(&m_Values[NewFront - 1]);
            node_index_type OldFront = m_Nodes[m_Anchor].m_Next;
            m_Nodes[NewFront].m_Prev = m_Anchor;
            m_Nodes[NewFront].m_Next = OldFront;
            m_Nodes[OldFront].m_Prev = NewFront;
            m_Nodes[m_Anchor].m_Next = NewFront;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void emplace_back()
    {
        if (size() < max_size())
        {
            node_index_type NewBack = _private_allocate_node();
//            m_Values[NewBack - 1] = value_type(); // ToDo: necessary?
            m_Nodes.construct_default(&m_Nodes[NewBack]);
            m_Values.construct_default(&m_Values[NewBack - 1]);
            node_index_type OldBack = m_Nodes[m_Anchor].m_Prev;
            m_Nodes[NewBack].m_Prev = OldBack;
            m_Nodes[NewBack].m_Next = m_Anchor;
            m_Nodes[m_Anchor].m_Prev = NewBack;
            m_Nodes[OldBack].m_Next = NewBack;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void pop_front()
    {
        if (not empty())
        {
            node_index_type OldFront = m_Nodes[m_Anchor].m_Next;
//            m_Values[OldFront - 1] = value_type(); // destroy element (ToDo: necessary?)
            node_index_type NewFront = m_Nodes[OldFront].m_Next;
            m_Nodes[NewFront].m_Prev = m_Anchor;
            m_Nodes[m_Anchor].m_Next = NewFront;
//            m_Nodes[OldFront] = node_type(); // reinitialize node (not necessary, but maybe helpful for debugging)
            m_Nodes.destroy(&m_Nodes[OldFront]);
            m_Values.destroy(&m_Values[OldFront - 1]);
            _private_deallocate_node(OldFront);
        }
    }

    void pop_back()
    {
        if (not empty())
        {
            node_index_type OldBack = m_Nodes[m_Anchor].m_Prev;
//            m_Values[OldBack - 1] = value_type(); // destroy element (ToDo: necessary?)
            node_index_type NewBack = m_Nodes[OldBack].m_Prev;
            m_Nodes[NewBack].m_Next = m_Anchor;
            m_Nodes[m_Anchor].m_Prev = NewBack;
//            m_Nodes[OldBack] = node_type(); // reinitialize node (not necessary, but maybe helpful for debugging)
            m_Nodes.destroy(&m_Nodes[OldBack]);
            m_Values.destroy(&m_Values[OldBack - 1]);
            _private_deallocate_node(OldBack);
        }
    }

    iterator insert(iterator Position, const_reference Value)
    {
        if (size() < max_size())
        {
            node_index_type Behind = Position.m_Node;
            node_index_type InFront = m_Nodes[Behind].m_Prev;
            node_index_type Inserted = _private_allocate_node();
            m_Nodes.construct_default(&m_Nodes[Inserted]);
            m_Values.construct_and_assign(&m_Values[Inserted - 1], Value);
//            m_Values[Inserted - 1] = Value;
            m_Nodes[InFront].m_Next = Inserted;
            m_Nodes[Behind].m_Prev = Inserted;
            m_Nodes[Inserted].m_Prev = InFront;
            m_Nodes[Inserted].m_Next = Behind;
            return iterator(this, Inserted);
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    template <typename IteratorType>
    iterator insert(iterator Position, IteratorType First, IteratorType Last)
    {
        if (First == Last)
        {
            return Position;
        }
        else if (std::distance(First, Last) <= static_cast<difference_type>(max_size() - size()))
        {
            iterator Result = insert(Position, *First);
            ++First;
            for (; First != Last; ++First)
            {
                insert(Position, *First);
            }
            return Result;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    iterator insert(iterator Position, size_type Count, const value_type& Value)
    {
        if (Count == 0)
        {
            return Position;
        }
        else if (Count <= max_size() - size())
        {
            iterator Result = insert(Position, Value);
            for (size_type i = 1; i < Count; ++i)
            {
                insert(Position, Value);
            }
            return Result;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    iterator emplace(iterator Position)
    {
        if (size() < max_size())
        {
            node_index_type Behind = Position.m_Node;
            node_index_type InFront = m_Nodes[Behind].m_Prev;
            node_index_type Inserted = _private_allocate_node();
//            m_Values[Inserted - 1] = value_type(); // ToDo: necessary?
            m_Nodes.construct_default(&m_Nodes[Inserted]);
            m_Values.construct_default(&m_Values[Inserted - 1]);
            m_Nodes[InFront].m_Next = Inserted;
            m_Nodes[Behind].m_Prev = Inserted;
            m_Nodes[Inserted].m_Prev = InFront;
            m_Nodes[Inserted].m_Next = Behind;
            return iterator(this, Inserted);
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    /**
     *
     * Note: According to the standard, passing an invalid or non-dereferencable iterator causes undefinied behavior.
     */
    iterator erase(iterator Position)
    {
        node_index_type Erase = Position.m_Node;
        node_index_type Behind = m_Nodes[Erase].m_Next;
        node_index_type InFront = m_Nodes[Erase].m_Prev;
        m_Nodes[InFront].m_Next = Behind;
        m_Nodes[Behind].m_Prev = InFront;
//        m_Nodes[Erase] = node_type(); // reinitialize node (not necessary, but maybe helpful for debugging)
//        m_Values[Erase - 1] = value_type(); // destroy element (ToDo: necessary?)
        m_Nodes.destroy(&m_Nodes[Erase]);
        m_Values.destroy(&m_Values[Erase - 1]);
        _private_deallocate_node(Erase);
        return iterator(this, Behind);
    }

    iterator erase(iterator First, iterator Last)
    {
        while (First != Last)
        {
            First = erase(First);
        }
        return First;
    }

};

#pragma pack(pop)

}

#endif /* CCC_POD_LIST_H_ */
