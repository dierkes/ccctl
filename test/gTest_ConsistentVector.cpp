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

#include "gTest_Container.h"
#include "gTest_SequenceContainer.h"

#if (__cplusplus >= 201103L)
#include <type_traits>
#endif

#include <vector>

typedef ccc::ConsistentVector<int, std::size_t, 10> ContainerOfInts;
typedef ccc::ConsistentVector<tPOD, std::size_t, 10> ContainerOfPODs;
typedef ccc::ConsistentVector<cNoPOD, std::size_t, 10> ContainerOfNonPODs;

#if (__cplusplus >= 201103L)
TEST(ConsistentVector, TypeTraits_Cpp11)
{
    EXPECT_TRUE(std::is_standard_layout<ContainerOfInts>::value);
    EXPECT_TRUE(std::is_standard_layout<ContainerOfPODs>::value);
}
#endif

template<> std::size_t TestOfStaticContainer<ContainerOfInts>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfPODs>::m_Capacity = 10;
template<> std::size_t TestOfStaticContainer<ContainerOfNonPODs>::m_Capacity = 10;

typedef ::testing::Types<ContainerOfInts, ContainerOfPODs, ContainerOfNonPODs> ContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfRegularContainer, ContainerImplementations);
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfStaticContainer, ContainerImplementations);

typedef reftest<ccc::ConsistentVector<int, std::size_t, 10>, std::vector<int> > RefContainerOfInts;
typedef reftest<ccc::ConsistentVector<tPOD, std::size_t, 10>, std::vector<tPOD> > RefContainerOfPODs;
typedef reftest<ccc::ConsistentVector<cNoPOD, std::size_t, 10>, std::vector<cNoPOD> > RefContainerOfNonPODs;

typedef ::testing::Types<RefContainerOfInts, RefContainerOfPODs, RefContainerOfNonPODs> RefContainerImplementations;
INSTANTIATE_TYPED_TEST_CASE_P(ConsistentVector, TestOfSequenceContainer, RefContainerImplementations);


//TEST(ConsistentVector, DestroyOnErase)
//{
//    cUniqueID::NextID = 0;
//    cUniqueID::CurrentIDs = std::set<std::size_t>();
//    typedef ccc::ConsistentVector<cUniqueID, std::size_t, 10> Container;
////    typedef std::vector<cUniqueID> Container;
//    EXPECT_TRUE(cUniqueID::CurrentIDs.empty());
//    {
////        Container c = Container();
//        Container c;
//        EXPECT_EQ(10, cUniqueID::CurrentIDs.size()) << PrintContent(cUniqueID::CurrentIDs) << std::endl << PrintContent(c);
//        {
//            cUniqueID obj = cUniqueID();
//            c.push_back(obj);
//        }
//        EXPECT_EQ(10, cUniqueID::CurrentIDs.size()) << PrintContent(cUniqueID::CurrentIDs) << std::endl << PrintContent(c);
//        c.push_back(cUniqueID());
//        EXPECT_EQ(10, cUniqueID::CurrentIDs.size()) << PrintContent(cUniqueID::CurrentIDs) << std::endl << PrintContent(c);
//    }
//    EXPECT_TRUE(cUniqueID::CurrentIDs.empty()) << PrintContent(cUniqueID::CurrentIDs);
//}


TEST(ConsistentVector, DestroyOnErase)
{
    typedef cUniqueID<true> U;
    U::NextID = 0;
    U::CurrentIDs = std::set<std::size_t>();
    typedef ccc::ConsistentVector<U, std::size_t, 10, 8, false> Container;
//    typedef std::vector<cUniqueID> Container;
    EXPECT_TRUE(U::CurrentIDs.empty());
    {
        Container c = Container();
        EXPECT_EQ(10, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
        c.push_back(U());
        c.push_back(U());
        c.push_back(U());
        c.erase(c.begin(), ccc::next(c.begin()));
        EXPECT_EQ(10, U::CurrentIDs.size()) << PrintContent(U::CurrentIDs);
        c.clear(); // This is necessary in case of uninititalized storage
    }
    EXPECT_TRUE(U::CurrentIDs.empty()) << PrintContent(U::CurrentIDs);
}
