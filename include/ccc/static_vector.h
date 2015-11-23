/**
 *
 * @file This file contains the StaticVector container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_STATIC_VECTOR_H_
#define CCC_STATIC_VECTOR_H_

#include<ccc/consistent_vector.h>

namespace ccc
{

#pragma pack(push, 1)

template <typename T, typename SizeType, SizeType Capacity, unsigned int Alignment = 8, bool UseRawMemOps = false>
class StaticVector : public ConsistentVector<T, SizeType, Capacity, Alignment, UseRawMemOps, true>
{
};

#pragma pack(pop)

}

#endif /* CCC_STATIC_VECTOR_H_ */
