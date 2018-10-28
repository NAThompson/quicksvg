#ifndef PLOT_FN_HPP
#define PLOT_FN_HPP

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
    std::ofstream fs(filename);

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

    fs << "<?xml version=\"1.0\" encoding='UTF-8' ?>\n"
       << "<svg xmlns='http://www.w3.org/2000/svg' version='1.1' width='"
       << width << "' height='"
       << height << "'>\n"
       // Black background; I don't want to go blind:
       << "<style>svg { background-color: black; }\n"
       << "</style>\n"
       // Title:
       << "<text x='" << floor((width - title.length())/2)
       << "' y='25' font-family='times' font-size='25' fill='white'>"
       << title
       << "</text>\n"


       // Construct SVG group to simplify the calculations slightly:
       << "<g transform='translate(" << margin_left << ", " << margin_top << ")'>\n"
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

    /*// Label the y-coordinate of the x_axis:
    fs << "<text x='" <<  -margin_left/2 + 3 << "' y='" << x_axis_loc - 3
       << "' font-family='times' font-size='10' fill='white' transform='rotate(-90 "
       << -margin_left/2 + 11 << " " << x_axis_loc + 5 << ")'>"
       <<  std::fixed << std::setprecision(2) << x_axis_yval << "</text>\n";*/

    // Make a grid:
    int horizontal_lines = 8;
    for (int i = 1; i <= horizontal_lines; ++i) {
        double y_cord_dataspace = min_val +  ((max_val - min_val)*i)/horizontal_lines;
        double y = yScale(y_cord_dataspace);
        fs << "<line x1='0' y1='" << y << "' x2='" << graph_width
           << "' y2='" << y
           << "' stroke='gray' stroke-width='1' opacity='0.5' stroke-dasharray='4' />\n";

        fs << "<text x='" <<  -margin_left/2 + 5 << "' y='" << y - 3
           << "' font-family='times' font-size='10' fill='white' transform='rotate(-90 "
           << -margin_left/2 + 11 << " " << y + 5 << ")'>"
           <<  std::fixed << std::setprecision(2) << y_cord_dataspace << "</text>\n";
     }

     int vertical_lines = 10;
     for (int i = 1; i <= vertical_lines; ++i) {
         double x_cord_dataspace = a +  ((b - a)*i)/vertical_lines;
         double x = xScale(x_cord_dataspace);
         fs << "<line x1='" << x << "' y1='0' x2='" << x
            << "' y2='" << graph_height
            << "' stroke='gray' stroke-width='1' opacity='0.5' stroke-dasharray='4' />\n";

          fs << "<text x='" <<  x - 10  << "' y='" << graph_height + 10
               << "' font-family='times' font-size='10' fill='white'>"
               <<  std::fixed << std::setprecision(2) << x_cord_dataspace << "</text>\n";

      }


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
