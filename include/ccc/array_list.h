/**
 *
 * @file This file contains the ArrayList container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_ARRAY_LIST_H_
#define CCC_ARRAY_LIST_H_

#include <ccc/compat.h>
#include <ccc/array_stack.h>
#include <ccc/iterator.h>

namespace ccc
{

template <class T, class SizeType, SizeType Capacity>
struct ArrayList
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef ArrayList<T, SizeType, Capacity> container_type; // necessary for iterator

    typedef size_type node_index_type;

    struct ListNode
    {
        node_index_type m_Prev;
        node_index_type m_Next;
    };

    typedef ListNode node_type;

    size_type m_Size;
    node_type m_Nodes[Capacity + 1];
    value_type m_Values[Capacity];
    ArrayStack<node_index_type, size_type, Capacity + 1> m_Deallocated;
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

        container_type* m_Container;
        node_index_type m_Node;

        BidirectionalIterator()
                : m_Container(), m_Node()
        {
        }

        explicit BidirectionalIterator(container_type* Container, node_index_type Node)
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

        const container_type* m_Container;
        node_index_type m_Node;

        BidirectionalConstIterator()
                : m_Container(), m_Node()
        {
        }

        explicit BidirectionalConstIterator(const container_type* Container, node_index_type Node)
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
        return Capacity;
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
        ++m_Size;
        return Allocated;
    }

    void _private_deallocate_node(node_index_type Node)
    {
        m_Deallocated.push_back(Node);
        --m_Size;
    }

    void clear() CCC_NOEXCEPT
    {
        while (not empty())
        {
            pop_front();
        }
    }

    void push_front(const_reference Value)
    {
        if (size() < Capacity)
        {
            node_index_type NewFront = _private_allocate_node();
            m_Values[NewFront - 1] = Value;
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
        if (size() < Capacity)
        {
            node_index_type NewBack = _private_allocate_node();
            m_Values[NewBack - 1] = Value;
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
        if (size() < Capacity)
        {
            node_index_type NewFront = _private_allocate_node();
            m_Values[NewFront - 1] = value_type(); // ToDo: necessary?
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
        if (size() < Capacity)
        {
            node_index_type NewBack = _private_allocate_node();
            m_Values[NewBack - 1] = value_type(); // ToDo: necessary?
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
            m_Values[OldFront - 1] = value_type(); // destroy element (ToDo: necessary?)
            node_index_type NewFront = m_Nodes[OldFront].m_Next;
            m_Nodes[NewFront].m_Prev = m_Anchor;
            m_Nodes[m_Anchor].m_Next = NewFront;
            m_Nodes[OldFront] = node_type(); // reinitialize node (not necessary, but maybe helpful for debugging)
            _private_deallocate_node(OldFront);
        }
    }

    void pop_back()
    {
        if (not empty())
        {
            node_index_type OldBack = m_Nodes[m_Anchor].m_Prev;
            m_Values[OldBack - 1] = value_type(); // destroy element (ToDo: necessary?)
            node_index_type NewBack = m_Nodes[OldBack].m_Prev;
            m_Nodes[NewBack].m_Next = m_Anchor;
            m_Nodes[m_Anchor].m_Prev = NewBack;
            m_Nodes[OldBack] = node_type(); // reinitialize node (not necessary, but maybe helpful for debugging)
            _private_deallocate_node(OldBack);
        }
    }

    iterator insert(iterator Position, const_reference Value)
    {
        if (size() < Capacity)
        {
            node_index_type Behind = Position.m_Node;
            node_index_type InFront = m_Nodes[Behind].m_Prev;
            node_index_type Inserted = _private_allocate_node();
            m_Values[Inserted - 1] = Value;
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

    iterator emplace(iterator Position)
    {
        if (size() < Capacity)
        {
            node_index_type Behind = Position.m_Node;
            node_index_type InFront = m_Nodes[Behind].m_Prev;
            node_index_type Inserted = _private_allocate_node();
            m_Values[Inserted - 1] = value_type(); // ToDo: necessary?
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
        m_Nodes[Erase] = node_type(); // reinitialize node (not necessary, but maybe helpful for debugging)
        m_Values[Erase - 1] = value_type(); // destroy element (ToDo: necessary?)
        _private_deallocate_node(Erase);
        return iterator(this, Behind);
    }

};

}

#endif /* CCC_ARRAY_LIST_H_ */
