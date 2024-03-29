/**
 *
 * @file The file contains macro definitions necessary for compatibility across different compilers and platforms.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_ALIGNMENT_H_
#define CCC_ALIGNMENT_H_

#include <cstddef>

#include <ccc/compat.h>

namespace ccc
{

#pragma pack(push, 16)

template <typename T, unsigned int Alignment, unsigned int Size = sizeof(T), unsigned int PaddedBytes = Alignment - (Size % Alignment)>
struct Padded
{
    T Data;
    char Padding[PaddedBytes];
};

template <typename T, unsigned int Alignment, unsigned int Size>
struct Padded<T, Alignment, Size, Alignment>
{
    T Data;
};

template <typename T, unsigned int Capacity, unsigned int Alignment>
struct PaddedArray
{
    Padded<T[Capacity], Alignment> m_PaddedArray;

    T& operator[](unsigned int Index)
    {
        return m_PaddedArray.Data[Index];
    }

    const T& operator[](unsigned int Index) const
    {
        return m_PaddedArray.Data[Index];
    }
};

template <typename T, unsigned int Alignment>
struct PaddedValue
{
    Padded<T, Alignment> m_PaddedValue;

    T& operator=(const T& Value)
    {
        m_PaddedValue.Data = Value;
        return m_PaddedValue.Data;
    }

    operator T() const
    {
        return m_PaddedValue.Data;
    }
};

template <typename T, unsigned int Alignment>
struct Aligned
{
    typedef void type;
};

template <typename T>
struct Aligned<T, 0>
{
    typedef T type;
};

template <typename T>
struct Aligned<T, 1>
{
    typedef CCC_ALIGNED(T, 1) type;
};

template <typename T>
struct Aligned<T, 2>
{
    typedef CCC_ALIGNED(T, 2) type;
};

template <typename T>
struct Aligned<T, 4>
{
    typedef CCC_ALIGNED(T, 4) type;
};

template <typename T>
struct Aligned<T, 8>
{
    typedef CCC_ALIGNED(T, 8) type;
};

template <typename T>
struct Aligned<T, 16>
{
    typedef CCC_ALIGNED(T, 16) type;
};

#pragma pack(pop)

}

#endif /* CCC_ALIGNMENT_H_ */
