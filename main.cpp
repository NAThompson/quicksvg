#include <cmath>
#include <iostream>
#include "plot_fn.hpp"
#include <boost/math/special_functions/hypergeometric_1F1.hpp>
#include <boost/math/special_functions/hypergeometric_2F0.hpp>
#include <boost/math/special_functions/trunc.hpp>
#include <boost/math/special_functions/detail/round_fwd.hpp>

using boost::math::hypergeometric_1F1;
using boost::math::hypergeometric_2F0;

int main() {

    double a = -3;
    double b = 2;
    std::string title = "sin(x)";
    std::string filename = "sine.svg";
    auto f = [](double x)->double { return std::sin(x); };
    plot_fn(f, a, b, title, filename);

    auto onef1_1 = [](double x)->double { return hypergeometric_1F1(3, 7, x); };
    a = -3;
    b = 2;
    title = "\u2081F\u2081(3, 7, x)";
    filename = "1F1_1.svg";
    plot_fn(onef1_1, a, b, title, filename);

    auto onef1_2 = [](double x)->double { return hypergeometric_1F1(-2, 3, x); };
    a = -3;
    b = 2;
    title = "\u2081F\u2081(-2, 3, x)";
    filename = "1F1_2.svg";
    plot_fn(onef1_2, a, b, title, filename);


    auto onef1_3 = [](double x)->double { return hypergeometric_1F1(2, -2.5, x); };
    a = 0;
    b = 3;
    title = "\u2081F\u2081(2, -2.5, x)";
    filename = "1F1_3.svg";
    plot_fn(onef1_3, a, b, title, filename);


    auto onef1_4 = [](double x)->double { return hypergeometric_1F1(-2, -2.5, x); };
    a = -3;
    b = 2;
    title = "\u2081F\u2081(-2, -2.5, x)";
    filename = "1F1_4.svg";
    plot_fn(onef1_4, a, b, title, filename);

}
