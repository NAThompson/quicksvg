#ifndef QUICKSVG_ULP_PLOT_HPP
#define QUICKSVG_ULP_PLOT_HPP
#include "detail/generic_svg_functionality.hpp"
#include <boost/multiprecision/float128.hpp>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <random>

// Design of this function comes from:
// https://blogs.mathworks.com/cleve/2017/01/23/ulps-plots-reveal-math-function-accurary/

namespace quicksvg {

template<class F1, class F2, class Real=double>
void ulp_plot(F1 f_lo_accuracy, F2 f_hi_accuracy, Real min_x, Real max_x,
              std::string const & title,
              std::string const & filename,
              size_t samples = 10000, int width = 1100)
{
    using boost::multiprecision::float128;
    std::ofstream fs;
    fs.open(filename);
    assert(max_x > min_x);
    int height = floor(double(width)/1.61803);

    int margin_top = 40;
    int margin_left = 25;
    int margin_bottom = 20;
    int margin_right = 20;
    int graph_height = height - margin_bottom - margin_top;
    int graph_width = width - margin_left - margin_right;

    detail::write_prelude(fs, title, width, height, margin_top);


    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<Real> dis(min_x, max_x);

    std::vector<float128> ulp(samples);
    std::vector<Real> abscissas(samples);

    float128 min_y = std::numeric_limits<Real>::max();
    float128 max_y = std::numeric_limits<Real>::lowest();
    for(size_t i = 0; i < samples; ++i)
    {
        abscissas[i] = dis(gen);
    }
    // Unnecessary since we're not connecting any dots:
    //std::sort(abscissas.begin(), abscissas.end());

    Real worst_abscissa = 0;
    float128 worst_ulp_dist = 0;
    for(size_t i = 0; i < samples; ++i)
    {
        Real x = abscissas[i];
        float128 y_lo_ac = static_cast<float128>(f_lo_accuracy(x));
        float128 y_hi_ac = f_hi_accuracy(static_cast<float128>(x));
        float128 ay = abs(y_hi_ac);

        float128 dist = nextafter(static_cast<Real>(ay), std::numeric_limits<Real>::max()) - static_cast<Real>(ay);

        float128 ulp_dist = (y_lo_ac - y_hi_ac)/dist;

        ulp[i] = ulp_dist;

        if (ulp_dist < min_y)
        {
            min_y = ulp_dist;
        }
        if (ulp_dist > max_y)
        {
            max_y = ulp_dist;
        }
        if(abs(ulp_dist) > worst_ulp_dist) {
          worst_ulp_dist = abs(ulp_dist);
          worst_abscissa = x;
        }
    }

    std::cout << std::setprecision(std::numeric_limits<Real>::digits10 + 3);
    std::cout << "The highest-error abscissa on the interval [" << min_x << ", " << max_x << "] is " << worst_abscissa << ", having ULP distance from true value of " << worst_ulp_dist << ".\n";
    std::cout << "The true value is " << f_hi_accuracy(static_cast<float128>(worst_abscissa)) << ", but was calculated to be " << f_lo_accuracy(worst_abscissa) << ".\n";

    // Maps [a,b] to [0, graph_width]
    auto x_scale = [&](Real x)->Real
    {
        return ((x-min_x)/(max_x - min_x))*static_cast<Real>(graph_width);
    };

    auto y_scale = [&](float128 y)->float128
    {
      return ((max_y - y)/(max_y - min_y) )*static_cast<Real>(graph_height);
    };

      // Construct SVG group to simplify the calculations slightly:
    fs << "<g transform='translate(" << margin_left << ", " << margin_top << ")'>\n";
         // y-axis:
    fs  << "<line x1='0' y1='0' x2='0' y2='" << graph_height
          << "' stroke='gray' stroke-width='1' />\n";
    Real x_axis_loc = y_scale(0);
    fs << "<line x1='0' y1='" << x_axis_loc
         << "' x2='" << graph_width << "' y2='" << x_axis_loc
         << "' stroke='gray' stroke-width='1' />\n";

    detail::write_gridlines(fs, 8, 10, x_scale, y_scale, min_x, max_x,
                            static_cast<double>(min_y), static_cast<double>(max_y), graph_width, graph_height, margin_left);


    for (size_t j = 0; j < samples; ++j)
    {
        Real x = x_scale(abscissas[j]);
        Real y = y_scale(ulp[j]);

        fs << "<circle cx='" << x << "' cy='" << y
             << "' r='1' fill='steelblue' />\n";
    }

    fs << "</g>\n"
       << "</svg>\n";
    fs.close();

}

} // namespace quicksvg
#endif
