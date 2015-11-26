/**
 *
 * @file This file contains the ConsistentStack container.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_CONSISTENT_STACK_H_
#define CCC_CONSISTENT_STACK_H_

#include <ccc/pod_vector.h>

namespace ccc
{

#pragma pack(push, 1)

template<class T, class SizeType, SizeType Capacity, unsigned int Alignment = 8, bool UseRawMemOps =
        false, bool StaticStorage = true>
struct ConsistentVector: public PodVector<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>
{
    ConsistentVector()
    {
        this->m_End = 0;
    }

    ~ConsistentVector()
    {
        this->clear();
    }

protected:
    using PodVector<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_End;
    using PodVector<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_Storage;
};

#pragma pack(pop)

}

#endif /* CCC_CONSISTENT_STACK_H_ */
