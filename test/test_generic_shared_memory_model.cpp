#include <stdio.h>

#include <gtest/gtest.h>

#include "GenericSharedMemoryModel.hpp"

using namespace std;

typedef struct _test_struct_base_t {
	int test_int;
	double test_double;
} test_struct_base_t;

typedef struct _test_struct_t {
	int test_int;
	double test_double;
	test_struct_base_t test_struct_base;
} test_struct_t;

TEST(GenericSharedMemoryModelTest, TestConstructor) {
	bool error = false;

	try {
		GenericSharedMemoryModel<int> test_int = GenericSharedMemoryModel<int>("test_int");
		GenericSharedMemoryModel<uint8_t> test_uint8_t = GenericSharedMemoryModel<uint8_t>("test_uint8_t");
		GenericSharedMemoryModel<uint16_t> test_uint16_t = GenericSharedMemoryModel<uint16_t>("test_uint16_t");
		GenericSharedMemoryModel<uint32_t> test_uint32_t = GenericSharedMemoryModel<uint32_t>("test_uint32_t");
		GenericSharedMemoryModel<uint64_t> test_uint64_t = GenericSharedMemoryModel<uint64_t>("test_uint64_t");
		GenericSharedMemoryModel<float> test_float = GenericSharedMemoryModel<float>("test_float");
		GenericSharedMemoryModel<double> test_double = GenericSharedMemoryModel<double>("test_double");
		GenericSharedMemoryModel<test_struct_t> test_test_struct_t = GenericSharedMemoryModel<test_struct_t>("test_test_struct_t");
	}
	catch (std::exception e)
	{
		error = true;
	}

	ASSERT_FALSE(error);
}


TEST(GenericSharedMemoryModelTest, TestConnect) {
	bool error = false;

	try {
		GenericSharedMemoryModel<int> test_int = GenericSharedMemoryModel<int>("test_int");
		GenericSharedMemoryModel<uint8_t> test_uint8_t = GenericSharedMemoryModel<uint8_t>("test_uint8_t");
		GenericSharedMemoryModel<uint16_t> test_uint16_t = GenericSharedMemoryModel<uint16_t>("test_uint16_t");
		GenericSharedMemoryModel<uint32_t> test_uint32_t = GenericSharedMemoryModel<uint32_t>("test_uint32_t");
		GenericSharedMemoryModel<uint64_t> test_uint64_t = GenericSharedMemoryModel<uint64_t>("test_uint64_t");
		GenericSharedMemoryModel<float> test_float = GenericSharedMemoryModel<float>("test_float");
		GenericSharedMemoryModel<double> test_double = GenericSharedMemoryModel<double>("test_double");
		GenericSharedMemoryModel<test_struct_t> test_test_struct_t = GenericSharedMemoryModel<test_struct_t>("test_test_struct_t");

		ASSERT_TRUE(test_int.connect());
		ASSERT_TRUE(test_uint8_t.connect());
		ASSERT_TRUE(test_uint16_t.connect());
		ASSERT_TRUE(test_uint32_t.connect());
		ASSERT_TRUE(test_uint64_t.connect());
		ASSERT_TRUE(test_float.connect());
		ASSERT_TRUE(test_double.connect());
		ASSERT_TRUE(test_test_struct_t.connect());

		ASSERT_TRUE(test_int.is_connected());
		ASSERT_TRUE(test_uint8_t.is_connected());
		ASSERT_TRUE(test_uint16_t.is_connected());
		ASSERT_TRUE(test_uint32_t.is_connected());
		ASSERT_TRUE(test_uint64_t.is_connected());
		ASSERT_TRUE(test_float.is_connected());
		ASSERT_TRUE(test_double.is_connected());
		ASSERT_TRUE(test_test_struct_t.is_connected());
	}
	catch (std::exception e)
	{
		error = true;
	}

	ASSERT_FALSE(error);
}

