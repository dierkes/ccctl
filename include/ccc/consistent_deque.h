/**
 *
 * @file This file contains the ConsistentDeque container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_CONSISTENT_DEQUE_H_
#define CCC_CONSISTENT_DEQUE_H_

#include <ccc/pod_deque.h>

namespace ccc
{

#pragma pack(push, 1)

template<class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8, bool UseRawMemOps =
        false, bool StaticStorage = true>
struct ConsistentDeque: public PodDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>
{
    ConsistentDeque()
    {
        this->m_Begin = 0;
        this->m_End = 0;
    }

    ~ConsistentDeque()
    {
        this->clear();
    }

protected:
    using PodDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_Begin;
    using PodDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_End;
    using PodDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_Storage;
};

#pragma pack(pop)

}

#endif /* CCC_CONSISTENT_DEQUE_H_ */
