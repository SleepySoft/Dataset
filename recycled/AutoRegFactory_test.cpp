#include <gtest/gtest.h>
#include "AutoRegFactory.h"


class ObjectBaseTest
{
public:
    ObjectBaseTest() { };
    virtual ~ObjectBaseTest() { };
};

class Object1 : public ObjectBaseTest, AutoMsgFactory< ObjectBaseTest, Object1 >
{
public:
    Object1() { };
    virtual ~Object1() { };

    void foo() { registered_; }
};


class Object2 : public ObjectBaseTest, AutoRegKit< ObjectBaseTest, Object2 >
{
public:
    Object2() { };
    virtual ~Object2() { };

    void foo() { registered_; }
};



class Object1_1 : public Object1, AutoRegKit< ObjectBaseTest, Object1_1 >
{
public:
    Object1_1() { };
    virtual ~Object1_1() { };
};



//test code
struct message_base {
    virtual ~message_base() {}
};

struct message1 : public message_base, AutoMsgFactory< message_base, message1 >
{
    static size_t Id() {
        assert(!registered_);
        return 1;
    }
    //static const char* __type_name() { return registered_ ? "" : typeid(message1).name(); };
};

struct message2 : public message_base, AutoMsgFactory< message_base, message2 >
{
    static size_t Id() {
        assert(!registered_);
        return 2;
    }
    //static const char* __type_name() { return registered_ ? "" : typeid(message2).name(); };
};

TEST(AutoRegFactory_Test, CreateObject)
{

    auto msg1 = MsgFactory< message_base >::Create2("struct message1");
        if (msg1) {
        std::cout << typeid(msg1).name() << std::endl;
    }


    auto msg2 = MsgFactory< message_base >::Create2("struct message2");
    if (msg2) {
        std::cout << typeid(msg2).name() << std::endl;
    }

    std::shared_ptr< ObjectBaseTest > object1 = AutoRegFactory< ObjectBaseTest >::createObject("class Object1");
    std::shared_ptr< ObjectBaseTest > object2 = AutoRegFactory< ObjectBaseTest >::createObject("class Object2");
    std::shared_ptr< ObjectBaseTest > object1_1 = AutoRegFactory< ObjectBaseTest >::createObject("class Object1_1");

    ASSERT_TRUE(object1 != NULL);
    ASSERT_TRUE(object2 != NULL);
    ASSERT_TRUE(object1_1 != NULL);
}
