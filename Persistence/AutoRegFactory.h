#ifndef _AUTO_REGISTER_FACTORY_SLEEPY_H_
#define _AUTO_REGISTER_FACTORY_SLEEPY_H_

#include <map>
#include <string>


template< class OBJ >
class AutoRegFactory
{
public:
    typedef OBJ* (*Creator)();
protected:
    typedef std::map< std::string, Creator > CreatorDict;
    static CreatorDict m_creatorDict;
public:
    AutoRegFactory() = delete;

    static bool registerObjectCreator(std::string name, Creator creator)
    {
        printf("Register for %s\n", name.c_str());
        m_creatorDict[name] = creator;
        return true;
    }
    static OBJ* createObject(std::string name)
    {
        Creator creator = NULL;

        for (auto pos = m_creatorDict.begin(); pos != m_creatorDict.end(); ++pos)
        {
            std::string name((*pos).first);
            printf("%s\n", name.c_str());
        }

        auto iter = m_creatorDict.find(name);
        if (iter != m_creatorDict.end())
        {
            Creator creator = (*iter).second;
            return creator();
        }
        return (creator == NULL) ? NULL : creator();
    }
};

template< class OBJ >
typename AutoRegFactory< OBJ >::CreatorDict AutoRegFactory< OBJ >::m_creatorDict = typename AutoRegFactory< OBJ >::CreatorDict();


template< class OBJ , class CLASS  >
class AutoRegKit 
{
protected:
    static bool m_registed;
public:
    AutoRegKit()
    {
        printf("AutoRegKit => %s\n", typeid(CLASS).name());
    }
    virtual ~AutoRegKit()
    {

    }
};

template< class OBJ, class CLASS >
bool AutoRegKit< OBJ, CLASS >::m_registed = AutoRegFactory< OBJ >::registerObjectCreator(typeid(CLASS).name(), [] { return new CLASS(); });

#endif // _AUTO_REGISTER_FACTORY_SLEEPY_H_

