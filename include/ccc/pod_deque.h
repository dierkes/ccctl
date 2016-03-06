/**
 *
 * @file This file contains the PODDeque container.
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
#include <ccc/storage.h>
#include <ccc/algorithm.h>

namespace ccc
{

#pragma pack(push, 16)

/**
 * @brief Consistent, static-capacity double-ended queue.
 *
 * Constant time: Inserting and erasing elements at the beginning and the end; accessing random elements.
 * Linear time: Inserting and erasing elements elsewhere.
 * Noncompliance: No swap method, ...
 */
template <class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8, bool UseRawMemOps = false, bool Uninitialized = false, bool Runtime = false>
struct PodDeque
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    static const typename ccc::integral_constant<bool, UseRawMemOps>::type RawMemOps;

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

    template <class T_>
    struct RandomAccessIterator
    {
        typedef T_ value_type;
        typedef value_type* pointer;
        typedef value_type& reference;
        typedef std::ptrdiff_t difference_type;
        typedef RandomAccessIterator<T_> iterator_type;
        typedef std::random_access_iterator_tag iterator_category;

        PodDeque* m_Container;
        size_type m_PhysicalIndex;

        RandomAccessIterator()
                : m_Container(), m_PhysicalIndex()
        {
        }

        RandomAccessIterator(PodDeque* Container, size_type PhysicalIndex)
                : m_Container(Container), m_PhysicalIndex(PhysicalIndex)
        {
        }

        reference operator*() const
        {
            return m_Container->m_Storage[m_PhysicalIndex];
        }

        pointer operator->() const
        {
            return ccc::addressof<pointer>(m_Container->m_Storage[m_PhysicalIndex]);
        }

        iterator_type& operator++()
        {
            m_PhysicalIndex = (m_Container->max_size() == m_PhysicalIndex) ? 0 : (m_PhysicalIndex + 1);
            return *this;
        }

        iterator_type operator++(int)
        {
            iterator_type tmp = *this;
            m_PhysicalIndex = (m_Container->max_size() == m_PhysicalIndex) ? 0 : (m_PhysicalIndex + 1);
            return tmp;
        }

        iterator_type&
        operator--()
        {
            m_PhysicalIndex = (0 == m_PhysicalIndex) ? m_Container->max_size() : (m_PhysicalIndex - 1);
            return *this;
        }

        iterator_type operator--(int)
        {
            iterator_type tmp = *this;
            m_PhysicalIndex = (0 == m_PhysicalIndex) ? m_Container->max_size() : (m_PhysicalIndex - 1);
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
                rhs = static_cast<difference_type>(modulo(rhs, (m_Container->max_size() + 1))) - (m_Container->max_size() + 1);
            }
            else
            {
                rhs = modulo(rhs, (m_Container->max_size() + 1));
            }
            if (0 > rhs)
            {
                m_PhysicalIndex = (-rhs > static_cast<difference_type>(m_PhysicalIndex)) ? (m_Container->max_size() + (rhs + m_PhysicalIndex) + 1) : (m_PhysicalIndex + rhs);
            }
            else
            {
                m_PhysicalIndex = (static_cast<difference_type>(m_Container->max_size() - m_PhysicalIndex) < rhs) ? (rhs - (m_Container->max_size() - m_PhysicalIndex) - 1) : (m_PhysicalIndex + rhs);
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
                NegativeResult = modulo((static_cast<difference_type>(this->m_PhysicalIndex) - m_Container->m_Begin), (m_Container->max_size() + 1)) < modulo((static_cast<difference_type>(rhs.m_PhysicalIndex) - m_Container->m_Begin), (m_Container->max_size() + 1));
            }
            else
            {
                NegativeResult = this->m_PhysicalIndex < rhs.m_PhysicalIndex;
            }
            if (NegativeResult)
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (m_Container->max_size() + 1)) - (m_Container->max_size() + 1);
            }
            else
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (m_Container->max_size() + 1));
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

        const PodDeque* m_Container;
        size_type m_PhysicalIndex;

        RandomAccessConstIterator()
                : m_Container(), m_PhysicalIndex()
        {
        }

        RandomAccessConstIterator(const PodDeque* Container, size_type PhysicalIndex)
                : m_Container(Container), m_PhysicalIndex(PhysicalIndex)
        {
        }

        RandomAccessConstIterator(const RandomAccessIterator<T_>& NonConst)
                : m_Container(NonConst.m_Container), m_PhysicalIndex(NonConst.m_PhysicalIndex)
        {
        }

        reference operator*() const
        {
            return m_Container->m_Storage[m_PhysicalIndex];
        }

        pointer operator->() const
        {
            return ccc::addressof<pointer>(m_Container->m_Storage[m_PhysicalIndex]);
        }

        iterator_type& operator++()
        {
            m_PhysicalIndex = (m_Container->max_size() == m_PhysicalIndex) ? 0 : (m_PhysicalIndex + 1);
            return *this;
        }

        iterator_type operator++(int)
        {
            iterator_type tmp = *this;
            m_PhysicalIndex = (m_Container->max_size() == m_PhysicalIndex) ? 0 : (m_PhysicalIndex + 1);
            return tmp;
        }

        iterator_type&
        operator--()
        {
            m_PhysicalIndex = (0 == m_PhysicalIndex) ? m_Container->max_size() : (m_PhysicalIndex - 1);
            return *this;
        }

        iterator_type operator--(int)
        {
            iterator_type tmp = *this;
            m_PhysicalIndex = (0 == m_PhysicalIndex) ? m_Container->max_size() : (m_PhysicalIndex - 1);
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
                rhs = static_cast<difference_type>(modulo(rhs, (m_Container->max_size() + 1))) - (m_Container->max_size() + 1);
            }
            else
            {
                rhs = modulo(rhs, (m_Container->max_size() + 1));
            }
            if (0 > rhs)
            {
                m_PhysicalIndex = (-rhs > static_cast<difference_type>(m_PhysicalIndex)) ? (m_Container->max_size() + (rhs + m_PhysicalIndex) + 1) : (m_PhysicalIndex + rhs);
            }
            else
            {
                m_PhysicalIndex = (static_cast<difference_type>(m_Container->max_size() - m_PhysicalIndex) < rhs) ? (rhs - (m_Container->max_size() - m_PhysicalIndex) - 1) : (m_PhysicalIndex + rhs);
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
                NegativeResult = modulo((static_cast<difference_type>(this->m_PhysicalIndex) - m_Container->m_Begin), (m_Container->max_size() + 1)) < modulo((static_cast<difference_type>(rhs.m_PhysicalIndex) - m_Container->m_Begin), (m_Container->max_size() + 1));
            }
            else
            {
                NegativeResult = this->m_PhysicalIndex < rhs.m_PhysicalIndex;
            }
            if (NegativeResult)
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (m_Container->max_size() + 1)) - (m_Container->max_size() + 1);
            }
            else
            {
                return modulo((static_cast<difference_type>(this->m_PhysicalIndex) - rhs.m_PhysicalIndex), (m_Container->max_size() + 1));
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

    typedef typename Storage<T, SizeType, Capacity + 1, Alignment, Uninitialized, Runtime>::type storage_type;

#if (CCC_ALIGNAS_AVAILABLE)
    alignas(Alignment) size_type m_Begin; // points at the first element
    alignas(Alignment) size_type m_End;
#elif CCC_ALIGNED_AVAILABLE
    typename ccc::Aligned<size_type, Alignment>::type m_Begin;
    typename ccc::Aligned<size_type, Alignment>::type m_End;
#else
    PaddedValue<size_type, Alignment> m_Begin;
    PaddedValue<size_type, Alignment> m_End; // points at the element behind the last valid element
#endif
    storage_type m_Storage;

    // Private methods:

    pointer data(size_type PhysicalIndex)
    {
        return ccc::addressof(m_Storage[PhysicalIndex]);
    }

    const_pointer data(size_type PhysicalIndex) const CCC_NOEXCEPT
    {
        return ccc::addressof(m_Storage[PhysicalIndex]);
    }

    // Assign:

    void assign(size_type Count, const value_type& Value)
    {
        clear();
        m_Storage.construct_and_assign(begin(), Count, Value);
        m_Storage.construct_and_assign(begin(), Count, Value);
        m_End = next(end(), Count).m_PhysicalIndex;
    }

    template <typename IteratorType>
    void assign(IteratorType First, IteratorType Last)
    {
        clear();
        m_Storage.construct_and_assign(begin(), First, Last);
        m_End = next(end(), std::distance(First, Last)).m_PhysicalIndex;
    }

    // Element access:

    reference operator[](size_type LogicalIndex)
    {
        return m_Storage[(max_size() - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (max_size() + 1)) : (m_Begin + LogicalIndex)];
    }

    CCC_CONSTEXPR
    const_reference operator[](size_type LogicalIndex) const CCC_NOEXCEPT
    {
        return m_Storage[(max_size() - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (max_size() + 1)) : (m_Begin + LogicalIndex)];
    }

    reference at(size_type LogicalIndex) // ToDo
    {
        if (LogicalIndex >= size())
        {
            throw std::out_of_range("ConsistentArray::at"), m_Storage[0];
        }
        return m_Storage[(max_size() - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (max_size() + 1)) : (m_Begin + LogicalIndex)];
    }

    CCC_CONSTEXPR
    const_reference at(size_type LogicalIndex) const // ToDo
    {
        return LogicalIndex < size() ?
                m_Storage[(max_size() - LogicalIndex < m_Begin) ? (m_Begin + LogicalIndex - (max_size() + 1)) : (m_Begin + LogicalIndex)] : (throw std::out_of_range("ConsistentArray::at"), m_Storage[0]);
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
        return (m_End < m_Begin) ? (m_End - m_Begin + max_size() + 1) : (m_End - m_Begin);
    }

    CCC_CONSTEXPR
    size_type max_size() const CCC_NOEXCEPT
    {
        return m_Storage.max_size() - 1;
    }

    // Modifiers:

    void clear() CCC_NOEXCEPT
    {
        m_Storage.destroy(begin(), end());
        m_Begin = size_type();
        m_End = size_type();
    }

    void push_front(const_reference Value)
    {
        if (size() < max_size())
        {
            m_Begin = (0 == m_Begin) ? max_size() : (m_Begin - 1);
            m_Storage.construct_and_assign(begin(), Value);
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
            m_Storage.construct_and_assign(end(), Value);
            m_End = (max_size() == m_End) ? 0 : (m_End + 1);
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
            m_Storage.destroy(begin());
            m_Begin = (max_size() == m_Begin) ? 0 : (m_Begin + 1);
        }
    }

    void pop_back()
    {
        if (not empty())
        {
            m_End = (0 == m_End) ? max_size() : (m_End - 1);
            m_Storage.destroy(end());
        }
    }

    iterator insert(size_type Position, const_reference Value)
    {
        return insert(iterator(ccc::addressof(m_Storage[Position])), Value);
    }

    iterator insert(iterator Position, const_reference Value)
    {
        if (size() < max_size()) // rules out case LogicalBegin == LogicalEnd
        {
            m_Storage.construct_default(begin() - 1);
            if (ccc::addressof(*Position) > ccc::addressof(*end())) // == Position is in range [LogicalBegin, PhysicalEnd], if PhysicalBegin <= LogicalEnd < LogicalBegin <= PhysicalEnd
            {
                ccc::move(begin(), Position, begin() - 1, RawMemOps);
                m_Begin = (0 == m_Begin) ? max_size() : (m_Begin - 1);
                *(Position - 1) = Value;
                return (Position - 1);
            }
            else // == Position is in range [PhysicalBegin, LogicalEnd], if PhysicalBegin <= LogicalEnd < LogicalBegin <= PhysicalEnd
                 // or Position is in range [LogicalBegin, LogicalEnd], if PhysicalBegin <= LogicalBegin < LogicalEnd <= PhysicalEnd
            {
                m_Storage.construct_default(end());
                ccc::move_backward(Position, end(), end() + 1, RawMemOps);
                m_End = (max_size() == m_End) ? 0 : (m_End + 1);
                *Position = Value;
                return Position;
            }
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    template <typename IteratorType>
    iterator insert(iterator Position, IteratorType First, IteratorType Last)
    {
        difference_type Count = std::distance(First, Last);
        if (Count <= 0)
        {
            return Position;
        }
        else if (Count <= static_cast<difference_type>(max_size() - size()))
        {
            m_Storage.construct_default(end(), static_cast<size_type>(Count));
            ccc::move_backward(Position, end(), end() + Count, RawMemOps);
            std::copy(First, Last, Position);
            m_End = next(end(), Count).m_PhysicalIndex;
            return Position;
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
            m_Storage.construct_default(end(), Count);
            ccc::move_backward(Position, end(), end() + Count, RawMemOps);
            std::fill(Position, Position + Count, Value);
            m_End = next(end(), Count).m_PhysicalIndex;
            return Position;
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
            ccc::move_backward(begin(), Position, Position + 1, RawMemOps);
            m_Storage.destroy(begin());
            m_Begin = (max_size() == m_Begin) ? 0 : (m_Begin + 1);
            return Position + 1;
        }
        else
        {
            ccc::move(Position + 1, end(), Position, RawMemOps);
            m_End = (0 == m_End) ? max_size() : (m_End - 1);
            m_Storage.destroy(end());
            return Position;
        }
    }

    iterator erase(iterator First, iterator Last)
    {
        std::copy(Last, end(), First);
        m_End = next(end(), -std::distance(First, Last)).m_PhysicalIndex;
        m_Storage.destroy(end(), end() + std::distance(First, Last));
        return First;
    }

};

#pragma pack(pop)

}

#endif /* CCC_POD_DEQUE_H_ */
