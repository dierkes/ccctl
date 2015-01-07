/**
 *
 * @file This file contains the ConsistentList container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_CONSISTENT_LIST_H_
#define CCC_CONSISTENT_LIST_H_

#include <ccc/pod_list.h>

namespace ccc
{

template<class T, class SizeType, SizeType Capacity>
struct ConsistentList: public PODList<T, SizeType, Capacity>
{
    ConsistentList()
    {
        this->m_Size = 0;
        this->m_Deallocated = PODStack<node_index_type, size_type, Capacity + 1>();
    }

    ~ConsistentList()
    {
        // destroy all (valid) elements?
    }
};

}

#endif /* CCC_CONSISTENT_LIST_H_ */
