/**
 *
 * @file This file contains the ArrayDeque container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_POD_DEQUE_H_
#define CCC_POD_DEQUE_H_

#include <ciso646>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <iostream>

//#include <utility> // to test adding missing operators automatically
#include <ccc/compat.h>
#include <ccc/memory.h>
#include <ccc/type_traits.h>
#include <ccc/alignment.h>

namespace ccc
{

/**
 * @brief Consistent, static-capacity double-ended queue.
 *
 * Constant time: Inserting and erasing elements at the beginning and the end; accessing random elements.
 * Linear time: Inserting and erasing elements elsewhere.
 * Noncompliance: No swap method, ...
 */
template <class T, class SizeType, SizeType Capacity, std::size_t Alignment = 8, bool UseRawMemOps = false>
struct PODDeque
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef PODDeque<T, SizeType, Capacity, Alignment, UseRawMemOps> container_type; // necessary for random access iterator

    /**
     * Assumption: divisor is a positive number
     */
    static size_type modulo(difference_type dividend, size_type divisor)
    {
        if (0 > (-1%2) and 0 > dividend)
        {
            return (dividend % static_cast<difference_type>(divisor)) + divisor;
        }
        else
        {
            return (dividend % static_cast<difference_type>(divisor));
        }
    }
//    template <bool NegativeModulo = (0 > (-1%2))>
//    struct modulo {};
//
//    template <bool NegativeModulo>
//    struct modulo<true>
//    {
//        size_type operator()(difference_type dividend, size_type divisor)
//        {
//            return (dividend % divisor) + divisor;
//        }
//    };
//
//    template <bool NegativeModulo>
//    struct modulo<false>
//    {
//        size_type operator()(difference_type dividend, size_type divisor)
//        {
//            return (dividend % divisor);
//        }
//    };

    template <class T_>
    struct RandomAccessIterator
    {
        typedef T_ value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef RandomAccessIterator<T_> iterator_type;
        typedef std::random_access_iterator_tag iterator_category;

        container_type* m_Container;
        size_type m_PhysicalIndex;

        RandomAccessIterator()
                : m_Container(), m_PhysicalIndex()
        {
        }

        RandomAccessIterator(container_type* Container, size_type PhysicalIndex)
                : m_Container(Container), m_PhysicalIndex(PhysicalIndex)
        {
        }

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

        // random access iterator requirements:

        iterator_type& operator+=(difference_type rhs)
        {
            if (0 > rhs)
            {
                rhs = modulo(rhs, (Capacity + 1)) - (Capacity + 1);
            }
            else
            {
                rhs = modulo(rhs, (Capacity + 1));
            }
            if (0 > rhs)
            {
                m_PhysicalIndex = (-rhs > static_cast<difference_type>(m_PhysicalIndex)) ? (Capacity + (rhs + m_PhysicalIndex) + 1) : (m_PhysicalIndex + rhs);
            }
            else
            {
                m_PhysicalIndex = (static_cast<difference_type>(Capacity - m_PhysicalIndex) < rhs) ? (rhs - (Capacity - m_PhysicalIndex) - 1) : (m_PhysicalIndex + rhs);
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

        friend iterator_type operator+(difference_type lhs, const iterator_type& rhs)
        {
            iterator_type Result = rhs;
            return Result += lhs;
        }

        difference_type operator-(const iterator_type& rhs) const// const // must function be const for short version of operator<?
        {
            bool NegativeResult;
            if (m_Container->m_End < m_Container->m_Begin)
            {
                NegativeResult = modulo((static_cast<difference_type>(this->m_PhysicalIndex) - m_Container->m_Begin), (Capacity + 1)) < modulo((static_cast<difference_type>(rhs.m_PhysicalIndex) - m_Container->m_Begin), (Capacity + 1));
            }
            else
            {
                NegativeResult = this->m_PhysicalIndex < rhs.m_PhysicalIndex;
            }
            if (NegativeResult)
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (Capacity + 1)) - (Capacity + 1);
            }
            else
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (Capacity + 1));
            }
        }

        bool operator<(const iterator_type& rhs)
        {
            return 0 < rhs - *this;
        }
    };

    template <class T_>
    struct RandomAccessConstIterator
    {
        typedef T_ value_type;
        typedef const value_type* pointer;
        typedef const value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef RandomAccessConstIterator<T_> iterator_type;
        typedef std::random_access_iterator_tag iterator_category;

        const container_type* m_Container;
        size_type m_PhysicalIndex;

        RandomAccessConstIterator()
                : m_Container(), m_PhysicalIndex()
        {
        }

        RandomAccessConstIterator(const container_type* Container, size_type PhysicalIndex)
                : m_Container(Container), m_PhysicalIndex(PhysicalIndex)
        {
        }

        RandomAccessConstIterator(const RandomAccessIterator<T_>& NonConst)
                : m_Container(NonConst.m_Container), m_PhysicalIndex(NonConst.m_PhysicalIndex)
        {
        }

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

        // random access iterator requirements:

        iterator_type& operator+=(difference_type rhs)
        {
            if (0 > rhs)
            {
                rhs = modulo(rhs, (Capacity + 1)) - (Capacity + 1);
            }
            else
            {
                rhs = modulo(rhs, (Capacity + 1));
            }
            if (0 > rhs)
            {
                m_PhysicalIndex = (-rhs > static_cast<difference_type>(m_PhysicalIndex)) ? (Capacity + (rhs + m_PhysicalIndex) + 1) : (m_PhysicalIndex + rhs);
            }
            else
            {
                m_PhysicalIndex = (static_cast<difference_type>(Capacity - m_PhysicalIndex) < rhs) ? (rhs - (Capacity - m_PhysicalIndex) - 1) : (m_PhysicalIndex + rhs);
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

        friend iterator_type operator+(difference_type lhs, const iterator_type& rhs)
        {
            iterator_type Result = rhs;
            return Result += lhs;
        }

        difference_type operator-(const iterator_type& rhs) const // const // must function be const for short version of operator<?
        {
            bool NegativeResult;
            if (m_Container->m_End < m_Container->m_Begin)
            {
                NegativeResult = modulo((static_cast<difference_type>(this->m_PhysicalIndex) - m_Container->m_Begin), (Capacity + 1)) < modulo((static_cast<difference_type>(rhs.m_PhysicalIndex) - m_Container->m_Begin), (Capacity + 1));
            }
            else
            {
                NegativeResult = this->m_PhysicalIndex < rhs.m_PhysicalIndex;
            }
            if (NegativeResult)
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (Capacity + 1)) - (Capacity + 1);
            }
            else
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (Capacity + 1));
            }
        }

        bool operator<(const iterator_type& rhs)
        {
            return 0 < rhs - *this;
        }
    };

    typedef RandomAccessIterator<value_type> iterator;
    typedef RandomAccessConstIterator<value_type> const_iterator;

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

