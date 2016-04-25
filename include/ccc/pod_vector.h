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
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

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

    // Assign:

    void assign(size_type Count, value_type const& Value)
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
        m_End = m_End + static_cast<size_type>(std::distance(First, Last));
    }

    reference operator[](size_type Position)
    {
        return m_Storage[Position];
    }

    const_reference operator[](size_type Position) const
    {
        return m_Storage[Position];
    }

    reference at(size_type Position)
    {
        if (Position >= size())
        {
            throw std::out_of_range("PodVector::at");
        }
        return m_Storage[Position];
    }

    const_reference at(size_type Position) const
    {
        if (Position >= size())
        {
            throw std::out_of_range("PodVector::at");
        }
        return m_Storage[Position];
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

    pointer data() CCC_NOEXCEPT
    {
        return ccc::addressof(m_Storage[0]);
    }

    const_pointer data() const CCC_NOEXCEPT
    {
        return ccc::addressof(m_Storage[0]);
    }

    // Iterators:

    iterator begin() CCC_NOEXCEPT
    {
        return iterator(data());
    }

    const_iterator begin() const CCC_NOEXCEPT
    {
        return const_iterator(data());
    }

    const_iterator cbegin() const CCC_NOEXCEPT
    {
        return const_iterator(data());
    }

    iterator end() CCC_NOEXCEPT
    {
        return iterator(data() + m_End);
    }

    const_iterator end() const CCC_NOEXCEPT
    {
        return const_iterator(data() + m_End);
    }

    const_iterator cend() const CCC_NOEXCEPT
    {
        return const_iterator(data() + m_End);
    }

    reverse_iterator rbegin() CCC_NOEXCEPT
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const CCC_NOEXCEPT
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() const CCC_NOEXCEPT
    {
        return const_reverse_iterator(cend());
    }

    reverse_iterator rend() CCC_NOEXCEPT
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const CCC_NOEXCEPT
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() const CCC_NOEXCEPT
    {
        return const_reverse_iterator(cbegin());
    }

    bool empty() const CCC_NOEXCEPT
    {
        return 0 == m_End;
    }

    size_type size() const CCC_NOEXCEPT
    {
        return m_End - 0;
    }

    size_type max_size() const CCC_NOEXCEPT
    {
        return m_Storage.capacity();
    }

    void reserve(size_type NewCapacity)
    {
        if (NewCapacity > max_size())
        {
            throw std::length_error("ccc::PodVector::reserve()");
        }
    }

    size_type capacity() const CCC_NOEXCEPT
    {
        return m_Storage.max_size();
    }

    void clear() CCC_NOEXCEPT
    {
        m_Storage.destroy(begin(), end());
        m_End = 0;
    }

    iterator insert(const_iterator Position, value_type const& Value)
    {
        if (size() < max_size())
        {
            m_Storage.construct_default(end());
            ccc::move_backward(const_cast<pointer>(Position), end(), end() + 1, UseRawMemOpsType());
            m_End = m_End + 1;
            *const_cast<pointer>(Position) = Value;
            return iterator(const_cast<pointer>(Position));
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    template <typename IteratorType>
    iterator insert(const_iterator Position, IteratorType First, IteratorType Last)
    {
        difference_type Count = std::distance(First, Last);
        if (Count <= static_cast<difference_type>(max_size() - size()))
        {
            m_Storage.construct_default(end(), Count);
            ccc::move_backward(const_cast<pointer>(Position), end(), end() + Count, UseRawMemOpsType());
            std::copy(First, Last, const_cast<pointer>(Position));
            m_End = m_End + Count;
            return iterator(const_cast<pointer>(Position));
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    iterator insert(const_iterator Position, size_type Count, value_type const& Value)
    {
        if (Count <= max_size() - size())
        {
            m_Storage.construct_default(end(), Count);
            ccc::move_backward(const_cast<pointer>(Position), end(), end() + Count, UseRawMemOpsType());
            std::fill(const_cast<pointer>(Position), const_cast<pointer>(Position) + Count, Value);
            m_End = m_End + Count;
            return iterator(const_cast<pointer>(Position));
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    iterator erase(const_iterator Position)
    {
        ccc::move(const_cast<pointer>(Position) + 1, end(), const_cast<pointer>(Position), UseRawMemOpsType());
        m_End = m_End - 1;
        m_Storage.destroy(end());
        return const_cast<pointer>(Position);
    }

    iterator erase(const_iterator First, const_iterator Last)
    {
        if (First != Last)
        {
            ccc::move(const_cast<pointer>(Last), end(), const_cast<pointer>(First), UseRawMemOpsType());
            m_End = static_cast<size_type>(m_End - std::distance(First, Last));
            m_Storage.destroy(end(), end() + std::distance(First, Last));
        }
        return const_cast<pointer>(First);
    }

    void push_back(value_type const& Value)
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

    void resize(size_type Count)
    {
        if (Count < size())
        {
            m_End = Count;
            m_Storage.destroy(end(), end() + Count);
        }
        else if (Count > size())
        {
            if (Count > max_size())
            {
                throw std::bad_alloc();
            }
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
            if (Count > max_size())
            {
                throw std::bad_alloc();
            }
            m_Storage.construct_and_assign(end(), Count - size(), Value);
            m_End = Count;
        }
    }

    void swap(PodVector& Other)
    {
        using std::swap;
        this->m_Storage.swap(Other.m_Storage);
        swap(this->m_End, Other.m_End);
    }
};

#pragma pack(pop)

}

#endif /* CCC_POD_VECTOR_H_ */
