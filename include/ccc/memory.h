/**
 *
 * @file Contains functions similar/analogous to those defined in the standard's memory header.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_MEMORY_H_
#define CCC_MEMORY_H_

namespace ccc
{

/**
 * Return the address of the passed object, even if its &-operater is overloaded.
 */
template <typename T>
inline
T* addressof(T& r)
{
    return reinterpret_cast<T*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(r)));
}

}

#define CCC_OFFSETOF(T, m) reinterpret_cast<uint64_t>(&reinterpret_cast<char const volatile&>(reinterpret_cast<T*>(0)->m))


#endif /* CCC_MEMORY_H_ */
