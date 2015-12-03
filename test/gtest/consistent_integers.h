/**
 *
 * @file
 *
 * @author Frank Dierkes
 *
 * $LastChangedBy$
 * $Date$
 * $Revision$
 *
 * @remarks
 *
 */


#ifndef TEST_CONSISTENT_INTEGERS_H_
#define TEST_CONSISTENT_INTEGERS_H_

#if defined _MSC_VER && _MSC_VER < 1600
   typedef __int8 int8_t;
   typedef __int16 int16_t;
   typedef __int32 int32_t;
   typedef __int64 int64_t;
   typedef unsigned __int8 uint8_t;
   typedef unsigned __int16 uint16_t;
   typedef unsigned __int32 uint32_t;
   typedef unsigned __int64 uint64_t;
#else
   #include <stdint.h>
#endif

#endif /* TEST_CONSISTENT_INTEGERS_H_ */
