#include <gtest/gtest.h>

#include "./../dataset.h"

TEST(Dataset_Test, BasicSetGetTest)
{
	Dataset ds;

	ds.set("data_1", dw::any(5));
	ds.set("data_2", dw::any(6.66f));
	ds.set("data_3", dw::any("I'm data_3"));
	ds.set("data_4", dw::any('s'));
	ds.set("data_5", dw::any(3.14));

	auto data_1 = ds.get("data_1");
	auto data_2 = ds.get("data_2");
	auto data_3 = ds.get("data_3");
	auto data_4 = ds.get("data_4");
	auto data_5 = ds.get("data_5");

	ASSERT_EQ(5,			data_1.value_as< int >());
	ASSERT_EQ(6.66f,		data_2.value_as< float >());
	ASSERT_EQ("I'm data_3", data_3.value_as< std::string >());
	ASSERT_EQ('s',			data_4.value_as< char >());
	ASSERT_EQ(3.14,			data_5.value_as< double >());
}

TEST(Dataset_Test, ErrorSetGetTest)
{
	Dataset ds;

	ds.set("data_1", dw::any(5));

	auto data_1 = ds.get("data_1");
	auto data_2 = ds.get("data_2");
	auto data_3 = ds.get("data_3");
	auto data_4 = ds.get("data_4");
	auto data_5 = ds.get("data_5");

	ASSERT_TRUE (data_1.valid());
	ASSERT_FALSE(data_2.valid());
	ASSERT_FALSE(data_3.valid());
	ASSERT_FALSE(data_4.valid());
	ASSERT_FALSE(data_5.valid());

	ASSERT_EQ(5,		data_1.value_as< int >());
	ASSERT_EQ(0.0f,		data_2.value_as< float >());
	ASSERT_EQ("",		data_3.value_as< std::string >());
	ASSERT_EQ('\0',		data_4.value_as< char >());
	ASSERT_EQ(0.0,		data_5.value_as< double >());
}

TEST(Dataset_Test, MultiTimesSetTest)
{
	bool ret = false;
	Dataset ds;

	ASSERT_TRUE(ds.set("data_1", dw::any(5)));
	ASSERT_TRUE(ds.set("data_1", dw::any(9)));

	ASSERT_FALSE(ds.set("data_1", dw::any(6.66f)));
	ASSERT_FALSE(ds.set("data_1", dw::any("I'm data_3")));
	ASSERT_FALSE(ds.set("data_1", dw::any('s')));
	ASSERT_FALSE(ds.set("data_1", dw::any(3.14)));

	auto data_a = ds.get("data_1");
	ASSERT_EQ(9, data_a.value_as< int >());

	ds.lockType(false);

	ASSERT_TRUE(ds.set("data_1", dw::any(6.66f)));
	ASSERT_TRUE(ds.set("data_1", dw::any("I'm data_3")));
	ASSERT_TRUE(ds.set("data_1", dw::any('s')));
	ASSERT_TRUE(ds.set("data_1", dw::any(3.14)));

	auto data_b = ds.get("data_1");
	ASSERT_EQ(3.14, data_b.value_as< double >());
}

class TestObserver : public IDataObserver
{
public:
	std::string m_name;
	DATALIST m_firstData;
	DATALIST m_changedData;
	DATALIST m_batChangedData;

public:
	TestObserver(std::string name) : m_name(name)
	{

	}
	virtual ~TestObserver()
	{

	}

	virtual void onFirstData(std::string key, const dw::any& value)
	{
		m_firstData.push_back(std::make_tuple(key, value));
	}
	virtual void onDataChanged(std::string key, const dw::any& value)
	{
		m_changedData.push_back(std::make_tuple(key, value));
	}
	virtual void onBatchDataChanged(const DATALIST& values)
	{
		m_batChangedData.insert(m_batChangedData.end(), values.begin(), values.end());
	}
};

