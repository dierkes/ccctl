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

namespace ccc
{

template <typename T, std::size_t Alignment, std::size_t Size = sizeof(T), std::size_t PaddedBytes = Alignment - (Size % Alignment)>
struct Padded
{
    T Data;
    char Padding[PaddedBytes];
};

template <typename T, std::size_t Alignment, std::size_t Size>
struct Padded<T, Alignment, Size, Alignment>
{
    T Data;
};

template <typename T, std::size_t Capacity, std::size_t Alignment>
struct PaddedArray
{
    Padded<T[Capacity], Alignment> m_PaddedArray;

    T& operator[](std::size_t Index)
    {
        return m_PaddedArray.Data[Index];
    }

    const T& operator[](std::size_t Index) const
    {
        return m_PaddedArray.Data[Index];
    }
};

template <typename T, std::size_t Alignment>
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

}



#endif /* CCC_ALIGNMENT_H_ */
