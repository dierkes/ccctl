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

#ifndef CCC_GTEST_CONTAINER_H_
#define CCC_GTEST_CONTAINER_H_

#include <gtest/gtest.h>

#include "utils.h"

template<typename T>
class TestOfContainer: public ::testing::Test
{

};

template<typename T>
class TestOfPODContainer: public ::testing::Test
{

};

template<typename T>
class TestOfRegularContainer: public ::testing::Test
{

};

template<typename T>
class TestOfStaticContainer: public ::testing::Test
{
public:
    static std::size_t m_Capacity;
};

TYPED_TEST_CASE_P(TestOfPODContainer);

TYPED_TEST_P(TestOfPODContainer, ValueInitialization){
TypeParam c = TypeParam();
EXPECT_TRUE(c.empty());
EXPECT_EQ(0, c.size());
EXPECT_EQ(c.begin(), c.end());
}

REGISTER_TYPED_TEST_CASE_P(TestOfPODContainer, ValueInitialization);

TYPED_TEST_CASE_P(TestOfRegularContainer);

TYPED_TEST_P(TestOfRegularContainer, DefaultInitialization){
TypeParam c;
EXPECT_TRUE(c.empty());
EXPECT_EQ(0, c.size());
EXPECT_EQ(c.begin(), c.end());
}

TYPED_TEST_P(TestOfRegularContainer, ValueInitialization){
TypeParam c = TypeParam();
EXPECT_TRUE(c.empty());
EXPECT_EQ(0, c.size());
EXPECT_EQ(c.begin(), c.end());
}

REGISTER_TYPED_TEST_CASE_P(TestOfRegularContainer, DefaultInitialization, ValueInitialization);

TYPED_TEST_CASE_P(TestOfContainer);

TYPED_TEST_P(TestOfContainer, max_size){
TypeParam c = TypeParam();
EXPECT_NO_THROW(c.max_size());
}

REGISTER_TYPED_TEST_CASE_P(TestOfContainer, max_size);

TYPED_TEST_CASE_P(TestOfStaticContainer);

TYPED_TEST_P(TestOfStaticContainer, max_size){
TypeParam c = TypeParam();
EXPECT_EQ(c.max_size(), this->m_Capacity);
}

REGISTER_TYPED_TEST_CASE_P(TestOfStaticContainer, max_size);

#endif /* CCC_GTEST_CONTAINER_H_ */