TEST(GenericSharedMemoryModelTest, TestDisconnect) {
	bool error = false;

	try {
		GenericSharedMemoryModel<int> test_int = GenericSharedMemoryModel<int>("test_int");
		GenericSharedMemoryModel<uint8_t> test_uint8_t = GenericSharedMemoryModel<uint8_t>("test_uint8_t");
		GenericSharedMemoryModel<uint16_t> test_uint16_t = GenericSharedMemoryModel<uint16_t>("test_uint16_t");
		GenericSharedMemoryModel<uint32_t> test_uint32_t = GenericSharedMemoryModel<uint32_t>("test_uint32_t");
		GenericSharedMemoryModel<uint64_t> test_uint64_t = GenericSharedMemoryModel<uint64_t>("test_uint64_t");
		GenericSharedMemoryModel<float> test_float = GenericSharedMemoryModel<float>("test_float");
		GenericSharedMemoryModel<double> test_double = GenericSharedMemoryModel<double>("test_double");
		GenericSharedMemoryModel<test_struct_t> test_test_struct_t = GenericSharedMemoryModel<test_struct_t>("test_test_struct_t");

		ASSERT_TRUE(test_int.connect());
		ASSERT_TRUE(test_uint8_t.connect());
		ASSERT_TRUE(test_uint16_t.connect());
		ASSERT_TRUE(test_uint32_t.connect());
		ASSERT_TRUE(test_uint64_t.connect());
		ASSERT_TRUE(test_float.connect());
		ASSERT_TRUE(test_double.connect());
		ASSERT_TRUE(test_test_struct_t.connect());

		ASSERT_TRUE(test_int.disconnect());
		ASSERT_TRUE(test_uint8_t.disconnect());
		ASSERT_TRUE(test_uint16_t.disconnect());
		ASSERT_TRUE(test_uint32_t.disconnect());
		ASSERT_TRUE(test_uint64_t.disconnect());
		ASSERT_TRUE(test_float.disconnect());
		ASSERT_TRUE(test_double.disconnect());
		ASSERT_TRUE(test_test_struct_t.disconnect());
	}
	catch (std::exception e)
	{
		error = true;
	}

	ASSERT_FALSE(error);
}

