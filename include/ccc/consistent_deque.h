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

template<class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8, bool UseRawMemOps =
        false, bool StaticStorage = true>
struct ConsistentDeque: public PODDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>
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
    using PODDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_Begin;
    using PODDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_End;
    using PODDeque<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_Storage;
};

}

#endif /* CCC_CONSISTENT_DEQUE_H_ */
