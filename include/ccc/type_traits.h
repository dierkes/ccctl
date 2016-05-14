/**
 *
 * @file Contains functions similar to those defined in the standard's type_traits header.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_TYPE_TRAITS_H_
#define CCC_TYPE_TRAITS_H_

#include <ccc/compat.h>

namespace ccc
{

template<class T, T Value>
struct integral_constant
{
    static CCC_CONSTEXPR const T value = Value;
    typedef T value_type;
    typedef integral_constant<T, Value> type;CCC_CONSTEXPR
    operator value_type() const
    {
        return value;
    }
};

#if CCC_IS_TRIVIALLY_COPYABLE_AVAILABLE

template <class T>
struct is_trivially_copyable_guaranteed : public ccc::integral_constant<bool, std::is_trivially_copyable<T>::value>
{};

#elif CCC_IS_POD_AVAILABLE

template <class T>
struct is_trivially_copyable_guaranteed : public ccc::integral_constant<bool, std::is_pod<T>::value>
{};

#else

template<class T>
struct is_trivially_copyable_guaranteed: public ccc::integral_constant<bool, false>
{
};

#endif

template<class T> struct remove_const
{
    typedef T type;
};
template<class T> struct remove_const<const T>
{
    typedef T type;
};

} // namespace ccc

#endif /* CCC_TYPE_TRAITS_H_ */
