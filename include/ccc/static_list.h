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

template <class T, class SizeType, SizeType Capacity>
struct StaticList: public ConsistentList<T, SizeType, Capacity>
{
protected:
    using ConsistentList<T, SizeType, Capacity>::m_Size;
    using ConsistentList<T, SizeType, Capacity>::m_Nodes;
    using ConsistentList<T, SizeType, Capacity>::m_Values;
    using ConsistentList<T, SizeType, Capacity>::m_Deallocated;
    using ConsistentList<T, SizeType, Capacity>::m_Anchor;
};

}

#endif /* CCC_STATIC_LIST_H_ */
