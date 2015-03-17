/**
 *
 * @file This file contains the ArrayStack container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_POD_STACK_H_
#define CCC_POD_STACK_H_

#include <ciso646>
#include <cstddef>
#include <cstring>
#include <algorithm>
#include <stdexcept>

#include <ccc/compat.h>
#include <ccc/memory.h>
#include <ccc/type_traits.h>
#include <ccc/alignment.h>

#include <ccc/storage.h>

namespace ccc
{

/**
 * @brief Consistent, static-capacity vector.
 *
 * Constant time: inserting and erasing elements at the end; accessing random elements.
 * Linear time: inserting and erasing elements elsewhere.
 * Noncompliance: No swap method, ...
 */
template <class T, class SizeType, SizeType Capacity, std::size_t Alignment = 8, bool UseRawMemOps = false>
struct PODVector
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

#if (__cplusplus >= 201103L)
    alignas(Alignment) size_type m_End;
#else
    PaddedValue<size_type, Alignment> m_End; // points at the element behind the last valid element
#endif
    StaticInitializedStorage<value_type, SizeType, Capacity, Alignment> m_Storage;

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
//        std::fill(begin(), begin() + Count, Value);
        m_Storage.construct_and_assign(0, Count, Value);
        m_End = m_End + Count;
    }

    template <typename IteratorType>
    void assign(IteratorType First, IteratorType Last)
    {
        clear();
//        std::copy(First, Last, begin());
        m_Storage.construct_and_assign(0, First, Last);
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
        return Capacity;
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

    /**
     * According to the standard, the push and pop methods have no return values.
     * Further, the methods may throw exceptions (e.g., std::bad_alloc if an allocation fails).
     *
     * We ...
     */

//     A vector, as defined by the standard, does not have the methods push_front and pop_front.
//    void push_front(const_reference Value)
//    {
//        if (size() < Capacity)
//        {
//            if (UseRawMemOps)
//            {
//                std::memmove(data(1), data(0), size() * sizeof(value_type));
//            }
//            else
//            {
//                std::copy_backward(begin(), end(), end() + 1);
//            }
//            m_End = m_End + 1;
//            m_Array[m_Begin] = Value;
//        }
//        else
//        {
//            throw std::bad_alloc();
//        }
//    }

    void push_back(const_reference Value)
    {
        if (size() < Capacity)
        {
            m_Storage.construct_and_assign(m_End, Value);
            m_End = m_End + 1;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

//    void emplace_front()
//    {
//        if (size() < Capacity)
//        {
//            if (UseRawMemOps)
//            {
//                std::memmove(data(1), data(0), size() * sizeof(value_type));
//            }
//            else
//            {
//                std::copy_backward(begin(), end(), end() + 1);
//            }
//            m_End = m_End + 1;
//            m_Array[m_Begin] = value_type();
//        }
//        else
//        {
//            throw std::bad_alloc();
//        }
//    }

    void emplace_back()
    {
        if (size() < Capacity)
        {
            m_Storage.construct_default(m_End);
            m_End = m_End + 1;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

//    void pop_front()
//    {
//        if (not empty())
//        {
//            // ToDo: Do we have to destroy the element?
//            m_Array[m_Begin] = value_type(); // destroy element by overwriting
//            if (UseRawMemOps)
//            {
//                std::memmove(data(0), data(1), (size() - 1) * sizeof(value_type));
//            }
//            else
//            {
//                std::copy(begin() + 1, end(), begin());
//            }
//            m_End = m_End - 1;
//        }
//    }

    void pop_back()
    {
        if (not empty())
        {
            // ToDo: Do we have to destroy the element?
//            m_Array[m_End - 1] = value_type(); // destroy element by overwriting
            m_Storage.destroy(m_End - 1);
            m_End = m_End - 1;
        }
    }

    iterator insert(size_type Position, const_reference Value)
    {
        return insert(iterator(ccc::addressof(m_Storage[Position])), Value);
    }

    /**
     *
     * Note:
     */
    iterator insert(iterator Position, const_reference Value)
    {
        if (size() < Capacity) // rules out case LogicalBegin == LogicalEnd
        {
            // Do we need to construct the element to which we shift the other ones?
            // Well, the object at Position is already constructed, so probably yes. Then the
            // behavior is well defined, as all elements are constructed
            m_Storage.construct_default(m_End);
            if (UseRawMemOps)
            {
                std::memmove(Position + 1, Position, (end() - Position) * sizeof(value_type));
            }
            else
            {
                std::copy_backward(Position, end(), end() + 1);
            }
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
        if (Count <= Capacity - size())
        {
            m_Storage.construct_default(m_End, Count);
            if (UseRawMemOps)
            {
                std::memmove(Position + Count, Position, (end() - Position) * sizeof(value_type));
            }
            else
            {
                std::copy_backward(Position, end(), end() + Count);
            }
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
        if (Count <= Capacity - size())
        {
            m_Storage.construct_default(m_End, Count);
            if (UseRawMemOps)
            {
                std::memmove(Position + Count, Position, (end() - Position) * sizeof(value_type));
            }
            else
            {
                std::copy_backward(Position, end(), end() + Count);
            }
            std::fill(Position, Position + Count, Value);
            m_End = m_End + Count;
            return Position;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    // emplace is a bad idea, if we have no default constructor
    iterator emplace(iterator Position)
    {
        if (size() < Capacity) // rules out case LogicalBegin == LogicalEnd
        {
            m_Storage.construct_default(m_End);
            if (UseRawMemOps)
            {
                std::memmove(Position + 1, Position, (end() - Position) * sizeof(value_type));
            }
            else
            {
                std::copy_backward(Position, end(), end() + 1);
            }
            m_End = m_End + 1;
            *Position = value_type();
            return Position;
        }
        else
        {
            throw std::bad_alloc();
        }
    }

    /**
     *
     * Note: According to the standard, passing an invalid iterator causes undefinied behavior.
     */
    iterator erase(iterator Position)
    {
        if (Position < end())
        {
            // (*Position).~T(); // call destructor of the object at Position (might not work for basic types)
//            *Position = value_type();
            if (UseRawMemOps)
            {
                std::memmove(Position, Position + 1, (end() - Position + 1) * sizeof(value_type));
            }
            else
            {
                std::copy(Position + 1, end(), Position);
            }
            m_End = m_End - 1;
            m_Storage.destroy(m_End);
            return Position;
        }
        else
        {
            return end();
        }
    }

    iterator erase(iterator First, iterator Last)
    {
        if (UseRawMemOps)
        {
            std::memmove(First, Last, std::distance(Last, end()) * sizeof(value_type));
        }
        else
        {
            std::copy(Last, end(), First);
        }
        m_End = m_End - std::distance(First, Last);
        m_Storage.destroy(end(), end() + std::distance(First, Last));
        return First;
    }

};

}

#endif /* CCC_POD_STACK_H_ */
