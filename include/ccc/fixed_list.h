/**
 *
 * @file This file contains the FixedList container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_FIXED_LIST_H_
#define CCC_FIXED_LIST_H_

#include<ccc/consistent_list.h>

namespace ccc
{

template <typename T, typename SizeType = std::size_t, std::size_t Alignment = 8>
class FixedList : public ConsistentList<T, SizeType, 0, Alignment, false>
{
public:
    explicit FixedList(SizeType Capacity)
    {
        this->m_Nodes.allocate(Capacity + 1);
        this->m_Nodes[this->m_Anchor] = typename PODList<T, SizeType, 0, Alignment, false>::node_type();
        this->m_Values.allocate(Capacity);
        this->m_Deallocated.m_Storage.allocate(Capacity);
    }

    ~FixedList()
    {
        this->clear();
        this->m_Nodes.deallocate();
        this->m_Values.deallocate();
        this->m_Deallocated.m_Storage.deallocate();
    }
};

}

#endif /* CCC_FIXED_LIST_H_ */
