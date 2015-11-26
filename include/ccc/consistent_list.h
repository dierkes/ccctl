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

template<class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8, bool StaticStorage = true>
class ConsistentList: public PodList<T, SizeType, Capacity, Alignment, StaticStorage>
{
public:
    ConsistentList()
    {
        this->m_Size = 0;
        this->m_Deallocated = typename PodList<T, SizeType, Capacity, Alignment, StaticStorage>::deallocated_storage_type();
        if (StaticStorage)
        {
            this->m_Nodes[this->m_Anchor] = typename PodList<T, SizeType, Capacity, Alignment, StaticStorage>::node_type();
        }
    }

    ~ConsistentList()
    {
        this->clear();
    }

protected:
    using PodList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Size;
    using PodList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Nodes;
    using PodList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Values;
    using PodList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Deallocated;
    using PodList<T, SizeType, Capacity, Alignment, StaticStorage>::m_Anchor;
};

}

#endif /* CCC_CONSISTENT_LIST_H_ */
