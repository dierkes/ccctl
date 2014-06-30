/**
 *
 * @file This file contains the ArrayDeque container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_ARRAY_DEQUE_H_
#define CCC_ARRAY_DEQUE_H_

#include <ciso646>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <stdexcept>

//#include <utility> // to test adding missing operators automatically
#include <ccc/compat.h>
#include <ccc/memory.h>
#include <ccc/type_traits.h>

namespace ccc
{

/**
 * @brief Consistent, static-capacity double-ended queue.
 *
 * Constant time: Inserting and erasing elements at the beginning and the end; accessing random elements.
 * Linear time: Inserting and erasing elements elsewhere.
 * Noncompliance: No swap method, ...
 */
template <class T, class SizeType, SizeType Capacity, bool UseRawMemOps = ccc::is_trivially_copyable_guaranteed<T>::value>
struct ArrayDeque
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef ArrayDeque<T, SizeType, Capacity, UseRawMemOps> container_type; // necessary for random access iterator

    template <class T>
    struct RandomAccessIterator
    {
        typedef T value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef RandomAccessIterator<T> iterator_type;
        typedef std::bidirectional_iterator_tag iterator_category;

        container_type* m_Container;
        size_type m_PhysicalIndex;

        reference operator*() const
        {
            return m_Container->m_Array[m_PhysicalIndex];
        }

        pointer operator->() const
        {
            return ccc::addressof<pointer>(m_Container->m_Array[m_PhysicalIndex]);
        }

        iterator_type& operator++()
        {
            m_PhysicalIndex = (Capacity == m_PhysicalIndex) ? 0 : (m_PhysicalIndex + 1);
            return *this;
        }

        iterator_type operator++(int)
        {
            iterator_type tmp = *this;
            m_PhysicalIndex = (Capacity == m_PhysicalIndex) ? 0 : (m_PhysicalIndex + 1);
            return tmp;
        }

        iterator_type&
        operator--()
        {
            m_PhysicalIndex = (0 == m_PhysicalIndex) ? Capacity : (m_PhysicalIndex - 1);
            return *this;
        }

        iterator_type operator--(int)
        {
            iterator_type tmp = *this;
            m_PhysicalIndex = (0 == m_PhysicalIndex) ? Capacity : (m_PhysicalIndex - 1);
            return tmp;
        }

        bool operator==(const iterator_type& rhs) const
        {
            return (m_Container == rhs.m_Container) and (m_PhysicalIndex == rhs.m_PhysicalIndex);
        }

        bool operator!=(const iterator_type& rhs) const
        {
            return (m_Container != rhs.m_Container) or (m_PhysicalIndex != rhs.m_PhysicalIndex);
        }

        // bidirectional iterator requirements:

        iterator_type& operator+=(const difference_type& rhs)
        {
            if (0 > rhs)
            {
                m_PhysicalIndex = (-rhs > m_PhysicalIndex) ? (Capacity + (rhs + m_PhysicalIndex) + 1) : (m_PhysicalIndex + rhs);
            }
            else
            {
                m_PhysicalIndex = (Capacity - m_PhysicalIndex < rhs) ? (rhs - (Capacity - m_PhysicalIndex) - 1) : (m_PhysicalIndex + rhs);
            }
            return *this;
        }

        iterator_type& operator-=(const difference_type& rhs)
        {
            *this += -rhs;
            return *this;
        }

        iterator_type operator+(const difference_type& rhs)
        {
            iterator_type Result = *this;
            return Result += rhs;
        }

        iterator_type operator-(const difference_type& rhs)
        {
            iterator_type Result = *this;
            return Result += -rhs;
        }

        /**
         * Note: This is not a member function. Possible alternative: template for operator+(lhs, rhs)
         *
         * template <class IntegerType, class IteratorType>
         * IteratorType operator+(IntegerType lhs, IteratorType rhs)
         * {
         *     return rhs += lhs;
         * }
         *
         * Has possibly side effects.
         *
         */
        friend iterator_type operator+(difference_type lhs, const iterator_type& rhs)
        {
            iterator_type Result = rhs;
            return Result += lhs;
        }

        difference_type operator-(const iterator_type& rhs) // const // must function be const for short version of operator<?
        {
//            return ((m_Container->m_Begin <= m_Container->m_End) or (m_Container->m_Begin <= m_PhysicalIndex and m_Container->m_Begin <= rhs.m_PhysicalIndex)
//                    or (m_Container->m_End >= m_PhysicalIndex and m_Container->m_End >= rhs.m_PhysicalIndex)) ?
//                    (m_PhysicalIndex - rhs.m_PhysicalIndex) : (m_Container->m_Begin <= m_PhysicalIndex and rhs.m_PhysicalIndex <= m_Container->m_End);
            if (m_Container->m_Begin <= m_PhysicalIndex and rhs.m_PhysicalIndex <= m_Container->m_End)
            {
                return (Capacity + 1 - m_PhysicalIndex) + rhs.m_PhysicalIndex;
            }
            else if (m_Container->m_Begin <= rhs.m_PhysicalIndex and m_PhysicalIndex <= m_Container->m_End)
            {
                return -((Capacity + 1 - rhs.m_PhysicalIndex) + m_PhysicalIndex);
            }
            else
            {
                return m_PhysicalIndex - rhs.m_PhysicalIndex;
            }

        }

        bool operator<(const iterator_type& rhs)
        {
//            return ((m_Container->m_Begin <= m_Container->m_End) or (m_Container->m_Begin <= m_PhysicalIndex and m_Container->m_Begin <= rhs.m_PhysicalIndex)
//                    or (m_Container->m_End >= m_PhysicalIndex and m_Container->m_End >= rhs.m_PhysicalIndex)) ?
//                    (m_PhysicalIndex < rhs.m_PhysicalIndex) : (m_Container->m_Begin <= m_PhysicalIndex and rhs.m_PhysicalIndex <= m_Container->m_End);
            return 0 < rhs - *this;
        }
    };

    typedef RandomAccessIterator<value_type> iterator;
    typedef RandomAccessIterator<const value_type> const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator; // ToDo: what does this mean in case of a pointer, in general it's a class derivation
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator; // ToDo: see above

    value_type m_Array[Capacity + 1]; // well, we might get rid of the extra element, but for the sake of simplicity let's keep it for now
    size_type m_Begin; // points at the first element
    size_type m_End; // points at the element

    pointer data(size_type PhysicalIndex)
    {
        return ccc::addressof(m_Array[PhysicalIndex]);
    }

    const_pointer data(size_type PhysicalIndex) const CCC_NOEXCEPT
    {
        return ccc::addressof(m_Array[PhysicalIndex]);
    }

    iterator begin() CCC_NOEXCEPT
    {
        iterator Result;
        Result.m_Container = this;
        Result.m_PhysicalIndex = m_Begin;
        return Result;
    }

    const_iterator begin() const CCC_NOEXCEPT
    {
        const_iterator Result;
        Result.m_Container = this;
        Result.m_PhysicalIndex = m_Begin;
        return Result;
    }

    iterator end() CCC_NOEXCEPT
    {
        iterator Result;
        Result.m_Container = this;
        Result.m_PhysicalIndex = m_End;
        return Result;
    }

    const_iterator end() const CCC_NOEXCEPT
    {
        const_iterator Result;
        Result.m_Container = this;
        Result.m_PhysicalIndex = m_End;
        return Result;
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

//    CCC_CONSTEXPR
    size_type size() const CCC_NOEXCEPT
    {
        return (m_End < m_Begin) ? (m_End - m_Begin + Capacity + 1) : (m_End - m_Begin);
    }

    CCC_CONSTEXPR
    size_type max_size() const CCC_NOEXCEPT
    {
        return Capacity;
    }

//    CCC_CONSTEXPR
    bool empty() const CCC_NOEXCEPT
    {
        return m_Begin == m_End;
    }

    reference operator[](size_type LogicalIndex)
    {
        return m_Array[(Capacity - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (Capacity + 1)) : (m_Begin + LogicalIndex)];
    }

//    CCC_CONSTEXPR
    const_reference operator[](size_type LogicalIndex) const CCC_NOEXCEPT
    {
        return m_Array[(Capacity - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (Capacity + 1)) : (m_Begin + LogicalIndex)];
    }

    reference at(size_type LogicalIndex) // ToDo
    {
        if (LogicalIndex >= size())
        {
            throw std::out_of_range("ConsistentArray::at"), m_Array[0];
        }
        return m_Array[(Capacity - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (Capacity + 1)) : (m_Begin + LogicalIndex)];
    }

//    CCC_CONSTEXPR
    const_reference at(size_type LogicalIndex) const // ToDo
    {
        return LogicalIndex < size() ?
                m_Array[(Capacity - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (Capacity + 1)) : (m_Begin + LogicalIndex)] : (throw std::out_of_range("ConsistentArray::at"), m_Array[0]);
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
        return *(end() - 1);
    }

    const_reference back() const
    {
        return *(end() - 1);
    }

    /**
     * According to the standard, the push and pop methods have no return values.
     * Further, the methods may throw exceptions (e.g., std::bad_alloc if an allocation fails).
     *
     * We ...
     */

    bool push_front(const_reference Value) CCC_NOEXCEPT
    {
        if (size() < Capacity)
        {
            m_Begin = (0 == m_Begin) ? Capacity : (m_Begin - 1);
            m_Array[m_Begin] = Value; // ToDo: Must type be trivially copyable?
            return true;
        }
        else
            return false;
    }

    bool push_back(const_reference Value) CCC_NOEXCEPT
    {
        if (size() < Capacity)
        {
            m_Array[m_End] = Value; // ToDo: Must type be trivially copyable?
            m_End = (Capacity == m_End) ? 0 : (m_End + 1);
            return true;
        }
        else
            return false;
    }

    bool pop_front() CCC_NOEXCEPT
    {
        if (not empty())
        {
            // ToDo: Do we have to destroy the element?
            m_Begin = (Capacity == m_Begin) ? 0 : (m_Begin + 1);
            return true;
        }
        else
            return false;
    }

    bool pop_back() CCC_NOEXCEPT
    {
        if (not empty())
        {
            // ToDo: Do we have to destroy the element?
            m_End = (0 == m_End) ? Capacity : (m_End - 1);
            return true;
        }
        else
            return false;
    }

    iterator insert(size_type Position, const_reference Value)
    {
        return insert(iterator(ccc::addressof(m_Array[Position])), Value);
    }

    iterator insert(iterator Position, const_reference Value)
    {
        if (size() < Capacity) // rules out case LogicalBegin == LogicalEnd
        {
            if (ccc::addressof(*Position) > ccc::addressof(*end())) // == Position is in range [LogicalBegin, PhysicalEnd], if PhysicalBegin <= LogicalEnd < LogicalBegin <= PhysicalEnd
            {
                if (UseRawMemOps)
                {
                    // ccc::addressof(*(begin() - 1)) should not be necessary, since ccc::addressof should return a pointer of type value_type*
                    std::memmove(ccc::addressof(*begin()) - 1, ccc::addressof(*begin()), (Position - begin()) * sizeof(value_type));
                }
                else
                {
                    std::copy(begin(), Position, begin() - 1);
                }
                m_Begin = (0 == m_Begin) ? Capacity : (m_Begin - 1);
                *(Position - 1) = Value;
                return (Position - 1);
            }
            else // == Position is in range [PhysicalBegin, LogicalEnd], if PhysicalBegin <= LogicalEnd < LogicalBegin <= PhysicalEnd
                 // or Position is in range [LogicalBegin, LogicalEnd], if PhysicalBegin <= LogicalBegin < LogicalEnd <= PhysicalEnd
            {
                if (UseRawMemOps)
                {
                    std::memmove(ccc::addressof(*Position) + 1, ccc::addressof(*Position), (end() - Position) * sizeof(value_type));
                }
                else
                {
                    std::copy_backward(Position, end(), end() + 1);
                }
                m_End = (Capacity == m_End) ? 0 : (m_End + 1);
                *Position = Value;
                return Position;
            }
        }
        else
        {
            return end();
//            return iterator(0);
//            throw std::bad_alloc();
        }
    }

    iterator erase(iterator Position)
    {
        if (ccc::addressof(*Position) > ccc::addressof(*end())) // == Position is in range [LogicalBegin, PhysicalEnd], if PhysicalBegin <= LogicalEnd < LogicalBegin <= PhysicalEnd
        {
            // (*Position).~T(); // call destructor of the object at Position (might not work for basic types)
            *Position = value_type();
            if (UseRawMemOps)
            {
                std::memmove(ccc::addressof(*begin()) + 1, ccc::addressof(*begin()), (Position - begin()) * sizeof(value_type));
            }
            else
            {
                std::copy_backward(begin(), Position, Position + 1);
            }
            m_Begin = (Capacity == m_Begin) ? 0 : (m_Begin + 1);
            return Position + 1;
        }
        else
        {
            *Position = value_type();
            if (UseRawMemOps)
            {
                std::memmove(ccc::addressof(*Position), ccc::addressof(*Position) + 1, ((end() - 1) - Position) * sizeof(value_type));
            }
            else
            {
                std::copy(Position + 1, end(), Position);
            }
            m_End = (0 == m_End) ? Capacity : (m_End - 1);
            return Position;
        }
    }

};

}

#endif /* CCC_ARRAY_DEQUE_H_ */
