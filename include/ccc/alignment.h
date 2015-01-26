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

template <typename T, std::size_t Alignment, std::size_t PaddedBytes = Alignment - (sizeof(T) % Alignment)>
struct Padded
{
    T Value;
    char Padding[PaddedBytes];
};

template <typename T, std::size_t Alignment>
struct Padded<T, Alignment, Alignment>
{
    T Value;
};

template <typename T, std::size_t Capacity, std::size_t Alignment>
struct PaddedArray
{
    Padded<T[Capacity], Alignment> m_Array;

    T& operator[](std::size_t Index)
    {
        return m_Array.Value[Index];
    }

    const T& operator[](std::size_t Index) const
    {
        return m_Array.Value[Index];
    }
};

template <typename T, std::size_t Alignment>
struct PaddedValue
{
    Padded<T, Alignment> m_Value;

    T& operator=(const T& Value)
    {
        m_Value.Value = Value;
        return m_Value.Value;
    }

    operator T() const
    {
        return m_Value.Value;
    }
};

}



#endif /* CCC_ALIGNMENT_H_ */
