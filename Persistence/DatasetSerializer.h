#ifndef _DATASET_SERIALIZER_SLEEPY_H_
#define _DATASET_SERIALIZER_SLEEPY_H_

#include <string>
#include "data_wrapper.h"


class Dataset;

namespace persist
{
    class Writer;
    class Reader;

    std::string serialize(Dataset* dataset, Writer* writer);
    bool deserialize(Dataset* dataset, Reader* reader);
}

#endif // _DATASET_SERIALIZER_SLEEPY_H_
