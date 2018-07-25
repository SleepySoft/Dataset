#include "DatasetSerializer.h"

#include "JsonRW.h"
#include "dataset.h"
#include "ambiguous.h"


DatasetSerializer::DatasetSerializer()
{

}

DatasetSerializer::~DatasetSerializer()
{

}

bool DatasetSerializer::serialize(Dataset* dataset, persist::Writer* writer)
{
    bool ret = false;

    if (writer->startDict())
    {
        dw::any any;
        Dataset::KEYLIST keys(dataset->keys());

        auto iter = keys.begin();
        for (; iter != keys.end(); ++iter)
        {
            if (dataset->exists((*iter)) &&
                dataset->get((*iter), any) && 
                writer->writeKey((*iter).c_str()))
            {
                if ((!anyWrite(writer, any)) && 
                    (!onWritingUnknownType(writer, any)))
                {
                    printf("Cannot write value: unknown type for %s\n", (*iter).c_str());
                    writer->writeVal("UnknowType");
                }
            }
        }
        writer->endDict();
    }

    return ret;
}

bool DatasetSerializer::deserialize(Dataset* dataset, persist::Reader* reader)
{
    if (reader->isDict() && 
        reader->into())
    {
        dw::any any;
        std::vector< std::string > keys(reader->keys());

        auto iter = keys.begin();
        for (; iter != keys.end(); ++iter)
        {
            if ((reader->seek((*iter).c_str())) && 
                (!reader->isNil()))
            {
                if ((!anyRead(reader, any)) && 
                    (!onReadingUnknownType(reader)))
                {
                    printf("Cannot read value: unknown type for %s\n", (*iter).c_str());
                    continue;
                }
                if (dataset->exists(*iter))
                {
                    // If data exists, type specified.
                    dw::any existed = dataset->get(*iter);
                }
                else
                {
                    // If not, use original type.
                    dataset->set((*iter), any);
                }
            }
        }

        reader->upto();
    }

    return true;
}

bool DatasetSerializer::onWritingUnknownType(persist::Writer* writer, const dw::any& any)
{
    return false;
}


// static

bool DatasetSerializer::anyRead(persist::Reader* reader, dw::any& any)
{
    bool ret = false;

    if (reader->isInt32())
    {
        int32_t val = 0;
        ret = reader->readVal(val);
        any = dw::any(val);
    }
    else if (reader->isUint32())
    {
        uint32_t val = 0;
        ret = reader->readVal(val);
        any = dw::any(val);
    }
    else if (reader->isInt64())
    {
        int64_t val = 0;
        ret = reader->readVal(val);
        any = dw::any(val);
    }
    else if (reader->isUint64())
    {
        uint64_t val = 0;
        ret = reader->readVal(val);
        any = dw::any(val);
    }

    else if (reader->isFloat())
    {
        float val = 0.0f;
        ret = reader->readVal(val);
        any = dw::any(val);
    }
    else if (reader->isDouble())
    {
        double val = 0.0;
        ret = reader->readVal(val);
        any = dw::any(val);
    }

    else if (reader->isBool())
    {
        bool val = false;
        ret = reader->readVal(val);
        any = dw::any(val);
    }
    else if (reader->isString())
    {
        std::string val;
        ret = reader->readVal(val);
        any = dw::any(val);
    }

    return ret;
}
bool DatasetSerializer::anyWrite(persist::Writer* writer, const dw::any& any)
{
    bool ret = false;

    if (any.istype< bool >())
    {
        ret = writer->writeVal(any.value_as< bool >());
    }
    else if (any.istype< float >())
    {
        ret = writer->writeVal(any.value_as< float >());
    }
    else if (any.istype< double >())
    {
        ret = writer->writeVal(any.value_as< double >());
    }
    else if (any.istype< std::string >())
    {
        ret = writer->writeVal(any.value_as< std::string >());
    }

    else if (any.istype< int8_t >())
    {
        ret = writer->writeVal((int32_t)any.value_as< int8_t >());
    }
    else if (any.istype< int16_t >())
    {
        ret = writer->writeVal((int32_t)any.value_as< int16_t >());
    }

    else if (any.istype< int32_t >())
    {
        ret = writer->writeVal(any.value_as< int32_t >());
    }
    else if (any.istype< int64_t >())
    {
        ret = writer->writeVal(any.value_as< int64_t >());
    }

    else if (any.istype< uint8_t >())
    {
        ret = writer->writeVal((uint32_t)any.value_as< uint8_t >());
    }
    else if (any.istype< uint16_t >())
    {
        ret = writer->writeVal((uint32_t)any.value_as< uint16_t >());
    }

    else if (any.istype< uint32_t >())
    {
        ret = writer->writeVal(any.value_as< uint32_t >());
    }
    else if (any.istype< uint64_t >())
    {
        ret = writer->writeVal(any.value_as< uint64_t >());
    }

    return ret;
}
