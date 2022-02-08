#include <iostream>
#include <optional>
#include <string>

#include "tester_calculator.h"
#include "expression.h"

using namespace std;

int main() {
    tester_calculator::TestCalculator();
    string input_s;
    getline(cin, input_s);
    expression::Expression exp(input_s);
    const auto is_correct = exp.IsCorrect();
    if (is_correct.first == false) {
        cerr << "Expression is incorrect (" <<  is_correct.second << ")" << endl;
    } else {
        cout << fixed;
        cout.precision(2);
        cout << exp.ToSolve() << endl;
    }

    return 0;
}
