/**
 *
 * @file This file contains the Array container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_FIXED_ARRAY_H_
#define CCC_FIXED_ARRAY_H_

#include <stdexcept>

#include <iterator>

#include <ccc/compat.h>
#include <ccc/memory.h>

namespace ccc
{

/**
 * @brief Consistent, fixed-capacity array.
 *
 * Analogous to std::array.
 */
template <class T, class SizeType = unsigned int>
struct FixedArray
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

    value_type* m_Array; // allow to change pointer to swap
    size_type m_Capacity; // allow to change size to swap

    FixedArray(size_type Capacity) : m_Capacity(Capacity), m_Array(0)
    {
        if (Capacity > 0) // allow zero-size arrays
        {
            m_Array = new value_type[Capacity];
        }
    }

    ~FixedArray()
    {
        delete[] m_Array;
    }

    pointer data() CCC_NOEXCEPT
    {
        return ccc::addressof(m_Array[0]);
    }

    const_pointer data() const CCC_NOEXCEPT
    {
        return ccc::addressof(m_Array[0]);
    }

    iterator begin() CCC_NOEXCEPT
    {
        return iterator(data());
    }

    const_iterator begin() const CCC_NOEXCEPT
    {
        return const_iterator(data());
    }

    iterator end() CCC_NOEXCEPT
    {
        return iterator(data() + m_Capacity);
    }

    const_iterator end() const CCC_NOEXCEPT
    {
        return const_iterator(data() + m_Capacity);
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
        return m_Capacity;
    }

    CCC_CONSTEXPR
    size_type max_size() const CCC_NOEXCEPT
    {
        return m_Capacity;
    }

    CCC_CONSTEXPR
    bool empty() const CCC_NOEXCEPT
    {
        return size() == 0;
    }

    reference operator[](size_type Index)
    {
        return m_Array[Index];
    }

    CCC_CONSTEXPR
    const_reference operator[](size_type Index) const CCC_NOEXCEPT
    {
        return m_Array[Index];
    }

    reference at(size_type Index)
    {
        if (Index >= m_Capacity)
        {
            throw std::out_of_range("FixedArray::at"), m_Array[0];
        }
        return m_Array[Index];
    }

    CCC_CONSTEXPR
    const_reference at(size_type Index) const
    {
        return Index < m_Capacity ? m_Array[Index] : (throw std::out_of_range("FixedArray::at"), m_Array[0]);
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
        return m_Capacity ? *(end() - 1) : *end();
    }

    const_reference back() const
    {
        return m_Capacity ? *(end() - 1) : *end();
    }

    void fill(const_reference Value)
    {
        for (iterator it = begin(); it != end(); ++it)
        {
            *it = Value;
        }
    }

};

}

#endif /* CCC_FIXED_ARRAY_H_ */
