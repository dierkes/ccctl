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

template <typename T, typename SizeType, SizeType Capacity, std::size_t Alignment = 8, bool UseRawMemOps = false>
class StaticVector : public ConsistentVector<T, SizeType, Capacity, Alignment, UseRawMemOps, true>
{
};

}

#endif /* CCC_STATIC_VECTOR_H_ */
