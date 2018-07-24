#include "DatasetSerializer.h"

#include "JsonRW.h"
#include "dataset.h"


namespace persist
{
    std::string serialize(Dataset* dataset, Writer* writer)
    {
        Dataset::KEYLIST keys(dataset->keys());
        return std::string();
    }

    bool deserialize(Dataset* dataset, Reader* reader)
    {
        return true;
    }
}