TEST(Dataset_Test, ObserverBasicTest)
{
	Dataset ds;
	TestObserver* ob1 = new TestObserver("Observer 1");
	TestObserver* ob2 = new TestObserver("Observer 2");

	// Key must not empty and the ob must not NULL
	ASSERT_FALSE(ds.addDataObserver("", ob1));
	ASSERT_FALSE(ds.addDataObserver("data_1", NULL));

	// Add ob sucessfully, there's no "data_1" yet, no first data.
	ASSERT_TRUE(ds.addDataObserver("data_1", ob1));
	ASSERT_TRUE(ob1->m_firstData.empty());

	// Set "data_1" data
	ASSERT_TRUE(ds.set("data_1", dw::any(5)));

	// Add ob2, it should get the first data.
	ASSERT_TRUE(ds.addDataObserver("data_1", ob2));
	ASSERT_EQ(1,		ob1->m_changedData.size());
	ASSERT_EQ("data_1", std::get<0>(ob1->m_changedData.front()));
	ASSERT_EQ(5,		std::get<1>(ob1->m_changedData.front()).value_as<int>());

	// Add the same ob to the same key, it should be failed.
	ob1->m_firstData.clear();
	ASSERT_FALSE(ds.addDataObserver("data_1", ob1));
	ASSERT_TRUE(ob1->m_firstData.empty());
}

