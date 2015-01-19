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

//namespace ccc
//{

template<typename T>
class BaseContainerTest: public ::testing::Test
{

};

template<typename T>
class PODContainerTest: public ::testing::Test
{

};

template<typename T>
class RegularContainerTest: public ::testing::Test
{

};

template<typename T>
class StaticContainerTest: public ::testing::Test
{
public:
    static std::size_t m_Capacity;
//protected:
//    virtual void SetUp()
//    {
//        m_Capacity = 10;
//    }
//
//    std::size_t m_Capacity;
};

//}

TYPED_TEST_CASE_P(PODContainerTest);

TYPED_TEST_P(PODContainerTest, ValueInitialization){
TypeParam c = TypeParam();
EXPECT_TRUE(c.empty());
EXPECT_EQ(0, c.size());
EXPECT_EQ(c.begin(), c.end());
}

REGISTER_TYPED_TEST_CASE_P(PODContainerTest, ValueInitialization);

TYPED_TEST_CASE_P(RegularContainerTest);

TYPED_TEST_P(RegularContainerTest, DefaultInitialization){
TypeParam c;
EXPECT_TRUE(c.empty());
EXPECT_EQ(0, c.size());
EXPECT_EQ(c.begin(), c.end());
}

TYPED_TEST_P(RegularContainerTest, ValueInitialization){
TypeParam c = TypeParam();
EXPECT_TRUE(c.empty());
EXPECT_EQ(0, c.size());
EXPECT_EQ(c.begin(), c.end());
}

REGISTER_TYPED_TEST_CASE_P(RegularContainerTest, DefaultInitialization, ValueInitialization);

TYPED_TEST_CASE_P(BaseContainerTest);

TYPED_TEST_P(BaseContainerTest, max_size){
TypeParam c = TypeParam();
EXPECT_NO_THROW(c.max_size());
}

REGISTER_TYPED_TEST_CASE_P(BaseContainerTest, max_size);

TYPED_TEST_CASE_P(StaticContainerTest);

TYPED_TEST_P(StaticContainerTest, max_size){
TypeParam c = TypeParam();
EXPECT_EQ(c.max_size(), this->m_Capacity);
}

REGISTER_TYPED_TEST_CASE_P(StaticContainerTest, max_size);

namespace ccc
{

struct tPOD
{
    int x;
    double y;

    bool operator==(const tPOD& rhs) const
    {
        return (this->x == rhs.x) && (this->y == rhs.y);
    }

    bool operator!=(const tPOD& rhs) const
    {
        return !(*this == rhs);
    }
};

class cNoPOD
{
public:
    cNoPOD()
            : m_x(1), m_y(2.0)
    {
    }

    bool operator==(const cNoPOD& rhs) const
    {
        return (this->m_x == rhs.m_x) && (this->m_y == rhs.m_y);
    }

    bool operator!=(const cNoPOD& rhs) const
    {
        return !(*this == rhs);
    }
private:
    int m_x;
    double m_y;
};

class cThrowWhenConstructed
{
public:
    cThrowWhenConstructed()
    {
        throw std::exception();
    }
};

template<typename Container>
void Test_Types()
{
    typedef typename Container::value_type Container_value_type;
    typedef typename Container::iterator Container_iterator;
}

template<typename Container>
void Test_DefaultInitialization()
{
    Container c;
    EXPECT_TRUE(c.empty());
    EXPECT_EQ(0, c.size());
    EXPECT_EQ(c.begin(), c.end());
}

template<typename Container>
void Test_ValueInitialization()
{
    Container c = Container();
    EXPECT_TRUE(c.empty());
    EXPECT_EQ(0, c.size());
    EXPECT_EQ(c.begin(), c.end());
}

template<typename Container>
void Test_max_size(const std::size_t ExpectedMaxSize)
{
    Container c = Container();
    EXPECT_EQ(ExpectedMaxSize, c.max_size());
}

}

#endif /* CCC_GTEST_CONTAINER_H_ */