#if (__cplusplus >= 201103L)
    alignas(Alignment) size_type m_Begin; // points at the first element
    alignas(Alignment) size_type m_End; // points at the element
    alignas(Alignment) value_type m_Array[Capacity + 1]; // well, we might get rid of the extra element, but for the sake of simplicity let's keep it for now
#else
    PaddedValue<size_type, Alignment> m_Begin;
    PaddedValue<size_type, Alignment> m_End; // points at the element behind the last valid element
    PaddedArray<value_type, Capacity + 1, Alignment> m_Array;
#endif

    // Private methods:

    pointer data(size_type PhysicalIndex)
    {
        return ccc::addressof(m_Array[PhysicalIndex]);
    }

    const_pointer data(size_type PhysicalIndex) const CCC_NOEXCEPT
    {
        return ccc::addressof(m_Array[PhysicalIndex]);
    }

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

    // Element access:

    reference operator[](size_type LogicalIndex)
    {
        return m_Array[(Capacity - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (Capacity + 1)) : (m_Begin + LogicalIndex)];
    }

    CCC_CONSTEXPR
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

    CCC_CONSTEXPR
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

    // Iterators:

    iterator begin() CCC_NOEXCEPT
    {
        return iterator(this, m_Begin);
    }

    const_iterator begin() const CCC_NOEXCEPT
    {
        return const_iterator(this, m_Begin);
    }

    iterator end() CCC_NOEXCEPT
    {
        return iterator(this, m_End);
    }

    const_iterator end() const CCC_NOEXCEPT
    {
        return const_iterator(this, m_End);
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

    // Capacity:

    bool empty() const CCC_NOEXCEPT
    {
        return m_Begin == m_End;
    }

    size_type size() const CCC_NOEXCEPT
    {
        return (m_End < m_Begin) ? (m_End - m_Begin + Capacity + 1) : (m_End - m_Begin);
    }

    CCC_CONSTEXPR
    size_type max_size() const CCC_NOEXCEPT
    {
        return Capacity;
    }

    // Modifiers:

    void clear() CCC_NOEXCEPT
    {
        for (iterator it = begin(); it != end(); ++it)
        {
            *it = value_type();
        }
        m_Begin = size_type();
        m_End = size_type();
    }

    void push_front(const_reference Value)
    {
        if (size() < Capacity)
        {
            m_Begin = (0 == m_Begin) ? Capacity : (m_Begin - 1);
            m_Array[m_Begin] = Value; // ToDo: Must type be trivially copyable?
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
            m_Array[m_End] = Value; // ToDo: Must type be trivially copyable?
            m_End = (Capacity == m_End) ? 0 : (m_End + 1);
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
            m_Begin = (0 == m_Begin) ? Capacity : (m_Begin - 1);
            m_Array[m_Begin] = value_type(); // ToDo: Must type be trivially copyable?
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
            m_Array[m_End] = value_type(); // ToDo: Must type be trivially copyable?
            m_End = (Capacity == m_End) ? 0 : (m_End + 1);
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
            // ToDo: Do we have to destroy the element?
            front() = value_type(); // destroy element
            m_Begin = (Capacity == m_Begin) ? 0 : (m_Begin + 1);
        }
    }

    void pop_back()
    {
        if (not empty())
        {
            // ToDo: Do we have to destroy the element?
            back() = value_type(); // destroy element
            m_End = (0 == m_End) ? Capacity : (m_End - 1);
        }
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
            throw std::bad_alloc();
        }
    }

    iterator emplace(iterator Position)
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
                *(Position - 1) = value_type();
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
                *Position = value_type();
                return Position;
            }
        }
        else
        {
            throw std::bad_alloc();
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

#endif /* CCC_POD_DEQUE_H_ */