TEST(Dataset_Test, MultiObserverTest)
{
	Dataset ds;
	TestObserver* ob_135 = new TestObserver("Observer 135");
	TestObserver* ob_246 = new TestObserver("Observer 246");
	TestObserver* ob_123 = new TestObserver("Observer 123");
	TestObserver* ob_456 = new TestObserver("Observer 456");
	TestObserver* ob_234 = new TestObserver("Observer 234");

	ASSERT_TRUE(ds.addDataObserver("data_1", ob_135));
	ASSERT_TRUE(ds.addDataObserver("data_3", ob_135));
	ASSERT_TRUE(ds.addDataObserver("data_5", ob_135));

	ASSERT_TRUE(ds.addDataObserver("data_2", ob_246));
	ASSERT_TRUE(ds.addDataObserver("data_4", ob_246));
	ASSERT_TRUE(ds.addDataObserver("data_6", ob_246));

	ASSERT_TRUE(ds.addDataObserver("data_1", ob_123));
	ASSERT_TRUE(ds.addDataObserver("data_2", ob_123));
	ASSERT_TRUE(ds.addDataObserver("data_3", ob_123));

	ASSERT_TRUE(ds.addDataObserver("data_4", ob_456));
	ASSERT_TRUE(ds.addDataObserver("data_5", ob_456));
	ASSERT_TRUE(ds.addDataObserver("data_6", ob_456));

	// Update data.

	ASSERT_TRUE(ds.set("data_1", dw::any(1001)));
	ASSERT_TRUE(ds.set("data_2", dw::any(2001)));
	ASSERT_TRUE(ds.set("data_3", dw::any(3001)));
	ASSERT_TRUE(ds.set("data_4", dw::any(4001)));
	ASSERT_TRUE(ds.set("data_5", dw::any(5001)));
	ASSERT_TRUE(ds.set("data_6", dw::any(6001)));

	ASSERT_TRUE(ds.set("data_2", dw::any(1002)));
	ASSERT_TRUE(ds.set("data_3", dw::any(2002)));
	ASSERT_TRUE(ds.set("data_4", dw::any(3002)));
	ASSERT_TRUE(ds.set("data_5", dw::any(4002)));
	ASSERT_TRUE(ds.set("data_6", dw::any(5002)));

	ASSERT_TRUE(ds.set("data_3", dw::any(1003)));
	ASSERT_TRUE(ds.set("data_4", dw::any(2003)));
	ASSERT_TRUE(ds.set("data_5", dw::any(3003)));
	ASSERT_TRUE(ds.set("data_6", dw::any(4003)));

	ASSERT_TRUE(ds.set("data_4", dw::any(1004)));
	ASSERT_TRUE(ds.set("data_5", dw::any(2004)));
	ASSERT_TRUE(ds.set("data_6", dw::any(3004)));

	ASSERT_TRUE(ds.set("data_5", dw::any(1005)));
	ASSERT_TRUE(ds.set("data_6", dw::any(2005)));

	ASSERT_TRUE(ds.set("data_6", dw::any(1006)));

	// Check the data count ob gets.

	int nIndex;

	nIndex = 0;
	ASSERT_EQ(1 + 3 + 5, ob_135->m_changedData.size());

	ASSERT_EQ("data_1", std::get<0>(ob_135->m_changedData[nIndex++]));
	ASSERT_EQ("data_3", std::get<0>(ob_135->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_135->m_changedData[nIndex++]));

	ASSERT_EQ("data_3", std::get<0>(ob_135->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_135->m_changedData[nIndex++]));

	ASSERT_EQ("data_3", std::get<0>(ob_135->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_135->m_changedData[nIndex++]));

	ASSERT_EQ("data_5", std::get<0>(ob_135->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_135->m_changedData[nIndex++]));

	nIndex = 0;
	ASSERT_EQ(2 + 4 + 6, ob_246->m_changedData.size());

	ASSERT_EQ("data_2", std::get<0>(ob_246->m_changedData[nIndex++]));
	ASSERT_EQ("data_4", std::get<0>(ob_246->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_246->m_changedData[nIndex++]));

	ASSERT_EQ("data_2", std::get<0>(ob_246->m_changedData[nIndex++]));
	ASSERT_EQ("data_4", std::get<0>(ob_246->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_246->m_changedData[nIndex++]));

	ASSERT_EQ("data_4", std::get<0>(ob_246->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_246->m_changedData[nIndex++]));

	ASSERT_EQ("data_4", std::get<0>(ob_246->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_246->m_changedData[nIndex++]));

	ASSERT_EQ("data_6", std::get<0>(ob_246->m_changedData[nIndex++]));

	ASSERT_EQ("data_6", std::get<0>(ob_246->m_changedData[nIndex++]));

	nIndex = 0;
	ASSERT_EQ(1 + 2 + 3, ob_123->m_changedData.size());

	ASSERT_EQ("data_1", std::get<0>(ob_123->m_changedData[nIndex++]));
	ASSERT_EQ("data_2", std::get<0>(ob_123->m_changedData[nIndex++]));
	ASSERT_EQ("data_3", std::get<0>(ob_123->m_changedData[nIndex++]));

	ASSERT_EQ("data_2", std::get<0>(ob_123->m_changedData[nIndex++]));
	ASSERT_EQ("data_3", std::get<0>(ob_123->m_changedData[nIndex++]));

	ASSERT_EQ("data_3", std::get<0>(ob_123->m_changedData[nIndex++]));

	nIndex = 0;
	ASSERT_EQ(4 + 5 + 6, ob_456->m_changedData.size());

	ASSERT_EQ("data_4", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_456->m_changedData[nIndex++]));

	ASSERT_EQ("data_4", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_456->m_changedData[nIndex++]));

	ASSERT_EQ("data_4", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_456->m_changedData[nIndex++]));

	ASSERT_EQ("data_4", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_5", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_456->m_changedData[nIndex++]));

	ASSERT_EQ("data_5", std::get<0>(ob_456->m_changedData[nIndex++]));
	ASSERT_EQ("data_6", std::get<0>(ob_456->m_changedData[nIndex++]));

	ASSERT_EQ("data_6", std::get<0>(ob_456->m_changedData[nIndex++]));

	// Add ob and check the first data count.

	ASSERT_TRUE(ds.addDataObserver("data_2", ob_234));
	ASSERT_TRUE(ds.addDataObserver("data_3", ob_234));
	ASSERT_TRUE(ds.addDataObserver("data_4", ob_234));

	nIndex = 0;
	ASSERT_EQ(3, ob_234->m_firstData.size());

	ASSERT_EQ("data_2", std::get<0>(ob_234->m_firstData[nIndex++]));
	ASSERT_EQ("data_3", std::get<0>(ob_234->m_firstData[nIndex++]));
	ASSERT_EQ("data_4", std::get<0>(ob_234->m_firstData[nIndex++]));
}


