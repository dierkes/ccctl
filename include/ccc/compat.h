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
#define CCC_ALIGNED_TYPE(T) // this will cause a compile error, if the macro is not specified for a compiler

/*
 * Compiler-specific definitions:
 *
 * (Basically check for C++11 support.)
 */

#if defined(__GNUG__) // equivalent to (__GNUC__ && __cplusplus)

#define CCC_ALIGNED_TYPE(T, Alignment) T __attribute__(aligned((Alignment)))

#if (__GNUC__ >= 4) && (__GNUC_MINOR__ >= 7) && (__cplusplus >= 201103L)

#undef CCC_NOEXCEPT
#define CCC_NOEXCEPT noexcept
#undef CCC_CONSTEXPR
#define CCC_CONSTEXPR constexpr
#undef CCC_DEFAULT
#define CCC_DEFAULT = default;

#endif

#elif defined(_MSC_VER)

#define CCC_ALIGNED_TYPE(T, Alignment) __declspec(align((Alignment))) T

/*
 * Version numbers of Microsoft compilers:
 *
 * VS 2005 == VC8 == _MSC_VER 1400
 * VS 2008 == VC9 == _MSC_VER 1500
 * VS 2010 == VC10 == _MSC_VER 1600
 * VS 2012 == VC11 == _MSC_VER 1700
 * VS 2013 == VC12 == _MSC_VER 1800
 */

#if (_MSC_VER < 1700)
#undef CCC_IS_TRIVIALLY_COPYABLE_AVAILABLE
#define CCC_IS_TRIVIALLY_COPYABLE_AVAILABLE 0
#undef CCC_IS_POD_AVAILABLE
#define CCC_IS_POD_AVAILABLE 0
#endif

#if (_MSC_VER >= 1800)

#undef CCC_NOEXCEPT
#define CCC_NOEXCEPT noexcept
#undef CCC_CONSTEXPR
#define CCC_CONSTEXPR constexpr
#undef CCC_DEFAULT
#define CCC_DEFAULT = default;

#endif // (_MSC_VER >= 1800)

#endif

#endif /* CCC_COMPAT_H_ */
