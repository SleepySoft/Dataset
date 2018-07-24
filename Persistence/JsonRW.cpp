#include "JsonRW.h"

#include <iostream>


namespace persist
{
    JsonRW::JsonRW()
        : m_writer(m_buffer)
    {
        m_nextExpect = EXP_VAL;
        m_scopeStack.push(SCP_NIL);

        m_readContexts.push_back(ReadContext());
        m_readContexts.back().operatingValue = &m_doc;
    }

    JsonRW::~JsonRW()
    {
    }

    // Writer

    bool JsonRW::newDoc()
    {
        m_buffer.Clear();
        m_writer.Reset(m_buffer);
        return true;
    }
    const char* JsonRW::getDoc() const
    {
        return m_buffer.GetString();
    }

    bool JsonRW::startDict()
    {
        bool ret = false;
        if (m_nextExpect == EXP_VAL)
        {
            ret = m_writer.StartObject();
        }
        if (ret)
        {
            m_scopeStack.push(SCP_OBJ);
            m_nextExpect = EXP_KEY;
        }
        return ret;
    }
    bool JsonRW::endDict()
    {
        bool ret = false;
        if ((m_scopeStack.top() == SCP_OBJ) && (m_nextExpect == EXP_KEY))
        {
            ret = m_writer.EndObject();
        }
        if (ret)
        {
            if (m_scopeStack.top() == SCP_OBJ)
            {
                m_scopeStack.pop();
            }
            if (m_scopeStack.top() == SCP_OBJ)
            {
                m_nextExpect = EXP_KEY;
            }
            else
            {
                m_nextExpect = EXP_VAL;
            }
        }
        return ret;
    }

    bool JsonRW::startArray()
    {
        bool ret = false;
        if (m_nextExpect == EXP_VAL)
        {
            ret = m_writer.StartArray();
        }
        if (ret)
        {
            m_scopeStack.push(SCP_ARR);
            m_nextExpect = EXP_VAL;
        }
        return ret;
    }
    bool JsonRW::endArray()
    {
        bool ret = false;
        if ((m_scopeStack.top() == SCP_ARR))
        {
            ret = m_writer.EndArray();
        }
        if (ret)
        {
            if (m_scopeStack.top() == SCP_ARR)
            {
                m_scopeStack.pop();
            }
            if (m_scopeStack.top() == SCP_OBJ)
            {
                m_nextExpect = EXP_KEY;
            }
            else
            {
                m_nextExpect = EXP_VAL;
            }
        }
        return ret;
    }

    bool JsonRW::writeKey(const char* key)
    {
        bool ret = false;
        if (m_nextExpect == EXP_KEY)
        {
            ret = m_writer.Key(key);
            m_nextExpect = EXP_VAL;
        }
        return ret;
    }