TEST(Dataset_Test, MultiDataUpdateTest)
{
	Dataset ds;
	TestObserver* ob_135 = new TestObserver("Observer 135");
	TestObserver* ob_246 = new TestObserver("Observer 246");
	TestObserver* ob_123 = new TestObserver("Observer 123");
	TestObserver* ob_456 = new TestObserver("Observer 456");
	TestObserver* ob_234 = new TestObserver("Observer 234");

	ASSERT_TRUE(ds.addDataObserver("data_1", ob_135));
	ASSERT_TRUE(ds.addDataObserver("data_3", ob_135));
	ASSERT_TRUE(ds.addDataObserver("data_5", ob_135));

	ASSERT_TRUE(ds.addDataObserver("data_2", ob_246));
	ASSERT_TRUE(ds.addDataObserver("data_4", ob_246));
	ASSERT_TRUE(ds.addDataObserver("data_6", ob_246));

	ASSERT_TRUE(ds.addDataObserver("data_1", ob_123));
	ASSERT_TRUE(ds.addDataObserver("data_2", ob_123));
	ASSERT_TRUE(ds.addDataObserver("data_3", ob_123));

	ASSERT_TRUE(ds.addDataObserver("data_4", ob_456));
	ASSERT_TRUE(ds.addDataObserver("data_5", ob_456));
	ASSERT_TRUE(ds.addDataObserver("data_6", ob_456));

	ASSERT_TRUE(ds.addDataObserver("data_2", ob_234));
	ASSERT_TRUE(ds.addDataObserver("data_3", ob_234));
	ASSERT_TRUE(ds.addDataObserver("data_4", ob_234));

	DATADICT dd;
	dd["data_1"] = dw::any(1001);
	dd["data_2"] = dw::any(2001);
	dd["data_3"] = dw::any(3001);
	dd["data_4"] = dw::any(4001);
	dd["data_5"] = dw::any(5001);
	dd["data_6"] = dw::any(6001);

	// Set 1 - 6

	ds.sets(dd);

	ASSERT_EQ(3, ob_135->m_batChangedData.size());
	ASSERT_EQ(3, ob_246->m_batChangedData.size());
	ASSERT_EQ(3, ob_123->m_batChangedData.size());
	ASSERT_EQ(3, ob_456->m_batChangedData.size());
	ASSERT_EQ(3, ob_234->m_batChangedData.size());

	ob_135->m_batChangedData.clear();
	ob_246->m_batChangedData.clear();
	ob_123->m_batChangedData.clear();
	ob_456->m_batChangedData.clear();
	ob_234->m_batChangedData.clear();

	// Set 2 - 6

	dd.erase("data_1");
	ds.sets(dd);

	ASSERT_EQ(2, ob_135->m_batChangedData.size());
	ASSERT_EQ(3, ob_246->m_batChangedData.size());
	ASSERT_EQ(2, ob_123->m_batChangedData.size());
	ASSERT_EQ(3, ob_456->m_batChangedData.size());
	ASSERT_EQ(3, ob_234->m_batChangedData.size());

	ob_135->m_batChangedData.clear();
	ob_246->m_batChangedData.clear();
	ob_123->m_batChangedData.clear();
	ob_456->m_batChangedData.clear();
	ob_234->m_batChangedData.clear();

	// Set 3 - 6

	dd.erase("data_2");
	ds.sets(dd);

	ASSERT_EQ(2, ob_135->m_batChangedData.size());
	ASSERT_EQ(2, ob_246->m_batChangedData.size());
	ASSERT_EQ(0, ob_123->m_batChangedData.size());
	ASSERT_EQ(3, ob_456->m_batChangedData.size());
	ASSERT_EQ(2, ob_234->m_batChangedData.size());

	ob_135->m_batChangedData.clear();
	ob_246->m_batChangedData.clear();
	ob_123->m_batChangedData.clear();
	ob_456->m_batChangedData.clear();
	ob_234->m_batChangedData.clear();
}

