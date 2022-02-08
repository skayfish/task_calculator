#pragma once

#include <string>
#include <utility>

namespace expression {

enum Operation {
    SUM,
    MINUS,
    MULTYPLY,
    DIVIDE,
    NONE
};

class Expression {
public:
    Expression() = default;
    Expression(const std::string& str);
    Expression(std::string&& str);

    void SetExpression(const std::string& str);
    void SetExpression(std::string&& str);
    std::pair<bool, const char*> IsCorrect() const noexcept;
    double ToSolve() const;

private:
    void PrepExpression() noexcept;

    std::string exp_;
};

}