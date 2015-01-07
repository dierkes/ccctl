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

#include <ccc/pod_stack.h>

namespace ccc
{

template<class T, class SizeType, SizeType Capacity, bool UseRawMemOps =
        ccc::is_trivially_copyable_guaranteed<T>::value>
struct ConsistentStack: public PODStack<T, SizeType, Capacity, UseRawMemOps>
{
    ConsistentStack()
    {
        this->m_End = 0;
    }

    ~ConsistentStack()
    {
        // destroy all (valid) elements?
    }
};

}

#endif /* CCC_CONSISTENT_STACK_H_ */
