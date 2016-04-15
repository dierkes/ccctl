/**
 *
 * @file This file contains the ArrayStack container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_POD_VECTOR_H_
#define CCC_POD_VECTOR_H_

#include <ciso646>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <stdexcept>
#include <iterator>

#include <ccc/compat.h>
#include <ccc/memory.h>
#include <ccc/type_traits.h>
#include <ccc/alignment.h>
#include <ccc/algorithm.h>
#include <ccc/storage.h>

namespace ccc
{

#pragma pack(push, 16)

/**
 * @brief Consistent, static-capacity vector.
 *
 * Constant time: inserting and erasing elements at the end; accessing random elements.
 * Linear time: inserting and erasing elements elsewhere.
 * Noncompliance: No swap method, ...
 */
template <class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8, bool UseRawMemOps = false, bool Uninitialized = false, bool Runtime = false>
struct PodVector
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef std::reverse_iterator<iterator> reverse_iterator; // ToDo: what does this mean in case of a pointer, in general it's a class derivation
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator; // ToDo: see above

    typedef typename ccc::integral_constant<bool, UseRawMemOps>::type UseRawMemOpsType;

    typedef typename Storage<T, SizeType, Capacity, Alignment, Uninitialized, Runtime>::type storage_type;

#if (CCC_ALIGNAS_AVAILABLE)
    alignas(Alignment) size_type m_End;
#elif CCC_ALIGNED_AVAILABLE
    typename ccc::Aligned<size_type, Alignment>::type m_End;
#else
    PaddedValue<size_type, Alignment> m_End; // points at the element behind the last valid element
#endif
    storage_type m_Storage;

    pointer data(size_type Index) CCC_NOEXCEPT
    {
        return ccc::addressof(m_Storage[Index]);
    }

    const_pointer data(size_type Index) const CCC_NOEXCEPT
    {
        return ccc::addressof(m_Storage[Index]);
    }

    pointer data() CCC_NOEXCEPT
    {
        return data(0);
    }

    const_pointer data() const CCC_NOEXCEPT
    {
        return data(0);
    }

    // Assign:

    void assign(size_type Count, const value_type& Value)
    {
        clear();
        m_Storage.construct_and_assign(begin(), Count, Value);
        m_End = m_End + Count;
    }

    template <typename IteratorType>
    void assign(IteratorType First, IteratorType Last)
    {
        clear();
        m_Storage.construct_and_assign(begin(), First, Last);
        m_End = m_End + std::distance(First, Last);
    }

    // Iterators:

    iterator begin() CCC_NOEXCEPT
    {
        return iterator(data(0));
    }

    const_iterator begin() const CCC_NOEXCEPT
    {
        return const_iterator(data(0));
    }

    iterator end() CCC_NOEXCEPT
    {
        return iterator(data(m_End));
    }

    const_iterator end() const CCC_NOEXCEPT
    {
        return const_iterator(data(m_End));
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
        return m_End - 0;
    }

    CCC_CONSTEXPR
    size_type max_size() const CCC_NOEXCEPT
    {
        return m_Storage.max_size();
    }

    CCC_CONSTEXPR
    bool empty() const CCC_NOEXCEPT
    {
        return 0 == m_End;
    }

    reference operator[](size_type Index)
    {
        return m_Storage[Index];
    }

    CCC_CONSTEXPR
    const_reference operator[](size_type Index) const CCC_NOEXCEPT
    {
        return m_Storage[Index];
    }

    reference at(size_type Index)
    {
        if (Index >= size())
        {
            throw std::out_of_range("ConsistentArray::at"), m_Storage[0];
        }
        return m_Storage[Index];
    }

    CCC_CONSTEXPR
    const_reference at(size_type Index) const
    {
        return Index < size() ? m_Storage[Index] : (throw std::out_of_range("ConsistentArray::at"), m_Storage[0]);
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

    void clear() CCC_NOEXCEPT
    {
        m_Storage.destroy(begin(), end());
        m_End = size_type();
    }

    void push_back(const_reference Value)
    {
        if (size() < max_size())
        {
            m_Storage.construct_and_assign(end(), Value);
            m_End = m_End + 1;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    void pop_back()
    {
        if (not empty())
        {
            m_End = m_End - 1;
            m_Storage.destroy(end());
        }
    }

    iterator insert(size_type Position, const_reference Value)
    {
        return insert(iterator(ccc::addressof(m_Storage[Position])), Value);
    }

    iterator insert(iterator Position, const_reference Value)
    {
        if (size() < max_size())
        {
            m_Storage.construct_default(end());
            ccc::move_backward(Position, end(), end() + 1, UseRawMemOpsType());
            m_End = m_End + 1;
            *Position = Value;
            return Position;
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
        if (Count <= static_cast<difference_type>(max_size() - size()))
        {
            m_Storage.construct_default(end(), Count);
            ccc::move_backward(Position, end(), end() + Count, UseRawMemOpsType());
            std::copy(First, Last, Position);
            m_End = m_End + Count;
            return Position;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    iterator insert(iterator Position, size_type Count, const value_type& Value)
    {
        if (Count <= max_size() - size())
        {
            m_Storage.construct_default(end(), Count);
            ccc::move_backward(Position, end(), end() + Count, UseRawMemOpsType());
            std::fill(Position, Position + Count, Value);
            m_End = m_End + Count;
            return Position;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    iterator erase(iterator Position)
    {
        if (Position < end())
        {
            ccc::move(Position + 1, end(), Position, UseRawMemOpsType());
            m_End = m_End - 1;
            m_Storage.destroy(end());
            return Position;
        }
        else
        {
            return end();
        }
    }

    iterator erase(iterator First, iterator Last)
    {
        ccc::move(Last, end(), First, UseRawMemOpsType());
        m_End = static_cast<size_type>(m_End - std::distance(First, Last));
        m_Storage.destroy(end(), end() + std::distance(First, Last));
        return First;
    }

    void resize(size_type const Count)
    {
        if (Count < size())
        {
            m_End = Count;
            m_Storage.destroy(end(), end() + Count);
        }
        else if (Count > size())
        {
            m_Storage.construct_default(end(), Count - size());
            m_End = Count;
        }
    }

    void resize(size_type const Count, value_type const& Value)
    {
        if (Count < size())
        {
            m_End = Count;
            m_Storage.destroy(end(), end() + Count);
        }
        else if (Count > size())
        {
            m_Storage.construct_and_assign(end(), Count - size(), Value);
            m_End = Count;
        }
    }

    void reserve(size_type const DummyCapacity)
    {
        if (DummyCapacity > max_size())
        {
            throw std::bad_alloc();
        }
    }

    size_type capacity()
    {
        return m_Storage.max_size();
    }
};

#pragma pack(pop)

}

#endif /* CCC_POD_VECTOR_H_ */
