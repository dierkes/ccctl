/*
 * gTest_Objects.h
 *
 *  Created on: Jan 19, 2015
 *      Author: dierkes
 */

#ifndef CCC_GTEST_OBJECTS_H_
#define CCC_GTEST_OBJECTS_H_

#include <cstdlib>
#include <exception>
#include <sstream>
#include <set>
#include <iostream>

#include <ccc/test/consistent_types.h>

template <typename T>
T random_object()
{
    return T();
}

template <typename T>
struct ContainerFactory
{
    static T Create()
    {
        return T();
    }
};


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

    friend inline std::ostream& operator<<(std::ostream& os, tPOD o)
    {
        os << "(" << o.x << "," << o.y << ")";
        return os;
    }
};


class cNoPOD
{
public:
    cNoPOD()
            : m_x(1), m_y(2.0)
    {
    }

    cNoPOD(int x, double y)
            : m_x(x), m_y(y)
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

    friend inline std::ostream& operator<<(std::ostream& os, cNoPOD o)
    {
        os << "(" << o.m_x << "," << o.m_y << ")";
        return os;
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

template <typename ContainerType>
std::string PrintContent(const ContainerType& c)
{
    std::stringstream ss;
    ss << "[";
    for (typename ContainerType::const_iterator it = c.begin(); it != c.end(); ++it)
    {
        ss << *it << ",";
    }
    ss << "]";
    return ss.str();
}

template <>
int random_object<int>();

template <>
tPOD random_object<tPOD>();

template <>
cNoPOD random_object<cNoPOD>();

template<bool Verbose = false>
class cUniqueID
{
public:
    static uint64_t NextID;
    static std::set<uint64_t> CurrentIDs;

    uint64_t m_ID;

    cUniqueID()
    {
        m_ID = NextID++;
        CurrentIDs.insert(m_ID);
        if (Verbose)
        {
            std::cout << "Default constructor of " << m_ID << std::endl;
        }
    }

    cUniqueID(const cUniqueID& Other)
    {
        m_ID = NextID++;
        CurrentIDs.insert(m_ID);
        if (Verbose)
        {
            std::cout << "Copy constructor of " << m_ID << " from " << Other.m_ID << std::endl;
        }
    }

    cUniqueID& operator=(const cUniqueID& Other)
    {
        // do NOT assign m_ID of other to m_ID of this!
        m_ID = m_ID + 0; // make cppcheck not complaining
        if (Verbose)
        {
            std::cout << "Assignment of " << Other.m_ID << " to " << m_ID << std::endl;
        }
        return *this;
    }

    ~cUniqueID()
    {
        CurrentIDs.erase(m_ID);
        if (Verbose)
        {
            std::cout << "Destructor of " << m_ID  << std::endl;
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const cUniqueID& obj) // output
    {
        out <<  obj.m_ID;
        return out;
    }

};

#endif /* CCC_GTEST_OBJECTS_H_ */
