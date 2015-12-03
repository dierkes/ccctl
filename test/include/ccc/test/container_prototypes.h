/*
 * container_prototypes.h
 *
 *  Created on: Nov 29, 2015
 *      Author: dierkes
 */

#ifndef CCC_TEST_CONTAINER_PROTOTYPES_H_
#define CCC_TEST_CONTAINER_PROTOTYPES_H_

#include <ccc/compat.h>

namespace ccc
{

#pragma pack(push, 16)

template <typename T, typename SizeType, SizeType Capacity>
struct ContainerPlain
{
    SizeType m_End;
    T m_Storage[Capacity];
};

#if CCC_ALIGNAS_AVAILABLE

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment>
struct ContainerAlignas
{
    alignas(Alignment) SizeType m_End;
    alignas(Alignment) T m_Storage[Capacity];
};

#endif

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment>
struct ContainerAligned
{
    typename ccc::Aligned<SizeType, Alignment>::type m_End;
    typename ccc::Aligned<T[Capacity], Alignment>::type m_Storage;
};

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment>
struct ContainerPadded
{
    ccc::PaddedValue<SizeType, Alignment> m_End;
    ccc::PaddedArray<T, Capacity, Alignment> m_Storage;
};

#pragma pack(pop)

}



#endif /* CCC_TEST_CONTAINER_PROTOTYPES_H_ */
