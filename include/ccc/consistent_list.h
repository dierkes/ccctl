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

#pragma pack(push, 16)

template <class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8,
        bool Uninitialized = false, bool Runtime = false>
class ConsistentList: public PodList<T, SizeType, Capacity, Alignment, Uninitialized, Runtime>
{
public:
    ConsistentList()
    {
        this->m_Size = 0;
        this->m_Deallocated = typename PodList<T, SizeType, Capacity, Alignment, Uninitialized,
                Runtime>::deallocated_storage_type();
        this->InitializeAnchor(ccc::integral_constant<bool, Runtime>());
    }

    ~ConsistentList()
    {
        this->clear();
    }

protected:
    using PodList<T, SizeType, Capacity, Alignment, Uninitialized, Runtime>::m_Size;
    using PodList<T, SizeType, Capacity, Alignment, Uninitialized, Runtime>::m_Nodes;
    using PodList<T, SizeType, Capacity, Alignment, Uninitialized, Runtime>::m_Values;
    using PodList<T, SizeType, Capacity, Alignment, Uninitialized, Runtime>::m_Deallocated;
    using PodList<T, SizeType, Capacity, Alignment, Uninitialized, Runtime>::m_Anchor;

private:
    void InitializeAnchor(ccc::integral_constant<bool, true> DynamicAllocation)
    {
        // This is done in the subclass after memory was allocated dynamically
    }

    void InitializeAnchor(ccc::integral_constant<bool, false> DynamicAllocation)
    {
        this->m_Nodes[this->m_Anchor] = typename PodList<T, SizeType, Capacity, Alignment,
                Uninitialized, Runtime>::node_type();
    }
};

#pragma pack(pop)

}

#endif /* CCC_CONSISTENT_LIST_H_ */
