#ifndef _DATASET_SERIALIZER_SLEEPY_H_
#define _DATASET_SERIALIZER_SLEEPY_H_

#include <string>
#include "data_wrapper.h"


class Dataset;

namespace persist
{
    class Writer;
    class Reader;
}

class DatasetSerializer
{
public:
    DatasetSerializer();
    virtual ~DatasetSerializer();

    bool serialize(Dataset* dataset, persist::Writer* writer);
    bool deserialize(Dataset* dataset, persist::Reader* reader);

    virtual bool onWritingUnknownType(persist::Writer* writer, const dw::any& any) = 0;
    virtual bool onReadingUnknownType(persist::Reader* reader) = 0;

    static bool anyRead(persist::Reader* reader, dw::any& any);
    static bool anyWrite(persist::Writer* writer, const dw::any& any);
};

#endif // _DATASET_SERIALIZER_SLEEPY_H_
