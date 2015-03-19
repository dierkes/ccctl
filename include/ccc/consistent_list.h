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

template<class T, class SizeType, SizeType Capacity, std::size_t Alignment = 8, bool StaticStorage = true>
class ConsistentList: public PODList<T, SizeType, Capacity, Alignment, StaticStorage>
{
public:
    ConsistentList()
    {
        this->m_Size = 0;
        this->m_Deallocated = typename PODList<T, SizeType, Capacity, Alignment, StaticStorage>::deallocated_storage_type();
        if (StaticStorage)
        {
            this->m_Nodes[this->m_Anchor] = typename PODList<T, SizeType, Capacity, Alignment, StaticStorage>::node_type();
        }
    }

    ~ConsistentList()
    {
        this->clear();
    }

protected:
    using PODList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Size;
    using PODList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Nodes;
    using PODList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Values;
    using PODList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Deallocated;
    using PODList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Anchor;
};

}

#endif /* CCC_CONSISTENT_LIST_H_ */
