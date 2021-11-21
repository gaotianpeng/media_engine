#include <gtest/gtest.h>
#include <string>
#include "nlohmann/json.hpp"

using nlohmann::json;
using namespace std;

TEST(HelloTest, BasicAssertions) {
	// Expect two strings not to be equal.
	EXPECT_STRNE("hello", "world");
	// Expect equality.
	EXPECT_EQ(7 * 6, 42);
}

TEST(JsonTest, HelloJson) {
	json j1;
	j1["pi"] = 3.141;
	j1["happy"] = true;
	j1["name"] = "Niels";
	j1["nothing"] = nullptr;
	j1["answer"]["everything"] = 42;
	j1["list"] = { 1, 0, 2 };
	j1["object"] = { {"currency", "USD"}, {"value", 42.99} };

	json j2 = {
		{"pi", 3.141},
		{"happy", true},
		{"name", "Niels"},
		{"nothing", nullptr},
		{"answer", {
			{"everything", 42}
		}},
		{"list", {1, 0, 2}},
		{"object", {
			{"currency", "USD"},
			{"value", 42.99}
		}}
	};

	EXPECT_TRUE(j1.dump() == j2.dump());

	auto j3 = json::parse(j1.dump());
	EXPECT_EQ(3.141, j3["pi"]);
}
