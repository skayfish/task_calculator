#include <ios>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "tester_calculator.h"
#include "expression.h"

using namespace std;

namespace tester_calculator {

template <typename T, typename U>
void AssertEqualImpl(
	const T& t, 
	const U& u, 
	const string& t_str, 
	const string& u_str, 
	const string& file,
	const string& func, 
	unsigned line, 
	const string& hint
) {
	if (t != u) {
		cerr << boolalpha;
		cerr << file << "("s << line << "): "s << func << ": "s;
		cerr << "ASSERT_EQUAL("s << t_str << ", "s << u_str << ") failed: "s;
		cerr << t << " != "s << u << "."s;
		if (!hint.empty()) {
			cerr << " Hint: "s << hint;
		}
		cerr << endl;
		abort();
	}
}

void AssertImpl(
	bool value, 
	const string& expr_str, 
	const string& file, 
	const string& func,
	unsigned line, 
	const string& hint
) {
	if (!value) {
		cerr << file << "("s << line << "): "s << func << ": "s;
		cerr << "ASSERT("s << expr_str << ") failed."s;
		if (!hint.empty()) {
			cerr << " Hint: "s << hint;
		}
		cerr << endl;
		abort();
	}
}

template <typename F>
void RunTestImpl(F func, const string& func_str) {
	func();
	cerr << func_str << " OK" << endl;
}

namespace detail {

void TestBracketSequenceValid() {
		expression::Expression e;

		string str = ""s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, true);

		str = "()"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, true);

		str = "(())"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, true);

		str = "()()"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, true);
		
		str = "(())()"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, true);

		str = "("s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, false);

		str = ")"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, false);

		str = "()("s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, false);

		str = "())"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, false);

		str = "(()"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, false);

		str = "(7657))"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, false);

		str = "(341"s;
		e.SetExpression(str);
		ASSERT_EQUAL(e.IsCorrect().first, false);
}

void TestExtraCharacters() {
	expression::Expression e;

	string str = ""s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, true);

	str = "3 + 19"s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, true);

	str = "(3 + 19) * 13.4"s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, true);

	str = "(       3        + 1               9) * 13      " +
		  "     .4           "s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, true);

	str = "abcd"s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, false);

	str = "3 + 19 / c * !!"s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, false);

	str = "((a + b) * 13,2 + 5)"s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, false);

	str = "(       (a           +   b)        * 1      "s +
		  "   3.2            +                       5)"s;
	e.SetExpression(str);
	ASSERT_EQUAL(e.IsCorrect().first, false);
}

void TestCorrectOfExpression() {
    RUN_TEST(TestBracketSequenceValid);
	RUN_TEST(TestExtraCharacters);
}

void TestSolve() {
	expression::Expression e;
	stringstream ss(stringstream::in | stringstream::out);
	ss << fixed;
	ss.precision(2);

	string str = ""s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "1"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "1.00");
	ss.str("");

	str = "11.0"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "11.00");
	ss.str("");

	str = "12,2"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "12.20");
	ss.str("");

	str = "()"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "() + 4"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "4.00");
	ss.str("");

	str = "() * 4.01"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "() / 4.01"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "4.01 + ()"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "4.01");
	ss.str("");

	str = "4.01 + (()+())"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "4.01");
	ss.str("");

	str = "(11.02)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "11.02");
	ss.str("");

	str = "(12,21)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "12.21");
	ss.str("");

	str = "0 * 50"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "0 * 51.5"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "0 * 61,57"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "10 * 61,57"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "615.70");
	ss.str("");

	str = "0.1 * 61,57"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "6.16");
	ss.str("");

	str = "0 / 61,57"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "0.00");
	ss.str("");

	str = "(1 * 61,57) * 0.1"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "6.16");
	ss.str("");

	str = "(1 * 61,57) / 10 - 5.16"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "1.00");
	ss.str("");

	str = "(1 * 61,57) * 0.1 - 5.16 * 1 + 3.14"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "4.14");
	ss.str("");

	str = "(1 * 61,57) * 0.1 - 5.16 * (1 + 3.14)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-15.21");
	ss.str("");

	str = "((1 * 61,57) /10 - 5.16 * (1 + 3.14))"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-15.21");
	ss.str("");

	str = "((1 * 61,57) * (0.1) - (5.16) * ((1) + 3.14))"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-15.21");
	ss.str("");
	
	str = "-14,0"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-14.00");
	ss.str("");

	str = "(-14,0)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-14.00");
	ss.str("");

	str = "(0 +- 14,0)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-14.00");
	ss.str("");

	str = "(0 +          - 14,0)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-14.00");
	ss.str("");

	str = "(-13.0   - -14,0)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "1.00");
	ss.str("");

	str = "(-13.0   - (-14,0))"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "1.00");
	ss.str("");

	str = "(-13.0   - (-14,0) / - 1)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-27.00");
	ss.str("");

	str = "(-13.0   - (-14,0) / (1 - 1))"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	stringstream double_inf(stringstream::in | stringstream::out);
	double_inf << fixed;
	double_inf.precision(2);
	double_inf << numeric_limits<double>::max();
	ASSERT_EQUAL(ss.str(), double_inf.str());
	ss.str("");

	str = "(-13.0   - (-14,0) * - 1)"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "-27.00");
	ss.str("");

	str = "(-13.0   - (-14,0) * (- 1 /- 1))"s;
	e.SetExpression(str);
	ss << e.ToSolve();
	ASSERT_EQUAL(ss.str(), "1.00");
	ss.str("");
}

} // namespace tester_calculator::detail

void TestCalculator() {
	using namespace detail;
    RUN_TEST(TestCorrectOfExpression);
	cerr << "##########################################\n"s; 
	RUN_TEST(TestSolve);
	cerr << "##########################################\n"s; 
}

} // namespace tester_calculator;