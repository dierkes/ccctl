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

#include <algorithm>

#include<ccc/consistent_list.h>

namespace ccc
{

template <typename T, typename SizeType = unsigned int, unsigned int Alignment = 8>
class FixedList : public ConsistentList<T, SizeType, 0, Alignment, false, true>
{
public:
    explicit FixedList(SizeType Capacity)
    {
        this->m_Nodes.allocate(Capacity + 1);
        this->m_Nodes[this->m_Anchor] = typename FixedList::node_type();
        this->m_Values.allocate(Capacity);
        this->m_Deallocated.m_Storage.allocate(Capacity);
    }

    FixedList(FixedList const& Other)
    {
        this->m_Nodes.allocate(Other.m_Nodes.m_Capacity);
        this->m_Nodes[this->m_Anchor] = typename FixedList::node_type();
        this->m_Values.allocate(Other.m_Values.m_Capacity);
        this->m_Deallocated.m_Storage.allocate(Other.m_Deallocated.m_Storage.m_Capacity);
        this->assign(Other.begin(), Other.end());
    }

    ~FixedList()
    {
        this->clear();
    }

    void operator=(FixedList const& Other)
    {
        if (this->m_Storage.m_Capacity != Other.m_Storage.m_Capacity)
        {
            FixedList Tmp(Other.m_Values.m_Capacity);
            this->swap(Tmp);
        }
        this->assign(Other.begin(), Other.end());
    }
};

}

#endif /* CCC_FIXED_LIST_H_ */
