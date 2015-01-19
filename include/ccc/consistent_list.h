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
        this->m_Deallocated = PODStack<typename PODList<T, SizeType, Capacity>::node_index_type, typename PODList<T, SizeType, Capacity>::size_type, Capacity + 1>();
        this->m_Nodes[this->m_Anchor] = typename PODList<T, SizeType, Capacity>::node_type();
    }

    ~ConsistentList()
    {
        // destroy all (valid) elements?
        this->clear();
    }
};

}

#endif /* CCC_CONSISTENT_LIST_H_ */
