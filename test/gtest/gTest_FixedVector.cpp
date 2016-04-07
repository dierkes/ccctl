/*
 * gTest_FixedVector.cpp
 *
 *  Created on: Mar 18, 2015
 *      Author: dierkes
 */


#include <gtest/gtest.h>

#include <vector>

#include <ccc/fixed_vector.h>
#include <ccc/test/consistent_types.h>

#include "utils.h"
#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

template <>
struct ContainerFactory<ccc::FixedVector<ccc_test::Pod<1, 1>, uint8_t> >
{
    static ccc::FixedVector<ccc_test::Pod<1, 1>, uint8_t> Create()
    {
        return ccc::FixedVector<ccc_test::Pod<1, 1>, uint8_t>(10);
    }
};

TEST(FixedVector, experimental)
{
    typedef ccc::FixedVector<int, uint64_t> FixedContainer;
    FixedContainer c(10);
}

typedef ::testing::Types<
        RefPair<ccc::FixedVector<ccc_test::Pod<1, 1>, uint8_t>, std::vector<ccc_test::Pod<1, 1> > >
> RefPairTypes;

INSTANTIATE_TYPED_TEST_CASE_P(FixedVector, TestOfSequenceContainer, RefPairTypes);
