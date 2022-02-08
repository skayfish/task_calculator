#pragma once

#include <string>

namespace tester_calculator {

template <typename T, typename U>
void AssertEqualImpl(
	const T& t, 
	const U& u, 
	const std::string& t_str, 
	const std::string& u_str, 
	const std::string& file,
	const std::string& func, 
	unsigned line, 
	const std::string& hint
);

#define ASSERT_EQUAL(a, b) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_EQUAL_HINT(a, b, hint) AssertEqualImpl((a), (b), #a, #b, __FILE__, __FUNCTION__, __LINE__, (hint))

void AssertImpl(
	bool value, 
	const std::string& expr_str, 
	const std::string& file, 
	const std::string& func,
	unsigned line,
	const std::string& hint
);

#define ASSERT(expr) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, ""s)
#define ASSERT_HINT(expr, hint) AssertImpl(!!(expr), #expr, __FILE__, __FUNCTION__, __LINE__, (hint))

template <typename F>
void RunTestImpl(F func, const std::string& func_str);

#define RUN_TEST(func)  RunTestImpl(func, #func)

void TestCalculator();

} // namespace tester_calculator