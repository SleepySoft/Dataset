#ifndef _AUTO_REGISTER_FACTORY_SLEEPY_H_
#define _AUTO_REGISTER_FACTORY_SLEEPY_H_

#include <map>
#include <string>
#include <memory>


template< class ROOT  >
class AutoRegFactory
{
public:
    typedef std::shared_ptr< ROOT >(*Creator)();
    typedef std::map< std::string, Creator > CreatorDict;
public:
    AutoRegFactory() = delete;

    static CreatorDict& creatorDict()
    {
        static CreatorDict s_creatorDict = CreatorDict();
        return s_creatorDict;
    }
    static bool registerObjectCreator(std::string name, Creator creator)
    {
        printf("Register for %s\n", name.c_str());
        creatorDict()[name] = creator;
        return true;
    }
    static std::shared_ptr< ROOT > createObject(std::string name)
    {
        Creator creator = NULL;

        for (auto pos = creatorDict().begin(); pos != creatorDict().end(); ++pos)
        {
            std::string name((*pos).first);
            printf("%s\n", name.c_str());
        }

        auto iter = creatorDict().find(name);
        if (iter != creatorDict().end())
        {
            Creator creator = (*iter).second;
            return creator();
        }
        return (creator == NULL) ? NULL : creator();
    }
};



























#include <memory>
#include <map>
#include <functional>
#include <iostream>

struct message_base;

template< class BASE  >
class MsgFactory {
public:
    typedef std::shared_ptr< BASE >(*Creator)();
    typedef std::map< std::string, Creator > CreatorDict;
public:
    MsgFactory() = delete;
    //using CreateMethod = std::function<std::unique_ptr<BASE>()>;

    static bool Register(size_t id, Creator creator)
    {
        auto& map = GetMap();
        auto it = map.find(id);
        if (it == map.end())
        {
            map[id] = creator;
            std::cout << id << " registered\n";
            return true;
        }
        return false;
    }

    static bool Register2(std::string name, Creator creator)
    {
        auto& map = GetMap2();
        auto it = map.find(name);
        if (it == map.end())
        {
            map[name] = creator;
            std::cout << name << " registered\n";
            return true;
        }
        return false;
    }

    static std::shared_ptr<BASE> Create(size_t id)
    {
        auto& map = GetMap();
        auto it = map.find(id);
        if (it != map.end())
            return it->second();

        return nullptr;
    }

    static std::shared_ptr<BASE> Create2(std::string name)
    {
        auto& map = GetMap2();
        auto it = map.find(name);
        if (it != map.end())
            return it->second();

        return nullptr;
    }

    static std::map<size_t, Creator>& GetMap() {
        static std::map<size_t, Creator> map;
        return map;
    }
    static std::map<std::string, Creator>& GetMap2() {
        static std::map<std::string, Creator> map;
        return map;
    }
};

template < class BASE, class OBJ>
class AutoMsgFactory
{
protected:
    static bool registered_;
    static const char* __type_name() { return typeid(OBJ).name(); };
};

template < class BASE, class OBJ>
bool AutoMsgFactory< BASE, OBJ >::registered_ = MsgFactory< BASE >::Register2(typeid(OBJ).name(), []() -> std::shared_ptr< BASE > { return std::make_shared< OBJ >(); });





template < class BASE, class OBJ>
class AutoRegKit
{
protected:
    static bool registered_;
    static const char* __type_name() { return typeid(OBJ).name(); };
};

template < class BASE, class OBJ>
bool AutoRegKit< BASE, OBJ >::registered_ = MsgFactory< BASE >::Register2(typeid(OBJ).name(), []() -> std::shared_ptr< BASE > { return std::make_shared< OBJ >(); });


//
//template< class BASE, class OBJ  >
//class AutoRegKit
//{
//protected:
//    static bool m_registed;
//};
//
//
//template< class BASE, class OBJ  >
//bool AutoRegKit< BASE, OBJ >::m_registed = MsgFactory< BASE >::Register2(typeid(OBJ).name(), []() -> std::shared_ptr< BASE > { return std::make_shared< OBJ >(); });





#endif // _AUTO_REGISTER_FACTORY_SLEEPY_H_

