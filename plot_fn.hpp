#ifndef PLOT_FN_HPP
#define PLOT_FN_HPP
#include "detail/generic_svg_functionality.hpp"
#include <iomanip>
#include <cassert>
#include <vector>
#include <utility>
#include <fstream>

template<class F>
void plot_fn(F f, double a, double b, std::string const & title, std::string const & filename, unsigned samples = 100, int width = 1100)
{
    assert(b > a);
    int height = floor(double(width)/1.61803);

    std::vector<std::pair<double, double>> xys(samples);
    double step = (b-a)/samples;
    double min_val = std::numeric_limits<double>::max();
    double max_val = std::numeric_limits<double>::lowest();
    for (unsigned i =0; i < xys.size(); ++i)
    {
        double x = a + step*i;
        double y = f(x);
        if (y > max_val) {
          max_val = y;
        }
        if (y < min_val) {
          min_val = y;
        }
        xys[i] = {x, f(x)};
    }

    int margin_top = 35;
    int margin_left = 25;
    int margin_bottom = 20;
    int margin_right = 20;
    int graph_height = height - margin_bottom - margin_top;
    int graph_width = width - margin_left - margin_right;


    // Maps [a,b] to [0, graph_width]
    auto xScale = [&a, &b, &graph_width](double x)->double {
      return ((x-a)/(b-a))*static_cast<double>(graph_width);
    };

    // Maps [min_val, max_val] to [0, graph_height].
    // Note that in SVG, y increases downwards,
    // so yScale(min_val) = graph_height, yScale(max_val) = 0.
    auto yScale = [&max_val, &min_val, &graph_height](double y)-> double {
      return ((max_val - y)/(max_val - min_val) )*static_cast<double>(graph_height);
    };

    std::ofstream fs(filename);

    quicksvg::detail::write_prelude(fs, title, width, height);

    // Construct SVG group to simplify the calculations slightly:
    fs << "<g transform='translate(" << margin_left << ", " << margin_top << ")'>\n"
       // y-axis:
       << "<line x1='0' y1='0' x2='0' y2='" << graph_height
       << "' stroke='gray' stroke-width='1' />\n";
    // x-axis: If 0 is between the min a max height, place the axis at zero.
    // Otherwise, place is at the bottom of the graph.
    double x_axis_loc = graph_height;
    double x_axis_yval = min_val;
    if (min_val <= 0 && max_val >= 0) {
         x_axis_loc = yScale(0);
         x_axis_yval = 0;
    }
    fs << "<line x1='0' y1='" << x_axis_loc << "' x2='" << graph_width << "' y2='" << x_axis_loc
       << "' stroke='gray' stroke-width='1' />\n";

    quicksvg::detail::write_gridlines(fs, 8, 10, xScale, yScale, a, b, min_val, max_val,
                                      graph_width, graph_height, margin_left);
    fs  << "<path d='M" << xScale(xys[0].first) << " " << yScale(xys[0].second);
    for (size_t i = 1; i < xys.size(); ++i) {
        // Yes, this is linear interpolation.
        // I'm not gonna bother with Bezier curve control point computation.
        // SVG 2 will have Catmull-Rom, which will be miraculous compared to this nonsense.
        fs << " L" << xScale(xys[i].first) << " " << yScale(xys[i].second);
    }
    fs << "' stroke='steelblue' stroke-width='3' fill='none'></path>\n"
       << "</g>\n"
       << "</svg>\n";
    fs.close();
}
#endif
