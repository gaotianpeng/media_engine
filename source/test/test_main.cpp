#include <limits.h>
#include "sample.h"
#include "gtest.h"
#include "easylogging++.h"

#include "../me/math.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"

INITIALIZE_EASYLOGGINGPP

using ::testing::InitGoogleTest;
using namespace rapidjson; 

namespace {

TEST(FactorialTest, Negative) {

  EXPECT_EQ(1, Factorial(-5));
  EXPECT_EQ(1, Factorial(-1));
  EXPECT_GT(Factorial(-10), 0);

}

// Tests factorial of 0.
TEST(FactorialTest, Zero) {
  EXPECT_EQ(1, Factorial(0));
}

// Tests factorial of positive numbers.
TEST(FactorialTest, Positive) {
  EXPECT_EQ(1, Factorial(1));
  EXPECT_EQ(2, Factorial(2));
  EXPECT_EQ(6, Factorial(3));
  EXPECT_EQ(40320, Factorial(8));
}

TEST(IsPrimeTest, Negative) {
  // This test belongs to the IsPrimeTest test case.

  EXPECT_FALSE(IsPrime(-1));
  EXPECT_FALSE(IsPrime(-2));
  EXPECT_FALSE(IsPrime(INT_MIN));
}

// Tests some trivial cases.
TEST(IsPrimeTest, Trivial) {
  EXPECT_FALSE(IsPrime(0));
  EXPECT_FALSE(IsPrime(1));
  EXPECT_TRUE(IsPrime(2));
  EXPECT_TRUE(IsPrime(3));
}

// Tests positive input.
TEST(IsPrimeTest, Positive) {
  EXPECT_FALSE(IsPrime(4));
  EXPECT_TRUE(IsPrime(5));
  EXPECT_FALSE(IsPrime(6));
  EXPECT_TRUE(IsPrime(23));
}
}  // namespace


int main(int argc, char **argv) 
{
	InitGoogleTest(&argc, argv);

	LOG(INFO) << "Starting...";
	
	////////////////////////////////////////////////////////////////////////////
	// 1. Parse a JSON text string to a document.

	const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
	printf("Original JSON:\n %s\n", json);

	Document document; 
	char buffer[sizeof(json)]; 
	memcpy(buffer, json, sizeof(json)); 
	if (document.ParseInsitu(buffer).HasParseError())
		return 1; 

	printf("\nAccess values in document:\n");
	assert(document.IsObject());    // Document is a JSON value represents the root of DOM. Root can be either an object or array.

	assert(document.HasMember("hello"));
	assert(document["hello"].IsString());
	printf("hello = %s\n", document["hello"].GetString());

	print_test(); 
	int ret_val = RUN_ALL_TESTS();
	return ret_val;
}
