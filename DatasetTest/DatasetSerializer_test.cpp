#include <gtest/gtest.h>
#include "JsonRW.h"
#include "dataset.h"
#include "DatasetSerializer.h"


class DatasetSerializerTest : public DatasetSerializer
{
public:
    DatasetSerializerTest() { };
    virtual ~DatasetSerializerTest() { };

    bool onWritingUnknownType(persist::Writer* writer, const dw::any& any)
    {
        return true;
    }
    bool onReadingUnknownType(persist::Reader* reader)
    {
        return true;
    }
};


static void fillDatasetWithTestData_1(Dataset& ds)
{
    ASSERT_TRUE(ds.set("data_1", dw::any(5)));
    ASSERT_TRUE(ds.set("data_2", dw::any(6.66f)));
    ASSERT_TRUE(ds.set("data_3", dw::any("I'm data_3")));
    ASSERT_TRUE(ds.set("data_4", dw::any('s')));
    ASSERT_TRUE(ds.set("data_5", dw::any(3.14)));
}

static void fillDatasetWithTestData_2(Dataset& ds)
{
    ASSERT_TRUE(ds.set("data_1", dw::any(100)));
    ASSERT_TRUE(ds.set("data_2", dw::any(233.33333f)));
    ASSERT_TRUE(ds.set("data_3", dw::any("This is data_3")));
    ASSERT_TRUE(ds.set("data_4", dw::any('l')));
    ASSERT_TRUE(ds.set("data_5", dw::any(1.732)));
}


TEST(Serializer_Test, BasicSerializeTest)
{
    Dataset ds1;
    persist::JsonRW jsonRw1;
    DatasetSerializer serializer;

    // Fill data
    fillDatasetWithTestData_1(ds1);

    // Test serialize
    ASSERT_TRUE(serializer.serialize(&ds1, &jsonRw1));

    const char* json = jsonRw1.getDoc();

    // ------------------------------------------------------------------------

    Dataset ds2;
    persist::JsonRW jsonRw2;

    // Test deserialize, without type specified

    ASSERT_TRUE(jsonRw2.parseDoc(json));
    ASSERT_TRUE(serializer.deserialize(&ds2, &jsonRw2));

    // Verify deserialized data, without type specified

    ASSERT_TRUE(ds2.exists("data_1"));
    ASSERT_TRUE(ds2.exists("data_2"));
    ASSERT_TRUE(ds2.exists("data_3"));
    ASSERT_TRUE(ds2.exists("data_4"));
    ASSERT_TRUE(ds2.exists("data_5"));

    ASSERT_TRUE(ds2.check< int32_t >("data_1", 5));
    ASSERT_TRUE(ds2.check< float >("data_2", 6.66f));
    ASSERT_TRUE(ds2.check< std::string >("data_3", std::string("I'm data_3")));
    ASSERT_TRUE(ds2.check< int32_t >("data_4", (int32_t)'s'));
    // Because according to the precision, the auto type detection think it's float...
    ASSERT_TRUE(ds2.check< float >("data_5", 3.14f));

    // ------------------------------------------------------------------------

    // Refill another data
    fillDatasetWithTestData_2(ds1);
    jsonRw2.rewind();

    // Verify data has been changed
    ASSERT_TRUE(ds1.check< int32_t >("data_1", 100));
    ASSERT_TRUE(ds1.check< float >("data_2", 233.33333f));
    ASSERT_TRUE(ds1.check< std::string >("data_3", std::string("This is data_3")));
    ASSERT_TRUE(ds1.check< char >("data_4", 'l'));
    ASSERT_TRUE(ds1.check< double >("data_5", 1.732));

    // Test deserialize, with type specified
    ASSERT_TRUE(serializer.deserialize(&ds1, &jsonRw2));

    // Verify deserialized data, with type specified

    ASSERT_TRUE(ds1.check< int32_t >("data_1", 5));
    ASSERT_TRUE(ds1.check< float >("data_2", 6.66f));
    ASSERT_TRUE(ds1.check< std::string >("data_3", std::string("I'm data_3")));
    ASSERT_TRUE(ds1.check< char >("data_4", 's'));
    ASSERT_TRUE(ds1.check< double >("data_5", 3.14));
}
