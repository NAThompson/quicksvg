#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
#include "quicksvg/graph_fn.hpp"
#include "gtest/gtest.h"

using boost::math::constants::pi;
using boost::multiprecision::cpp_bin_float_50;

TEST(graph_fn, types) {
    {
        float a = -pi<float>();
        float b = pi<float>();
        std::string title = "sin(𝑥) and cos(𝑥)";
        std::string filename = "examples/sine_and_cosine_float.svg";
        quicksvg::graph_fn sin_graph(a, b, title, filename);
        auto f = [](float x)->float { return std::sin(x); };
        auto g = [](float x)->float { return std::cos(x); };
        sin_graph.add_fn(f);
        sin_graph.add_fn(g, "green");

        sin_graph.write_all();
    }

    {
        double a = -pi<double>();
        double b = pi<double>();
        std::string title = "sin(𝑥) and cos(𝑥)";
        std::string filename = "examples/sine_and_cosine_double.svg";
        auto f = [](double x)->double { return std::sin(x); };
        auto g = [](double x)->double { return std::cos(x); };
        quicksvg::graph_fn sin_graph(a, b, title, filename);

        sin_graph.add_fn(f);
        sin_graph.add_fn(g, "green");

        sin_graph.write_all();
    }

    {
        long double a = -pi<long double>();
        long double b = pi<long double>();
        std::string title = "sin(𝑥) and cos(𝑥)";
        std::string filename = "examples/sine_and_cosine_long_double.svg";
        auto f = [](long double x)->long double { return std::sin(x); };
        auto g = [](long double x)->long double { return std::cos(x); };
        quicksvg::graph_fn sin_graph(a, b, title, filename);

        sin_graph.add_fn(f);
        sin_graph.add_fn(g, "green");
        sin_graph.write_all();
    }

    {
        cpp_bin_float_50 a = -pi<cpp_bin_float_50>();
        cpp_bin_float_50 b = pi<cpp_bin_float_50>();
        std::string title = "sin(𝑥) and cos(𝑥)";
        std::string filename = "examples/sine_and_cosine_cpp_bin_float_50.svg";
        quicksvg::graph_fn sin_graph(a, b, title, filename);
        auto f = [](cpp_bin_float_50 x)->cpp_bin_float_50 { return sin(x); };
        auto g = [](cpp_bin_float_50 x)->cpp_bin_float_50 { return cos(x); };
        sin_graph.add_fn(f);
        sin_graph.add_fn(g, "green");

        sin_graph.write_all();
    }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
