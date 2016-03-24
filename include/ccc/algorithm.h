/*
 * algorithm.h
 *
 *  Created on: Mar 5, 2016
 *      Author: dierkes
 */

#ifndef CCC_ALGORITHM_H_
#define CCC_ALGORITHM_H_

#include <algorithm>
#include <iterator>

#include <ccc/type_traits.h>
#include <ccc/iterator.h>

namespace ccc
{

template<class InputIt, class OutputIt>
OutputIt copy(InputIt First, InputIt Last, OutputIt DestFirst,
        ccc::integral_constant<bool, false>::type)
{
    return std::copy(First, Last, DestFirst);
}

template<class InputIt, class OutputIt>
OutputIt move_backward(InputIt First, InputIt Last, OutputIt DestLast,
        ccc::integral_constant<bool, true>::type)
{
    const typename std::iterator_traits<InputIt>::difference_type Distance = std::distance(First, Last);
    const OutputIt DestFirst = DestLast - Distance;
    std::memmove(ccc::addressof(*DestFirst), ccc::addressof(*First),
            Distance * sizeof(typename std::iterator_traits<InputIt>::value_type));
    return DestFirst;
}

template<class InputIt, class OutputIt>
OutputIt move_backward(InputIt First, InputIt Last, OutputIt DestLast,
        ccc::integral_constant<bool, false>::type)
{
    return std::copy_backward(First, Last, DestLast);

}

template<class InputIt, class OutputIt>
OutputIt move(InputIt First, InputIt Last, OutputIt DestFirst,
        ccc::integral_constant<bool, true>::type)
{
    const typename std::iterator_traits<InputIt>::difference_type Distance = std::distance(First, Last);
    std::memmove(ccc::addressof(*DestFirst), ccc::addressof(*First),
            Distance * sizeof(typename std::iterator_traits<InputIt>::value_type));
    return DestFirst + Distance;
}

template<class InputIt, class OutputIt>
OutputIt move(InputIt First, InputIt Last, OutputIt DestFirst,
        ccc::integral_constant<bool, false>::type)
{
    return std::copy(First, Last, DestFirst);
}

}

#endif /* CCC_ALGORITHM_H_ */
