#ifndef _JSON_RW_SLEEPY_H_
#define _JSON_RW_SLEEPY_H_

#include "Interface.h"

#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

#include <stack>
#include <vector>
#include <ostream>


namespace persist
{
    class JsonRW : public Writer, public Reader
    {
        typedef rapidjson::Value jsonVal;
        typedef rapidjson::Document jsonDoc;
        typedef rapidjson::StringBuffer jsonBuffer;
        typedef rapidjson::Value::ConstMemberIterator jsonIter;
        typedef rapidjson::Writer< rapidjson::StringBuffer > jsonWriter;

        typedef enum JSON_SCOPE
        {
            SCP_NIL,
            SCP_OBJ,
            SCP_ARR,
        } jsonScope;

        typedef enum JSON_EXPECT
        {
            EXP_KEY,
            EXP_VAL,
        } jsonExpect;

        typedef std::stack< jsonScope > scopeStack;

        struct ReadContext
        {
            int accessingIndex;
            std::string accessingKey;
            jsonVal* operatingValue;
            jsonVal* accessingValue;

            ReadContext()
            {
                accessingIndex = -1;
                operatingValue = NULL;
                accessingValue = NULL;

            }
        };
        typedef std::vector< ReadContext > readContexts;

    protected:
        jsonDoc m_doc;
        jsonBuffer m_buffer;
        jsonWriter m_writer;

        // Write context
        jsonExpect m_nextExpect;
        scopeStack m_scopeStack;

        // Read context
        readContexts m_readContexts;

    public:
        JsonRW();
        ~JsonRW();

        // Writer

        virtual bool newDoc();
        virtual const char* getDoc() const;

        virtual bool startDict();
        virtual bool endDict();

        virtual bool startArray();
        virtual bool endArray();

        virtual bool writeKey(const char* key);

        virtual bool writeVal(const bool& val);

        virtual bool writeVal(const int32_t& val);
        virtual bool writeVal(const uint32_t& val);

        virtual bool writeVal(const int64_t& val);
        virtual bool writeVal(const uint64_t& val);

        virtual bool writeVal(const float& val);
        virtual bool writeVal(const double& val);

        virtual bool writeVal(const char* val);
        virtual bool writeVal(const std::string& val);

        virtual bool writeVal(const void* val);

    protected:
        bool checkPut() const;
        bool checkPutTip(std::ostream& os, const char* function, int line) const;
        bool afterPut();

    public:

        // Reader

        virtual bool parseDoc(std::string doc);

        virtual bool into();
        virtual bool upto();
        virtual bool seek(const char* key);
        virtual bool seek(unsigned int index);

        virtual unsigned int size() const;
        virtual std::vector< std::string > keys() const;

        virtual bool isNil() const;
        virtual bool isDict() const;
        virtual bool isArray() const;
        virtual bool isSimple() const;

        virtual bool isInt32() const = 0;
        virtual bool isUint32() const = 0;

        virtual bool isInt64() const = 0;
        virtual bool isUint64() const = 0;

        virtual bool isFloat() const = 0;
        virtual bool isDouble() const = 0;

        virtual bool isBool() const = 0;
        virtual bool isString() const = 0;

        virtual bool readVal(int32_t& val) const;
        virtual bool readVal(uint32_t& val) const;

        virtual bool readVal(int64_t& val) const;
        virtual bool readVal(uint64_t& val) const;

        virtual bool readVal(float& val) const;
        virtual bool readVal(double& val) const;

        virtual bool readVal(bool& val) const;
        virtual bool readVal(std::string& val) const;

        virtual bool readVal(void* val) const;
    };
}

#endif // _JSON_RW_SLEEPY_H_
