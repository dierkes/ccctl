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

template<typename T, typename SizeType, SizeType Capacity, std::size_t Alignment = 8>
struct StaticInitializedStorage
{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

#if (__cplusplus >= 201103L)
    alignas(Alignment) value_type m_Array[Capacity];
#else
    PaddedArray<value_type, Capacity, Alignment> m_Array;
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

    void construct_default(size_type Index)
    {
        m_Array[Index] = value_type();
    }

    void construct_default(size_type Index, size_type Count)
    {
        // storage is already initializes
        std::fill(&m_Array[Index], &m_Array[Index+Count], value_type());
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

    void construct_and_assign(size_type Index, const_reference Value)
    {
        // storage is already initializes
        m_Array[Index] = Value;
    }

    void construct_and_assign(size_type Index, size_type Count, const_reference Value)
    {
        // storage is already initializes
        std::fill(&m_Array[Index], &m_Array[Index+Count], Value);
    }

    template <typename IteratorType>
    void construct_and_assign(size_type Index, IteratorType First, IteratorType Last)
    {
        // storage is already initialized
        std::copy(First, Last, &m_Array[Index]);
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

    void destroy(size_type Index)
    {
        // do not destroy, since storage is expected to be initializes
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
        return m_Array[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return m_Array[Index];
    }
};

template<typename T, typename SizeType, SizeType Capacity, std::size_t Alignment = 8>
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

#if (__cplusplus >= 201103L)
    alignas(Alignment) byte_type m_Array[sizeof(value_type) * Capacity];
#else
    PaddedArray<byte_type, sizeof(value_type) * Capacity, Alignment> m_Array;
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

    void construct_and_assign(size_type Index, const_reference Value)
    {
        ::new((void *)(&reinterpret_cast<pointer>(&m_Array[0])[Index])) value_type(Value);
    }

    void construct_and_assign(size_type Index, size_type Count, const_reference Value)
    {
        for (size_type i = 0; i < Count; ++i, ++Index)
        {
            ::new((void *)(&reinterpret_cast<pointer>(&m_Array[0])[Index])) value_type(Value);
        }
    }

    template <typename IteratorType>
    void construct_and_assign(size_type Index, IteratorType First, IteratorType Last)
    {
        for (; First != Last; ++First, ++Index)
        {
            ::new((void *)(&reinterpret_cast<pointer>(&m_Array[0])[Index])) value_type(*First);
        }
    }

    void destroy(size_type Index)
    {
        (&reinterpret_cast<pointer>(&m_Array[0])[Index])->~value_type();
    }

    template <typename IteratorType>
    void destroy(IteratorType Position)
    {
        reinterpret_cast<pointer>(address(*Position))->~value_type();
    }

    template <typename IteratorType>
    void destroy(IteratorType First, IteratorType Last)
    {
        for (; First != Last; ++First)
        {
            (reinterpret_cast<pointer>(&*First))->~value_type();
        }
    }

    reference operator[](size_type Index)
    {
        return reinterpret_cast<pointer>(&m_Array[0])[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return (reinterpret_cast<const_pointer>(&m_Array[0])[Index]);
    }
};

template<typename T, typename SizeType, std::size_t Alignment = 8>
struct FixedInitializedStorage
{
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef SizeType size_type;
    typedef std::ptrdiff_t difference_type;

#if (__cplusplus >= 201103L)
    alignas(Alignment) size_type m_Capacity;
    alignas(Alignment) value_type* m_Array;
#else
    PaddedValue<size_type, Alignment> m_Capacity;
    PaddedValue<value_type*, Alignment> m_Array;
#endif

    explicit FixedInitializedStorage(size_type Capacity)
    {
        m_Capacity = Capacity;
        m_Array = new value_type[m_Capacity];
    }

    ~FixedInitializedStorage()
    {
        delete[] m_Array;
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

    void construct_default(size_type Index)
    {
        m_Array[Index] = value_type();
    }

    void construct_default(size_type Index, size_type Count)
    {
        // storage is already initializes
        std::fill(&m_Array[Index], &m_Array[Index+Count], value_type());
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

    void construct_and_assign(size_type Index, const_reference Value)
    {
        // storage is already initializes
        m_Array[Index] = Value;
    }

    void construct_and_assign(size_type Index, size_type Count, const_reference Value)
    {
        // storage is already initializes
        std::fill(&m_Array[Index], &m_Array[Index+Count], Value);
    }

    template <typename IteratorType>
    void construct_and_assign(size_type Index, IteratorType First, IteratorType Last)
    {
        // storage is already initialized
        std::copy(First, Last, &m_Array[Index]);
    }

    template <typename DestIteratorType, typename SrcIteratorType>
    void construct_and_assign(DestIteratorType Position, size_type Count, const_reference Value)
    {
        std::fill(Position, next(Position, Count), Value);
    }

    template <typename DestIteratorType, typename SrcIteratorType>
    void construct_and_assign(DestIteratorType Position, SrcIteratorType First, SrcIteratorType Last)
    {
        std::copy(First, Last, Position);
    }

    void destroy(size_type Index)
    {
        // do not destroy, since storage is expected to be initialized
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
        return m_Array[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return m_Array[Index];
    }
};

template<typename T, typename SizeType, std::size_t Alignment = 8>
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

#if (__cplusplus >= 201103L)
    alignas(Alignment) size_type m_Capacity;
    alignas(Alignment) byte_type* m_Array;
#else
    PaddedValue<size_type, Alignment> m_Capacity;
    PaddedValue<byte_type*, Alignment> m_Array;
#endif

    explicit FixedUninitializedStorage(size_type Capacity)
    {
        m_Capacity = Capacity;
        m_Array = new byte_type[m_Capacity * sizeof(value_type)];
    }

    ~FixedUninitializedStorage()
    {
        delete[] m_Array;
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

    void construct_and_assign(size_type Index, const_reference Value)
    {
        ::new((void *)(&reinterpret_cast<pointer>(&m_Array[0])[Index])) value_type(Value);
    }

    void construct_and_assign(size_type Index, size_type Count, const_reference Value)
    {
        for (size_type i = 0; i < Count; ++i, ++Index)
        {
            ::new((void *)(&reinterpret_cast<pointer>(&m_Array[0])[Index])) value_type(Value);
        }
    }

    template <typename IteratorType>
    void construct_and_assign(size_type Index, IteratorType First, IteratorType Last)
    {
        for (; First != Last; ++First, ++Index)
        {
            ::new((void *)(&reinterpret_cast<pointer>(&m_Array[0])[Index])) value_type(*First);
        }
    }

    void destroy(size_type Index)
    {
        (&reinterpret_cast<pointer>(&m_Array[0])[Index])->~value_type();
    }

    template <typename IteratorType>
    void destroy(IteratorType Position)
    {
        reinterpret_cast<pointer>(address(*Position))->~value_type();
    }

    template <typename IteratorType>
    void destroy(IteratorType First, IteratorType Last)
    {
        for (; First != Last; ++First)
        {
            (reinterpret_cast<pointer>(&*First))->~value_type();
        }
    }

    reference operator[](size_type Index)
    {
        return reinterpret_cast<pointer>(&m_Array[0])[Index];
    }

    const_reference operator[](size_type Index) const
    {
        return (reinterpret_cast<const_pointer>(&m_Array[0])[Index]);
    }
};

template <typename T, typename SizeType, SizeType Capacity, std::size_t Alignment, bool StaticStorage, bool InitializedStorage>
struct StorageType
{
    typedef void type;
};

template <typename T, typename SizeType, SizeType Capacity, std::size_t Alignment>
struct StorageType<T, SizeType, Capacity, Alignment, true, true>
{
    typedef StaticInitializedStorage<T, SizeType, Capacity, Alignment> type;
};

template <typename T, typename SizeType, SizeType Capacity, std::size_t Alignment>
struct StorageType<T, SizeType, Capacity, Alignment, true, false>
{
    typedef StaticUninitializedStorage<T, SizeType, Capacity, Alignment> type;
};

template <typename T, typename SizeType, SizeType Capacity, std::size_t Alignment>
struct StorageType<T, SizeType, Capacity, Alignment, false, true>
{
    typedef FixedInitializedStorage<T, SizeType, Alignment> type;
};

template <typename T, typename SizeType, SizeType Capacity, std::size_t Alignment>
struct StorageType<T, SizeType, Capacity, Alignment, false, false>
{
    typedef FixedUninitializedStorage<T, SizeType, Alignment> type;
};

}

#endif /* CCC_STORAGE_H_ */
