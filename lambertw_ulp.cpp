#include <cmath>
#include <iostream>
#include <boost/multiprecision/float128.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/lambert_w.hpp>
#include <boost/core/demangle.hpp>
#include "quicksvg/ulp_plot.hpp"

using boost::math::lambert_w0;
using boost::math::lambert_wm1;
using boost::math::lambert_w0_prime;
using boost::math::lambert_wm1_prime;
using boost::multiprecision::float128;
using boost::math::constants::exp_minus_one;

int main()
{
    using PreciseReal = float128;
    using CoarseReal = float;
    CoarseReal divider = -0.3667;
    int samples = 15000;
    CoarseReal a = -exp_minus_one<CoarseReal>();
    CoarseReal b = divider;
    std::string title = "ULP accuracy of " + boost::core::demangle(typeid(CoarseReal).name()) + " precision Lambert W₀ on (-1/e, -0.3667)";
    std::cout << title << "\n";
    //title = "";
    std::string filename = "examples/ulp_lambert_w0_1e_3667.svg";
    auto flo = [](CoarseReal x)->CoarseReal { return lambert_w0<CoarseReal>(x); };
    auto fhi = [](PreciseReal x)->PreciseReal { return lambert_w0<PreciseReal>(x); };

    int clip = 3;
    int horizontal_lines = 5;
    int vertical_lines = 5;
    auto ulp_plot = quicksvg::ulp_plot<decltype(fhi), PreciseReal, CoarseReal>(fhi, a, b, true, samples);
    ulp_plot.add_fn(flo);
    ulp_plot.write(filename, clip, true, title, 1100, horizontal_lines, vertical_lines);
    clip = 100;
    filename = "examples/ulp_lambert_w0_1e_3667_clip_" + std::to_string(clip) + ".svg";
    ulp_plot.write(filename, clip, true, title, 1100, horizontal_lines, vertical_lines);

}
