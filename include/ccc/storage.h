/**
 *
 * @file This file contains classes that provide allocated storage.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_STORAGE_H_
#define CCC_STORAGE_H_

#include <ccc/memory.h>
#include <ccc/alignment.h>
#include <ccc/iterator.h>

namespace ccc
{

template<typename T, typename SizeType, SizeType Capacity, unsigned int Alignment = 8>
struct StaticInitializedStorage
{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

#if (CCC_ALIGNAS_AVAILABLE)
    alignas(Alignment) value_type m_StaticInitializedStorage[Capacity];
#else
    PaddedArray<value_type, Capacity, Alignment> m_StaticInitializedStorage;
#endif

    pointer address(reference Object) const
    {
        return addressof(Object);
    }

    const_pointer address(const_reference Object) const
    {
        return addressof(Object);
    }

    size_type max_size() const
    {
        return Capacity;
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position)
    {
        *Position = value_type();
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position, size_type Count)
    {
        std::fill(Position, next(Position, Count), value_type());
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, const_reference Value)
    {
        // storage is already initializes
        *Position = Value;
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, size_type Count, const_reference Value)
    {
        std::fill(Position, next(Position, Count), Value);
    }

    template <typename DestIteratorType, typename SrcIteratorType>
    void construct_and_assign(DestIteratorType Position, SrcIteratorType First, SrcIteratorType Last)
    {
        std::copy(First, Last, Position);
    }

    template <typename IteratorType>
    void destroy(IteratorType Position)
    {
        // do not destroy, since storage is expected to be initializes
    }

    template <typename IteratorType>
    void destroy(IteratorType First, IteratorType Last)
    {
    }

    reference operator[](size_type Index)
    {
        return m_StaticInitializedStorage[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return m_StaticInitializedStorage[Index];
    }
};

template<typename T, typename SizeType, SizeType Capacity, unsigned int Alignment = 8>
struct StaticUninitializedStorage
{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef unsigned char byte_type;

#if (CCC_ALIGNAS_AVAILABLE)
    alignas(Alignment) byte_type m_StaticUninitializedStorage[sizeof(value_type) * Capacity];
#else
    PaddedArray<byte_type, sizeof(value_type) * Capacity, Alignment> m_StaticUninitializedStorage;
#endif

    pointer address(reference Object) const
    {
        return addressof(Object);
    }

    const_pointer address(const_reference Object) const
    {
        return addressof(Object);
    }

    size_type max_size() const
    {
        return Capacity;
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position)
    {
        ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type();
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position, size_type Count)
    {
        for (size_type i = 0; i < Count; ++i, ++Position)
        {
            ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type();
        }
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, const_reference Value)
    {
        ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type(Value);
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, size_type Count, const_reference Value)
    {
        for (size_type i = 0; i < Count; ++i, ++Position)
        {
            ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type(Value);
        }
    }

    template <typename DestIteratorType, typename SrcIteratorType>
    void construct_and_assign(DestIteratorType Position, SrcIteratorType First, SrcIteratorType Last)
    {
        for (; First != Last; ++First, ++Position)
        {
            ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type(*First);
        }
    }

    template <typename IteratorType>
    void destroy(IteratorType Position)
    {
        Position->~value_type();
    }

    template <typename IteratorType>
    void destroy(IteratorType First, IteratorType Last)
    {
        for (; First != Last; ++First)
        {
            First->~value_type();
        }
    }

    reference operator[](size_type Index)
    {
        return reinterpret_cast<pointer>(&m_StaticUninitializedStorage[0])[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return (reinterpret_cast<const_pointer>(&m_StaticUninitializedStorage[0])[Index]);
    }
};

template<typename T, typename SizeType, unsigned int Alignment = 8>
struct FixedInitializedStorage
{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

#if (CCC_ALIGNAS_AVAILABLE)
    alignas(Alignment) size_type m_Capacity;
    alignas(Alignment) value_type* m_FixedInitializedStorage;
#else
    PaddedValue<size_type, Alignment> m_Capacity;
    PaddedValue<value_type*, Alignment> m_FixedInitializedStorage;
#endif

    void allocate(size_type Capacity)
    {
        m_FixedInitializedStorage = new value_type[Capacity];
        m_Capacity = Capacity;
    }

    void deallocate()
    {
        if (m_FixedInitializedStorage)
        {
            delete[] static_cast<pointer>(m_FixedInitializedStorage);
            m_FixedInitializedStorage = 0;
        }
    }

    pointer address(reference Object) const
    {
        return addressof(Object);
    }

    const_pointer address(const_reference Object) const
    {
        return addressof(Object);
    }

    size_type max_size() const
    {
        return m_Capacity;
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position)
    {
        *Position = value_type();
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position, size_type Count)
    {
        std::fill(Position, next(Position, Count), value_type());
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, const_reference Value)
    {
        *Position = Value;
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, size_type Count, const_reference Value)
    {
        std::fill(Position, next(Position, Count), Value);
    }

    template <typename DestIteratorType, typename SrcIteratorType>
    void construct_and_assign(DestIteratorType Position, SrcIteratorType First, SrcIteratorType Last)
    {
        std::copy(First, Last, Position);
    }

    template <typename IteratorType>
    void destroy(IteratorType Position)
    {
        // do not destroy, since storage is expected to be initialized
    }

    template <typename IteratorType>
    void destroy(IteratorType First, IteratorType Last)
    {
    }

    reference operator[](size_type Index)
    {
        return m_FixedInitializedStorage[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return m_FixedInitializedStorage[Index];
    }
};

template<typename T, typename SizeType, unsigned int Alignment = 8>
struct FixedUninitializedStorage
{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

    typedef unsigned char byte_type;

#if (CCC_ALIGNAS_AVAILABLE)
    alignas(Alignment) size_type m_Capacity;
    alignas(Alignment) byte_type* m_FixedUninitializedStorage;
#else
    PaddedValue<size_type, Alignment> m_Capacity;
    PaddedValue<byte_type*, Alignment> m_FixedUninitializedStorage;
#endif

    void allocate(size_type Capacity)
    {
        m_FixedUninitializedStorage = new byte_type[Capacity * sizeof(value_type)];
        m_Capacity = Capacity;
    }

    void deallocate()
    {
        if (m_FixedUninitializedStorage)
        {
            delete[] static_cast<byte_type*>(m_FixedUninitializedStorage);
            m_FixedUninitializedStorage = 0;
        }
    }

    pointer address(reference Object) const
    {
        return addressof(Object);
    }

    const_pointer address(const_reference Object) const
    {
        return addressof(Object);
    }

    size_type max_size() const
    {
        return m_Capacity;
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position)
    {
        ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type();
    }

    template <typename IteratorType>
    void construct_default(IteratorType Position, size_type Count)
    {
        for (size_type i = 0; i < Count; ++i, ++Position)
        {
            ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type();
        }
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, const_reference Value)
    {
        ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type(Value);
    }

    template <typename DestIteratorType>
    void construct_and_assign(DestIteratorType Position, size_type Count, const_reference Value)
    {
        for (size_type i = 0; i < Count; ++i, ++Position)
        {
            ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type(Value);
        }
    }

    template <typename DestIteratorType, typename SrcIteratorType>
    void construct_and_assign(DestIteratorType Position, SrcIteratorType First, SrcIteratorType Last)
    {
        for (; First != Last; ++First, ++Position)
        {
            ::new((void *)(reinterpret_cast<pointer>(&*Position))) value_type(*First);
        }
    }

    template <typename IteratorType>
    void destroy(IteratorType Position)
    {
        Position->~value_type();
    }

    template <typename IteratorType>
    void destroy(IteratorType First, IteratorType Last)
    {
        for (; First != Last; ++First)
        {
            First->~value_type();
        }
    }

    reference operator[](size_type Index)
    {
        return reinterpret_cast<pointer>(&m_FixedUninitializedStorage[0])[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return (reinterpret_cast<const_pointer>(&m_FixedUninitializedStorage[0])[Index]);
    }
};

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment, bool StaticStorage, bool InitializedStorage>
struct Storage
{
    typedef void type;
};

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment>
struct Storage<T, SizeType, Capacity, Alignment, true, true>
{
    typedef StaticInitializedStorage<T, SizeType, Capacity, Alignment> type;
};

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment>
struct Storage<T, SizeType, Capacity, Alignment, true, false>
{
    typedef StaticUninitializedStorage<T, SizeType, Capacity, Alignment> type;
};

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment>
struct Storage<T, SizeType, Capacity, Alignment, false, true>
{
    typedef FixedInitializedStorage<T, SizeType, Alignment> type;
};

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment>
struct Storage<T, SizeType, Capacity, Alignment, false, false>
{
    typedef FixedUninitializedStorage<T, SizeType, Alignment> type;
};

}

#endif /* CCC_STORAGE_H_ */
