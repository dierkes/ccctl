/**
 *
 * @file This file contains the ConsistentList container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_STATIC_LIST_H_
#define CCC_STATIC_LIST_H_

#include <ccc/consistent_list.h>

namespace ccc
{

template <class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8>
struct StaticList: public ConsistentList<T, SizeType, Capacity, Alignment, true>
{
};

}

#endif /* CCC_STATIC_LIST_H_ */
