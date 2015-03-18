/**
 *
 * @file This file contains the StaticDeque container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_STATIC_DEQUE_H_
#define CCC_STATIC_DEQUE_H_

#include<ccc/consistent_deque.h>

namespace ccc
{

template <typename T, typename SizeType, std::size_t Alignment = 8, bool UseRawMemOps = false>
class StaticDeque : public ConsistentDeque<T, SizeType, 0, Alignment, UseRawMemOps, true>
{
};

}

#endif /* CCC_STATIC_DEQUE_H_ */
