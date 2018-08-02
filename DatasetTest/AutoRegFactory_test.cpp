#include <gtest/gtest.h>
#include "AutoRegFactory.h"


class ObjectBaseTest
{
public:
    ObjectBaseTest() { };
    virtual ~ObjectBaseTest() { };
};

class Object1 : public ObjectBaseTest, AutoRegKit< ObjectBaseTest, Object1 >
{
public:
    Object1() { };
    virtual ~Object1() { };
};

class Object2 : public ObjectBaseTest, AutoRegKit< ObjectBaseTest, Object2 >
{
public:
    Object2() { };
    virtual ~Object2() { };
};

class Object1_1 : public Object1, AutoRegKit< ObjectBaseTest, Object1_1 >
{
public:
    Object1_1() { };
    virtual ~Object1_1() { };
};

TEST(AutoRegFactory_Test, CreateObject)
{
    ObjectBaseTest* object1 = AutoRegFactory< ObjectBaseTest >::createObject("Object1");
    ObjectBaseTest* object2 = AutoRegFactory< ObjectBaseTest >::createObject("Object2");
    ObjectBaseTest* object1_1 = AutoRegFactory< ObjectBaseTest >::createObject("Object1_1");

    ASSERT_TRUE(object1 != NULL);
    ASSERT_TRUE(object2 != NULL);
    ASSERT_TRUE(object1_1 != NULL);
}
