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

#include <ccc/consistent_vector.h>
#include <gtest/gtest.h>

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#include <type_traits>
#include <vector>

typedef ccc::ConsistentVector<char, std::size_t, 10> VectorOfChars;
typedef ccc::ConsistentVector<tPOD, std::size_t, 10> VectorOfPODs;
typedef ccc::ConsistentVector<cNoPOD, std::size_t, 10> VectorOfNonPODs;

#if (__cplusplus >= 201103L)
TEST(ConsistentVector, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_standard_layout<VectorOfChars>::value);
    EXPECT_TRUE(std::is_standard_layout<VectorOfChars>::value);
}
#endif

template<> std::size_t TestOfStaticContainer<VectorOfChars>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<VectorOfPODs>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<VectorOfNonPODs>::m_Capacity = 10;

typedef ::testing::Types<VectorOfChars, VectorOfPODs, VectorOfNonPODs> VectorImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfContainer, VectorImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfRegularContainer, VectorImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfStaticContainer, VectorImplementations);

typedef reftest<ccc::ConsistentVector<char, std::size_t, 10>, std::vector<char> > RefVectorOfChars;
typedef reftest<ccc::ConsistentVector<tPOD, std::size_t, 10>, std::vector<tPOD> > RefVectorOfPODs;
typedef reftest<ccc::ConsistentVector<cNoPOD, std::size_t, 10>, std::vector<cNoPOD> > RefVectorOfNonPODs;

typedef ::testing::Types<RefVectorOfChars, RefVectorOfPODs, RefVectorOfNonPODs> RefStackImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfSequenceContainer, RefStackImplementations);
