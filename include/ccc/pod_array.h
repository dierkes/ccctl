/**
 *
 * @file This file contains the ArrayStack container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_POD_ARRAY_H_
#define CCC_POD_ARRAY_H_

#include <ciso646>
#include <cstddef>
#include <cstring>
#include <stdexcept>

#include <ccc/compat.h>
#include <ccc/memory.h>
#include <ccc/type_traits.h>
#include <ccc/alignment.h>

#include <ccc/storage.h>

namespace ccc
{

/**
 * @brief Consistent, static-capacity array.
 *
 */
template<typename T, typename SizeType, SizeType Capacity, std::size_t Alignment = 8,
        bool UseRawMemOps = false, bool StaticStorage = true>
struct PODArray
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

    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

    // support zero-sized arrays (see std::array in gcc)
    typedef typename Storage<T, SizeType, Capacity ? Capacity : 1, Alignment, StaticStorage,
            true>::type storage_type;
    storage_type m_Storage;

    pointer data() CCC_NOEXCEPT
    {
        return ccc::addressof(m_Storage[0]);
    }

    const_pointer data() const CCC_NOEXCEPT
    {
        return ccc::addressof(m_Storage[0]);
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
        return iterator(data() + m_Storage.max_size());
    }

    const_iterator end() const CCC_NOEXCEPT
    {
        return const_iterator(data() + m_Storage.max_size());
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
        return m_Storage.max_size();
    }

    CCC_CONSTEXPR
    size_type max_size() const CCC_NOEXCEPT
    {
        return m_Storage.max_size();
    }

    CCC_CONSTEXPR
    bool empty() const CCC_NOEXCEPT
    {
        return size() == 0;
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
        if (Index >= m_Storage.max_size())
        {
            throw std::out_of_range("ConsistentArray::at"), m_Storage[0];
        }
        return m_Storage[Index];
    }

    CCC_CONSTEXPR
    const_reference at(size_type Index) const
    {
        return Index < m_Storage.max_size() ? m_Storage[Index] : (throw std::out_of_range("ConsistentArray::at"), m_Storage[0]);
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
        return m_Storage.max_size() ? *(end() - 1) : *end();
    }

    const_reference back() const
    {
        return m_Storage.max_size() ? *(end() - 1) : *end();
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

#endif /* CCC_POD_ARRAY_H_ */