    bool JsonRW::writeVal(const bool& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.Bool(val) && afterPut());
    }

    bool JsonRW::writeVal(const int32_t& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.Int(val) && afterPut());
    }
    bool JsonRW::writeVal(const uint32_t& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.Uint(val) && afterPut());
    }

    bool JsonRW::writeVal(const int64_t& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.Int64(val) && afterPut());
    }
    bool JsonRW::writeVal(const uint64_t& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.Uint64(val) && afterPut());
    }

    bool JsonRW::writeVal(const float& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.Double(val) && afterPut());
    }
    bool JsonRW::writeVal(const double& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.Double(val) && afterPut());
    }

    bool JsonRW::writeVal(const char* val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.String(val));
    }
    bool JsonRW::writeVal(const std::string& val)
    {
        return (checkPutTip(std::cout, __FUNCTION__, __LINE__) && m_writer.String(val.c_str(), val.size(), true));
    }

    bool JsonRW::writeVal(const void* val)
    {
        (void)(val);
        return false;
    }

    bool JsonRW::checkPut() const
    {
        return (m_nextExpect == EXP_VAL);
    }

    bool JsonRW::checkPutTip(std::ostream& os, const char* function, int line) const
    {
        bool ret = checkPut();
        if (!ret)
        {
            os << "JsonSerializer - expect for key(), but pub() invoked. At: ";
            os << function << " " << line << std::endl;
        }
        return ret;
    }

    bool JsonRW::afterPut()
    {
        if (m_scopeStack.top() = SCP_OBJ)
        {
            m_nextExpect = EXP_KEY;
        }
        else
        {
            m_nextExpect = EXP_VAL;
        }
        return true;
    }

    // Reader

    bool JsonRW::parseDoc(std::string doc)
    {
        return m_doc.Parse(doc.c_str()).HasParseError();
    }

    bool JsonRW::into()
    {
        bool ret = false;
        ReadContext& ctx = m_readContexts.back();

        if (ctx.accessingValue != NULL &&
            (ctx.accessingValue->IsObject() ||
             ctx.accessingValue->IsArray()))
        {
            m_readContexts.push_back(ReadContext());
            ret = true;
        }
        return ret;
    }
    bool JsonRW::upto()
    {
        bool ret = false;
        if (m_readContexts.size() > 1)
        {
            m_readContexts.pop_back();
            ret = true;
        }
        return ret;
    }
    bool JsonRW::seek(const char* key)
    {
        bool ret = false;
        ReadContext& ctx = m_readContexts.back();

        if ((ctx.operatingValue != NULL) &&
            (ctx.operatingValue->IsObject()) &&
            (ctx.operatingValue->HasMember(key)))
        {
            ctx.accessingKey = key;
            ctx.accessingValue = &((*ctx.operatingValue)[key]);
            ret = true;
        }
        return ret;
    }
    bool JsonRW::seek(unsigned int index)
    {
        bool ret = false;
        ReadContext& ctx = m_readContexts.back();

        if ((ctx.operatingValue != NULL) &&
            (ctx.operatingValue->IsArray()) &&
            (ctx.operatingValue->Size() > index))
        {
            ctx.accessingIndex = index;
            ctx.accessingValue = &((*ctx.operatingValue)[index]);
            ret = true;
        }
        return ret;
    }

    unsigned int JsonRW::size() const
    {
        const ReadContext& ctx = m_readContexts.back();
        return ((ctx.operatingValue != NULL) ? ctx.operatingValue->MemberCount() : 0);
    }
    std::vector< std::string > JsonRW::keys() const
    {
        std::vector< std::string > ret;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.operatingValue != NULL) && 
            (ctx.operatingValue->IsObject()))
        {
            rapidjson::Value::ConstMemberIterator iter;
            iter = ctx.operatingValue->MemberBegin();
            for (; iter != ctx.operatingValue->MemberEnd(); ++iter)
            {
                std::string name = (*iter).name.GetString();
                ret.push_back(name);
            }
        }
        return ret;
    }

    bool JsonRW::isNil() const
    {
        const ReadContext& ctx = m_readContexts.back();
        return (ctx.accessingValue == NULL) || ctx.accessingValue->IsNull();
    }
    bool JsonRW::isDict() const
    {
        const ReadContext& ctx = m_readContexts.back();
        return (ctx.accessingValue != NULL) && ctx.accessingValue->IsObject();
    }
    bool JsonRW::isArray() const
    {
        const ReadContext& ctx = m_readContexts.back();
        return (ctx.accessingValue != NULL) && ctx.accessingValue->IsArray();
    }
    bool JsonRW::isSimple() const
    {
        return (!isNil()) && (!isDict()) && (!isArray());
    }

    bool JsonRW::readVal(int32_t& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) && 
            (ctx.accessingValue->IsInt()))
        {
            val = ctx.accessingValue->GetInt();
            ret = true;
        }
        return ret;
    }
    bool JsonRW::readVal(uint32_t& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) &&
            (ctx.accessingValue->IsUint()))
        {
            val = ctx.accessingValue->GetUint();
            ret = true;
        }
        return ret;
    }

    bool JsonRW::readVal(int64_t& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) &&
            (ctx.accessingValue->IsInt()))
        {
            val = ctx.accessingValue->GetInt64();
            ret = true;
        }
        return ret;
    }
    bool JsonRW::readVal(uint64_t& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) &&
            (ctx.accessingValue->IsUint()))
        {
            val = ctx.accessingValue->GetUint64();
            ret = true;
        }
        return ret;
    }

    bool JsonRW::readVal(float& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) &&
            (ctx.accessingValue->IsFloat()))
        {
            val = ctx.accessingValue->GetFloat();
            ret = true;
        }
        return ret;
    }
    bool JsonRW::readVal(double& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) &&
            (ctx.accessingValue->IsDouble()))
        {
            val = ctx.accessingValue->GetDouble();
            ret = true;
        }
        return ret;
    }

    bool JsonRW::readVal(bool& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) &&
            (ctx.accessingValue->IsBool()))
        {
            val = ctx.accessingValue->GetBool();
            ret = true;
        }
        return ret;
    }
    bool JsonRW::readVal(std::string& val) const
    {
        bool ret = false;
        const ReadContext& ctx = m_readContexts.back();

        if ((ctx.accessingValue != NULL) &&
            (ctx.accessingValue->IsString()))
        {
            val = ctx.accessingValue->GetString();
            ret = true;
        }
        return ret;
    }

    bool JsonRW::readVal(void* val) const
    {
        (void)(val);
        return false;
    }
}
