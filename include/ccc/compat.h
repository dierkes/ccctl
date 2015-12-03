/**
 *
 * @file The file contains macro definitions necessary for compatibility across different compilers and platforms.
 *
 * @author Frank Dierkes
 *
 * @copyright MIT license (A copy of the license is distributed with the software.)
 *
 */

#ifndef CCC_COMPAT_H_
#define CCC_COMPAT_H_

/*
 * Default definitions:
 */

#define CCC_NOEXCEPT
#define CCC_CONSTEXPR
#define CCC_DEFAULT {}
#define CCC_IS_TRIVIALLY_COPYABLE_AVAILABLE false
#define CCC_IS_POD_AVAILABLE false
#define CCC_ALIGNED_AVAILABLE false
#define CCC_ALIGNAS_AVAILABLE false
#define CCC_ALIGNOF_AVAILABLE false

/*
 * Compiler-specific definitions:
 *
 * (Basically check for C++11 support.)
 */

#if defined(__GNUG__) // equivalent to (__GNUC__ && __cplusplus)

#define CCC_ALIGNED(T, Alignment) T __attribute__((aligned((Alignment))))
#undef CCC_ALIGNED_AVAILABLE
#define CCC_ALIGNED_AVAILABLE true
#undef CCC_ALIGNOF_AVAILABLE
#define CCC_ALIGNOF_AVAILABLE true

#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 7) && (__cplusplus >= 201103L)

#undef CCC_NOEXCEPT
#define CCC_NOEXCEPT noexcept
#undef CCC_CONSTEXPR
#define CCC_CONSTEXPR constexpr
#undef CCC_DEFAULT
#define CCC_DEFAULT = default;
#define CCC_ALIGNOF(type) alignof(type)

#else

#define CCC_ALIGNOF(type) __alignof((type))

#endif

#elif defined(_MSC_VER)

#define CCC_ALIGNED(T, Alignment) __declspec(align((Alignment))) T
#undef CCC_ALIGNED_AVAILABLE
#define CCC_ALIGNED_AVAILABLE true
#undef CCC_ALIGNOF_AVAILABLE
#define CCC_ALIGNOF_AVAILABLE true

/*
 * Version numbers of Microsoft compilers:
 *
 * VS 2005 == VC8 == _MSC_VER 1400
 * VS 2008 == VC9 == _MSC_VER 1500
 * VS 2010 == VC10 == _MSC_VER 1600
 * VS 2012 == VC11 == _MSC_VER 1700
 * VS 2013 == VC12 == _MSC_VER 1800
 * VS 2015 == VC14 == _MSC_VER 1900
 */

#if (_MSC_VER >= 1900)

#undef CCC_NOEXCEPT
#define CCC_NOEXCEPT noexcept
#undef CCC_CONSTEXPR
#define CCC_CONSTEXPR constexpr
#undef CCC_DEFAULT
#define CCC_DEFAULT = default;
#undef CCC_ALIGNAS_AVAILABLE
#define CCC_ALIGNAS_AVAILABLE true
#define CCC_ALIGNOF(expression) alignof(expression)

#else

#define CCC_ALIGNOF(type) __alignof(type)

#endif // (_MSC_VER >= 1800)

#endif

#endif /* CCC_COMPAT_H_ */
