#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>
#include <stack>
#include <optional>
#include <map>
#include <iostream>
#include <algorithm>
#include <limits>

#include "expression.h"


using namespace std;

namespace expression {

namespace detail {

bool IsOperation(const char c) {
    static const vector<char> operations{'+', '-', '*', '/'};
    for (const char o : operations) {
        if (c == o) {
            return true;
        }
    }

    return false;
}

Operation WhatIsOperation(const char c) {
    switch (c) {
    case '+': return Operation::SUM;
    case '-': return Operation::MINUS;
    case '*': return Operation::MULTYPLY;
    case '/': return Operation::DIVIDE;
    }
    return Operation::NONE;
}

bool IsBracketSequenceValid(const string& exp) {
    int balance = 0;
    for (const auto c : exp) {
        if (c == '(' || c == ')') {
            if (c == '(') {
                balance++;
            } else {
                balance--;
            }
        }
        if (balance < 0) {
            return false;
        }
    }
    return balance == 0;
}

bool IsNotHasExtraCharacters(const string& exp) {
    for (const auto c : exp) {
        if (detail::IsOperation(c) == false && isdigit(c) == false && 
            c != '.' && c != ',' && c != '(' && c != ')'
        ) {
            return false;
        }
    }
    return true;
}

class MapCompare {
public: 
    bool operator() (const Operation& lhs, const Operation& rhs) const {
        return lhs > rhs;
    }
};

double ToSolveMiniExp(const string& exp) {
    // Parse string for numbers and operations
    vector<double> numbers;
    multimap<Operation, pair<size_t, size_t>, MapCompare> operation_to_numbers_pos;
    auto add_all = 
        [&numbers, &operation_to_numbers_pos](string& num_str, const char c) {
            numbers.emplace_back(stod(num_str));
            operation_to_numbers_pos.emplace(
                WhatIsOperation(c), 
                move(pair<size_t, size_t>{numbers.size() - 1, numbers.size()})
            );
            num_str = ""s;
        };
    string num_str;
    for (size_t i = 0; i < exp.size(); i++) {
        const auto c = exp[i];
        if (isdigit(c) || c == '.' || c == ',') {
            num_str += c;
        } else if (c == '-') {
            if (i == 0 || IsOperation(exp[i - 1])) {
                num_str += c;
            } else {
                add_all(num_str, c);
            }
        } else {
            add_all(num_str, c);
        }
    }
    if (num_str.empty() == false) {
        numbers.emplace_back(move(stod(num_str)));
    }

    map<size_t, size_t> pnum_to_pnum;
    for (size_t i = 0; i < numbers.size(); ++i) {
        pnum_to_pnum[i] = i;
    }

    // Solve the expression
    size_t last_num = 0;
    for (const auto& [op, nums] : operation_to_numbers_pos) {
        double tmp = 0;
        switch (op) {
        case Operation::SUM : 
            tmp = numbers[nums.first] + numbers[nums.second];
            break;
        case Operation::MINUS : 
            tmp = numbers[nums.first] - numbers[nums.second];
            break;
        case Operation::MULTYPLY : 
            tmp = numbers[nums.first] * numbers[nums.second];
            break;
        case Operation::DIVIDE :
            if (numbers[nums.second] == 0.) {
                return numeric_limits<double>::max();
            }
            tmp = numbers[nums.first] / numbers[nums.second];
            break;
        default:
            break;
        }
        vector<size_t> poses{
            nums.first, nums.second, 
            pnum_to_pnum[nums.first], pnum_to_pnum[nums.second]};
        sort(poses.begin(), poses.end());
        pnum_to_pnum[poses.front()] = poses.back();
        pnum_to_pnum[poses.back()] = poses.front();
        numbers[poses.front()] = numbers[poses.back()] = tmp;
    }
    if (operation_to_numbers_pos.empty() == false) {
        return numbers[last_num];
    } else if (numbers.empty() == false) {
        return numbers.front();
    }

    return 0;
}

} // namespace expression::detail

Expression::Expression(const std::string& str) : exp_(str) {
    PrepExpression();
}

Expression::Expression(std::string&& str) : exp_(move(str)) {
    PrepExpression();
}

void Expression::SetExpression(const std::string& str) {
    exp_ = str;
    PrepExpression();
}

void Expression::SetExpression(std::string&& str) {
    exp_ = move(str);
    PrepExpression();
}

pair<bool, const char*> Expression::IsCorrect() const noexcept {
    if (detail::IsBracketSequenceValid(exp_) == false) {
        return {false, "bracket sequence is not valid"};
    }
    if (detail::IsNotHasExtraCharacters(exp_) == false) {
        return {false, "expression has extra characters"};
    }
    return {true, "The expression is valid"};
}

double Expression::ToSolve() const {
    string exp(exp_);
    stack<size_t> open_bracket_poses;
    for (size_t i = 0; i < exp.size(); ) {
        const auto c = exp[i];
        if (c == '(') {
            open_bracket_poses.push(i++);
        } else if (c == ')') {
            size_t 
                start_pos = open_bracket_poses.top() + 1,
                end_pos = i - 1,
                length = end_pos - start_pos + 1;
            open_bracket_poses.pop();
            string mini_exp(move(exp.substr(start_pos, length)));
            string mini_exp_ans(move(
                to_string(detail::ToSolveMiniExp(mini_exp))));
            exp = move(
                exp.substr(0, start_pos - 1) + 
                mini_exp_ans + 
                exp.substr(end_pos + 2)
            );
            i = start_pos - 1 + mini_exp_ans.size();
        } else {
            i++;
        }
    }
    double ans = detail::ToSolveMiniExp(exp);
    return ans;
}

void Expression::PrepExpression() noexcept {
    // Remove spaces and replace with '.'
    size_t pos = 0;
    for (auto c : exp_) {
        if (c == ',') {
            c = '.';
        }
        if (c != ' ') {
            exp_[pos++] = c;
        }
    }
    exp_.erase(exp_.begin() + pos, exp_.end());
}

} // namespace expression