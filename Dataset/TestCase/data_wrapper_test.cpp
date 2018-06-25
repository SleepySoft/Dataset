#include <gtest/gtest.h>
#include "../data_wrapper.h"

TEST(data_wrapper_Test, SetAndGetConstTypes)
{
	dw::any a1('a');
	dw::any a2(12);
	dw::any a3(34.56f);
	dw::any a4(78.90);
	dw::any a5("bbb");

	char* p1 = a1.pointer_as< char >();
	char v1 = a1.value_as< char >(0);

	EXPECT_EQ('a', *p1);
	EXPECT_EQ('a', v1);

	int* p2 = a2.pointer_as< int >();
	int v2 = a2.value_as(0);

	EXPECT_EQ(12, *p2);
	EXPECT_EQ(12, v2);

	float* p3 = a3.pointer_as< float >();
	float v3 = a3.value_as(0.0f);

	EXPECT_EQ(34.56f, *p3);
	EXPECT_EQ(34.56f, v3);

	double* p4 = a4.pointer_as< double >();
	double v4 = a4.value_as(0.0);

	EXPECT_EQ(78.90, *p4);
	EXPECT_EQ(78.90, v4);

	std::string* p5 = a5.pointer_as< std::string >();
	std::string v5 = a5.value_as< std::string >();

	EXPECT_EQ("bbb", *p5);
	EXPECT_EQ("bbb", v5);

}

TEST(data_wrapper_Test, SetAndGetStrictTypes)
{
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

	char* cp = ca.pointer_as< char >();
	char ucv = uca.value_as< unsigned char >();

	EXPECT_EQ(cc, *cp);
	EXPECT_EQ(ucc, ucv);

	int8_t* i8p = i8a.pointer_as< int8_t >();
	uint8_t i8v = u8a.value_as< uint8_t >();

	EXPECT_EQ(i8c, *i8p);
	EXPECT_EQ(u8c, i8v);

	int16_t* i16p = i16a.pointer_as< int16_t >();
	uint16_t i16v = u16a.value_as< uint16_t >();

	EXPECT_EQ(i16c, *i16p);
	EXPECT_EQ(u16c, i16v);

	int32_t* i32p = i32a.pointer_as< int32_t >();
	uint32_t i32v = u32a.value_as< uint32_t >();

	EXPECT_EQ(i32c, *i32p);
	EXPECT_EQ(u32c, i32v);

	int64_t* i64p = i64a.pointer_as< int64_t >();
	uint64_t i64v = u64a.value_as< uint64_t >();

	EXPECT_EQ(i64c, *i64p);
	EXPECT_EQ(u64c, i64v);

	float* fap = fa.pointer_as< float >();
	double dav = da.value_as< double >();

	EXPECT_EQ(fc, *fap);
	EXPECT_EQ(dc, dav);

	std::string* csp = csa.pointer_as< std::string >();
	std::string ssp = ssa.value_as< std::string >();

	EXPECT_EQ(csc, *csp);
	EXPECT_EQ(ssc, ssp);
}

TEST(data_wrapper_Test, SetAndGetWrongTypes)
{
	dw::any a((int32_t)627);

	uint8_t* u8p = a.pointer_as< uint8_t >();
	uint8_t u8v = a.value_as< uint8_t >();

	EXPECT_EQ(NULL, u8p);
	EXPECT_EQ(0, u8v);

	uint16_t* u16p = a.pointer_as< uint16_t >();
	uint16_t u16v = a.value_as< uint16_t >();

	EXPECT_EQ(NULL, u16p);
	EXPECT_EQ(0, u16v);

	uint32_t* u32p = a.pointer_as< uint32_t >();
	uint32_t u32v = a.value_as< uint32_t >(10);

	EXPECT_EQ(NULL, u32p);
	EXPECT_EQ(10, u32v);

	float* fp = a.pointer_as< float >();
	float fv = a.value_as< float >();

	EXPECT_EQ(NULL, fp);
	EXPECT_EQ(0.0f, fv);

	double* dp = a.pointer_as< double >();
	double dv = a.value_as< double >(5.0);

	EXPECT_EQ(NULL, dp);
	EXPECT_EQ(5.0f, dv);

	std::string* ssp = a.pointer_as< std::string >();
	std::string ssv = a.value_as< std::string >("error");

	EXPECT_EQ(NULL, ssp);
	EXPECT_EQ(std::string("error"), ssv);
}