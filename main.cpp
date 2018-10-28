#include <cmath>
#include <iostream>
#include <boost/math/special_functions/hypergeometric_1F1.hpp>
#include <boost/math/special_functions/hypergeometric_2F0.hpp>
#include "graph_fn.hpp"
#include "plot_time_series.hpp"


using boost::math::hypergeometric_1F1;
using boost::math::hypergeometric_2F0;

int main() {

    double a = -3.14159;
    double b = 3.14159;
    std::string title = "sin(𝑥) and cos(𝑥)";
    std::string filename = "examples/sine_and_cosine.svg";
    auto f = [](double x)->double { return std::sin(x); };
    auto g = [](double x)->double { return std::cos(x); };
    quicksvg::graph_fn sin_graph(a, b, title, filename);

    sin_graph.add_fn(f);
    sin_graph.add_fn(g, "green");

    sin_graph.write_all();

    auto onef1_1 = [](double x)->double { return hypergeometric_1F1(3, 7, x); };
    a = -3;
    b = 2;
    title = "\u2081F\u2081(3, 7, 𝑥)";
    filename = "examples/1F1_1.svg";
    quicksvg::graph_fn onef1_1graph(a, b, title, filename);
    onef1_1graph.add_fn(onef1_1);
    onef1_1graph.write_all();

    auto onef1_2 = [](double x)->double { return hypergeometric_1F1(-2, 3, x); };
    a = -3;
    b = 2;
    title = "\u2081F\u2081(-2, 3, 𝑥)";
    filename = "examples/1F1_2.svg";
    quicksvg::graph_fn onef1_2graph(a, b, title, filename);
    onef1_2graph.add_fn(onef1_2);
    onef1_2graph.write_all();

    auto onef1_3 = [](double x)->double { return hypergeometric_1F1(2, -2.5, x); };
    a = -2;
    b = 1;
    title = "\u2081F\u2081(2, -2.5, 𝑥)";
    filename = "examples/1F1_3.svg";

    quicksvg::graph_fn onef1_3graph(a, b, title, filename);
    onef1_3graph.add_fn(onef1_3);
    onef1_3graph.write_all();

    auto onef1_4 = [](double x)->double { return hypergeometric_1F1(-2, -2.5, x); };
    a = -4;
    b = 1;
    title = "\u2081F\u2081(-2, -2.5, 𝑥)";
    filename = "examples/1F1_4.svg";
    quicksvg::graph_fn onef1_4graph(a, b, title, filename);
    onef1_4graph.add_fn(onef1_4);
    onef1_4graph.write_all();



    std::vector<double> v(50);
    std::vector<double> u(50);
    double start_time = 0;
    double time_step = 0.25;
    for (size_t i = 0; i < v.size(); ++i) {
      v[i] = std::sin(start_time + i*time_step);
      u[i] = std::cos(start_time + i*time_step);
    }

    title = "sine and cosine time series";
    filename = "examples/sin_cos_time_series.svg";

    quicksvg::plot_time_series pts(start_time, time_step, title, filename);
    pts.add_dataset(v);
    pts.add_dataset(u, false, "lime", "lightgreen");
    pts.write_all();

}
