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

template<class T, class SizeType, SizeType Capacity, std::size_t Alignment = 8, bool UseRawMemOps =
        false, bool StaticStorage = true>
struct ConsistentVector: public PODVector<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>
{
    ConsistentVector()
    {
        this->m_End = 0;
    }

    ~ConsistentVector()
    {
        // destroy all (valid) elements?
    }

protected:
    using PODVector<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_End;
    using PODVector<T, SizeType, Capacity, Alignment, UseRawMemOps, StaticStorage>::m_Storage;
};

}

#endif /* CCC_CONSISTENT_STACK_H_ */
