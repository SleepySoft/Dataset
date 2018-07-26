#ifndef _INTERFACE_PERSISTENCE_SLEEPY_H_
#define _INTERFACE_PERSISTENCE_SLEEPY_H_


#include <string>
#include <vector>


//////////////////////////////////////////////////////////////////////////////////////
// 
// The requirement of Writer and Reader:
//      1.Supports Dict and Array, the key of Dict MUST be a string.
//      2.Dict and Array can be nested, All nested item should only be value.
//      3.The result of writer nanmed 'Doc', 'Doc' can be value.
// 
//////////////////////////////////////////////////////////////////////////////////////

namespace persist
{
    class Writer
    {
    public:
        Writer() { };
        virtual ~Writer() { };

        virtual bool newDoc() = 0;
        virtual const char* getDoc() const = 0;

        virtual bool startDict() = 0;
        virtual bool endDict() = 0;

        virtual bool startArray() = 0;
        virtual bool endArray() = 0;

        virtual bool writeKey(const char* key) = 0;

        virtual bool writeVal(const bool& val) = 0;

        virtual bool writeVal(const int32_t& val) = 0;
        virtual bool writeVal(const uint32_t& val) = 0;

        virtual bool writeVal(const int64_t& val) = 0;
        virtual bool writeVal(const uint64_t& val) = 0;

        virtual bool writeVal(const float& val) = 0;
        virtual bool writeVal(const double& val) = 0;

        virtual bool writeVal(const char* val) = 0;
        virtual bool writeVal(const std::string& val) = 0;

        virtual bool writeVal(const void* val) = 0;
    };
    class Reader
    {
    public:
        Reader() { };
        virtual ~Reader() { };

        virtual bool parseDoc(std::string doc) = 0;

        virtual bool into() = 0;
        virtual bool upto() = 0;
        virtual bool seek(const char* key) = 0;
        virtual bool seek(unsigned int index) = 0;
        virtual bool rewind() = 0;

        virtual unsigned int size() const = 0;
        virtual std::vector< std::string > keys() const = 0;

        virtual bool isNil() const = 0;
        virtual bool isDict() const = 0;
        virtual bool isArray() const = 0;
        virtual bool isSimple() const = 0;

        virtual bool isInt32() const = 0;
        virtual bool isUint32() const = 0;

        virtual bool isInt64() const = 0;
        virtual bool isUint64() const = 0;

        virtual bool isFloat() const = 0;
        virtual bool isDouble() const = 0;

        virtual bool isBool() const = 0;
        virtual bool isString() const = 0;

        virtual bool readVal(int32_t& val) const = 0;
        virtual bool readVal(uint32_t& val) const = 0;

        virtual bool readVal(int64_t& val) const = 0;
        virtual bool readVal(uint64_t& val) const = 0;

        virtual bool readVal(float& val) const = 0;
        virtual bool readVal(double& val) const = 0;

        virtual bool readVal(bool& val) const = 0;
        virtual bool readVal(std::string& val) const = 0;

        virtual bool readVal(void* val) const = 0;
    };
};

#endif // _INTERFACE_PERSISTENCE_SLEEPY_H_
