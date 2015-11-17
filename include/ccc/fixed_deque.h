/*
 * fixed_deque.h
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */

#ifndef CCC_FIXED_DEQUE_H_
#define CCC_FIXED_DEQUE_H_

#include <ccc/consistent_deque.h>

namespace ccc
{

template <typename T, typename SizeType = unsigned int, unsigned int Alignment = 8, bool UseRawMemOps = false>
class FixedDeque : public ConsistentDeque<T, SizeType, 0, Alignment, UseRawMemOps, false>
{
public:
    explicit FixedDeque(SizeType Capacity)
    {
        this->m_Storage.allocate(Capacity + 1);
    }

    ~FixedDeque()
    {
        this->clear();
        this->m_Storage.deallocate();
    }
};

}

#endif /* CCC_FIXED_DEQUE_H_ */