TEST(GenericSharedMemoryModelTest, TestReadWrite) {
	bool error = false;

	try {
		GenericSharedMemoryModel<int> test_write_int = GenericSharedMemoryModel<int>("test_int");
		GenericSharedMemoryModel<uint8_t> test_write_uint8_t = GenericSharedMemoryModel<uint8_t>("test_uint8_t");
		GenericSharedMemoryModel<uint16_t> test_write_uint16_t = GenericSharedMemoryModel<uint16_t>("test_uint16_t");
		GenericSharedMemoryModel<uint32_t> test_write_uint32_t = GenericSharedMemoryModel<uint32_t>("test_uint32_t");
		GenericSharedMemoryModel<uint64_t> test_write_uint64_t = GenericSharedMemoryModel<uint64_t>("test_uint64_t");
		GenericSharedMemoryModel<float> test_write_float = GenericSharedMemoryModel<float>("test_float");
		GenericSharedMemoryModel<double> test_write_double = GenericSharedMemoryModel<double>("test_double");
		GenericSharedMemoryModel<test_struct_t> test_write_test_struct_t = GenericSharedMemoryModel<test_struct_t>("test_test_struct_t");

		GenericSharedMemoryModel<int> test_read_int = GenericSharedMemoryModel<int>("test_int");
		GenericSharedMemoryModel<uint8_t> test_read_uint8_t = GenericSharedMemoryModel<uint8_t>("test_uint8_t");
		GenericSharedMemoryModel<uint16_t> test_read_uint16_t = GenericSharedMemoryModel<uint16_t>("test_uint16_t");
		GenericSharedMemoryModel<uint32_t> test_read_uint32_t = GenericSharedMemoryModel<uint32_t>("test_uint32_t");
		GenericSharedMemoryModel<uint64_t> test_read_uint64_t = GenericSharedMemoryModel<uint64_t>("test_uint64_t");
		GenericSharedMemoryModel<float> test_read_float = GenericSharedMemoryModel<float>("test_float");
		GenericSharedMemoryModel<double> test_read_double = GenericSharedMemoryModel<double>("test_double");
		GenericSharedMemoryModel<test_struct_t> test_read_test_struct_t = GenericSharedMemoryModel<test_struct_t>("test_test_struct_t");
		
		ASSERT_TRUE(test_write_int.connect());
		ASSERT_TRUE(test_write_uint8_t.connect());
		ASSERT_TRUE(test_write_uint16_t.connect());
		ASSERT_TRUE(test_write_uint32_t.connect());
		ASSERT_TRUE(test_write_uint64_t.connect());
		ASSERT_TRUE(test_write_float.connect());
		ASSERT_TRUE(test_write_double.connect());
		ASSERT_TRUE(test_write_test_struct_t.connect());

		ASSERT_TRUE(test_read_int.connect());
		ASSERT_TRUE(test_read_uint8_t.connect());
		ASSERT_TRUE(test_read_uint16_t.connect());
		ASSERT_TRUE(test_read_uint32_t.connect());
		ASSERT_TRUE(test_read_uint64_t.connect());
		ASSERT_TRUE(test_read_float.connect());
		ASSERT_TRUE(test_read_double.connect());
		ASSERT_TRUE(test_read_test_struct_t.connect());

		int test_data_int = 42;
		uint8_t test_data_uint8_t = 42;
		uint16_t test_data_uint16_t = 42;
		uint32_t test_data_uint32_t = 42;
		uint64_t test_data_uint64_t = 42;
		float test_data_float = 42.42;
		double test_data_double = 42.42;
		test_struct_t test_data_test_struct_t = {42, 42.42, {42, 42.42}};

		test_write_int.write_data(test_data_int);
		test_write_uint8_t.write_data(test_data_uint8_t);
		test_write_uint16_t.write_data(test_data_uint16_t);
		test_write_uint32_t.write_data(test_data_uint32_t);
		test_write_uint64_t.write_data(test_data_uint64_t);
		test_write_float.write_data(test_data_float);
		test_write_double.write_data(test_data_double);
		test_write_test_struct_t.write_data(test_data_test_struct_t);

		ASSERT_EQ(test_read_int.get_data(), test_data_int);
		ASSERT_EQ(test_read_uint8_t.get_data(), test_data_uint8_t);
		ASSERT_EQ(test_read_uint16_t.get_data(), test_data_uint16_t);
		ASSERT_EQ(test_read_uint32_t.get_data(), test_data_uint32_t);
		ASSERT_EQ(test_read_uint64_t.get_data(), test_data_uint64_t);
		ASSERT_EQ(test_read_float.get_data(), test_data_float);
		ASSERT_EQ(test_read_double.get_data(), test_data_double);
		ASSERT_EQ(test_read_test_struct_t.get_data().test_double, test_data_test_struct_t.test_double);
		ASSERT_EQ(test_read_test_struct_t.get_data().test_int, test_data_test_struct_t.test_int);
		ASSERT_EQ(test_read_test_struct_t.get_data().test_struct_base.test_double, test_data_test_struct_t.test_struct_base.test_double);
		ASSERT_EQ(test_read_test_struct_t.get_data().test_struct_base.test_int, test_data_test_struct_t.test_struct_base.test_int);

		ASSERT_EQ(*test_read_int.data, test_data_int);
		ASSERT_EQ(*test_read_uint8_t.data, test_data_uint8_t);
		ASSERT_EQ(*test_read_uint16_t.data, test_data_uint16_t);
		ASSERT_EQ(*test_read_uint32_t.data, test_data_uint32_t);
		ASSERT_EQ(*test_read_uint64_t.data, test_data_uint64_t);
		ASSERT_EQ(*test_read_float.data, test_data_float);
		ASSERT_EQ(*test_read_double.data, test_data_double);
		ASSERT_EQ(test_read_test_struct_t.data->test_double, test_data_test_struct_t.test_double);
		ASSERT_EQ(test_read_test_struct_t.data->test_int, test_data_test_struct_t.test_int);
		ASSERT_EQ(test_read_test_struct_t.data->test_struct_base.test_double, test_data_test_struct_t.test_struct_base.test_double);
		ASSERT_EQ(test_read_test_struct_t.data->test_struct_base.test_int, test_data_test_struct_t.test_struct_base.test_int);

		ASSERT_TRUE(test_write_int.disconnect());
		ASSERT_TRUE(test_write_uint8_t.disconnect());
		ASSERT_TRUE(test_write_uint16_t.disconnect());
		ASSERT_TRUE(test_write_uint32_t.disconnect());
		ASSERT_TRUE(test_write_uint64_t.disconnect());
		ASSERT_TRUE(test_write_float.disconnect());
		ASSERT_TRUE(test_write_double.disconnect());
		ASSERT_TRUE(test_write_test_struct_t.disconnect());

		ASSERT_TRUE(test_read_int.disconnect());
		ASSERT_TRUE(test_read_uint8_t.disconnect());
		ASSERT_TRUE(test_read_uint16_t.disconnect());
		ASSERT_TRUE(test_read_uint32_t.disconnect());
		ASSERT_TRUE(test_read_uint64_t.disconnect());
		ASSERT_TRUE(test_read_float.disconnect());
		ASSERT_TRUE(test_read_double.disconnect());
		ASSERT_TRUE(test_read_test_struct_t.disconnect());
	}
	catch (std::exception e)
	{
		error = true;
	}

	ASSERT_FALSE(error);
}