TEST(Dataset_Test, DatasetCheckTest)
{
    Dataset ds;

    // Const data [type] + c

    char cc = 'a';
    unsigned char ucc = 'b';

    const int8_t i8c = -100;
    const uint8_t u8c = 254;

    const int16_t i16c = -222;
    const uint16_t u16c = 333;

    const int32_t i32c = -4444;
    const uint32_t u32c = 5555;

    const int64_t i64c = -666666;
    const uint64_t u64c = 299792458;

    const float fc = 3.14159f;
    const double dc = 6.62607004;

    const char* csc = "SleepySoft";
    std::string ssc = "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks";

    // Any data [type] + a

    dw::any ca(cc);
    dw::any uca(ucc);

    dw::any i8a(i8c);
    dw::any u8a(u8c);

    dw::any i16a(i16c);
    dw::any u16a(u16c);

    dw::any i32a(i32c);
    dw::any u32a(u32c);

    dw::any i64a(i64c);
    dw::any u64a(u64c);

    dw::any fa(fc);
    dw::any da(dc);

    dw::any csa(csc);
    dw::any ssa(ssc);

    // Set to dataset

    ASSERT_TRUE(ds.set("ca", ca));
    ASSERT_TRUE(ds.set("uca", uca));

    ASSERT_TRUE(ds.set("i8a", i8a));
    ASSERT_TRUE(ds.set("u8a", u8a));

    ASSERT_TRUE(ds.set("i16a", i16a));
    ASSERT_TRUE(ds.set("u16a", u16a));

    ASSERT_TRUE(ds.set("i32a", i32a));
    ASSERT_TRUE(ds.set("u32a", u32a));

    ASSERT_TRUE(ds.set("i64a", i64a));
    ASSERT_TRUE(ds.set("u64a", u64a));

    ASSERT_TRUE(ds.set("fa", fc));
    ASSERT_TRUE(ds.set("da", dc));

    ASSERT_TRUE(ds.set("csa", csa));
    ASSERT_TRUE(ds.set("ssa", ssa));

    // Verify with const value

    EXPECT_TRUE(ds.check("ca", cc));
    EXPECT_TRUE(ds.check("uca", ucc));

    EXPECT_TRUE(ds.check("i8a", i8c));
    EXPECT_TRUE(ds.check("u8a", u8c));

    EXPECT_TRUE(ds.check("i16a", i16c));
    EXPECT_TRUE(ds.check("u16a", u16c));

    EXPECT_TRUE(ds.check("i32a", i32c));
    EXPECT_TRUE(ds.check("u32a", u32c));

    EXPECT_TRUE(ds.check("i64a", i64c));
    EXPECT_TRUE(ds.check("u64a", u64c));

    EXPECT_TRUE(ds.check("fa", fc));
    EXPECT_TRUE(ds.check("da", dc));

    EXPECT_TRUE(ds.check("csa", csc));
    EXPECT_TRUE(ds.check("ssa", ssc));

    EXPECT_TRUE(ds.check("csa", "SleepySoft"));
    EXPECT_TRUE(ds.check("ssa", "The Times 03/Jan/2009 Chancellor on brink of second bailout for banks"));

    // Verify with 'any' data

    //EXPECT_TRUE(ds.check("ca", ca));
    //EXPECT_TRUE(ds.check("uca", uca));

    //EXPECT_TRUE(ds.check("i8a", i8a));
    //EXPECT_TRUE(ds.check("u8a", u8a));

    //EXPECT_TRUE(ds.check("i16a", i16a));
    //EXPECT_TRUE(ds.check("u16a", u16a));

    //EXPECT_TRUE(ds.check("i32a", i32a));
    //EXPECT_TRUE(ds.check("u32a", u32a));

    //EXPECT_TRUE(ds.check("i64a", i64a));
    //EXPECT_TRUE(ds.check("u64a", u64a));

    //EXPECT_TRUE(ds.check("fa", fa));
    //EXPECT_TRUE(ds.check("da", da));

    //EXPECT_TRUE(ds.check("csa", csa));
    //EXPECT_TRUE(ds.check("ssa", ssa));
}
