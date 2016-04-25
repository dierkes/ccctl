/*
 * fixed_deque.h
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */

#ifndef CCC_FIXED_DEQUE_H_
#define CCC_FIXED_DEQUE_H_

#include <algorithm>

#include <ccc/consistent_deque.h>

namespace ccc
{

template <typename T, typename SizeType = unsigned int, unsigned int Alignment = 8, bool UseRawMemOps = false>
class FixedDeque : public ConsistentDeque<T, SizeType, 0, Alignment, UseRawMemOps, false, true>
{
public:
    explicit FixedDeque(SizeType Capacity)
    {
        this->m_Storage.allocate(Capacity + 1);
    }

    FixedDeque(FixedDeque const& Other)
    {
        this->m_Storage.allocate(Other.m_Storage.m_Capacity);
        this->assign(Other.begin(), Other.end());
    }

    void operator=(FixedDeque const& Other)
    {
        if (this->m_Storage.m_Capacity != Other.m_Storage.m_Capacity)
        {
            FixedDeque Tmp(Other.m_Storage.m_Capacity);
            this->swap(Tmp);
        }
        this->assign(Other.begin(), Other.end());
    }

    ~FixedDeque()
    {
        this->clear();
    }
};

}

#endif /* CCC_FIXED_DEQUE_H_ */
