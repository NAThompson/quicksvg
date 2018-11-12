#include <cmath>
#include <iostream>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/sign.hpp>
#include "quicksvg/graph_fn.hpp"
#include "quicksvg/plot_time_series.hpp"


using boost::math::constants::pi;
using boost::math::sign;

template<class Real>
Real blocks(Real x)
{
  static constexpr const std::array<double, 11> t{0.1, 0.13, 0.15, 0.23, 0.25, 0.40, 0.44, 0.65, 0.76, 0.78, 0.81};
  static constexpr const std::array<double, 11> h{4.0, -5.0, 3.0, -4.0, 5.0, -4.2, 2.1, 4.3, -3.1, 5.1, -4.2};

  double f = 0;
  for (size_t i = 0; i < 11; ++i) {
    f += h[i]*(1+sign(x-t[i]))/2;
  }

  return f;
}

template<class Real>
Real bumps(Real x)
{
  static constexpr const std::array<Real, 11> t{0.1, 0.13, 0.15, 0.23, 0.25, 0.40, 0.44, 0.65, 0.76, 0.78, 0.81};
  static constexpr const std::array<Real, 11> h{4.0, 5.0, 3.0, 4.0, 5.0, 4.2, 2.1, 4.3, 3.1, 5.1, 4.2};
  static constexpr const std::array<Real, 11> w{0.005, 0.005, 0.006, 0.01, 0.01, 0.03, 0.01, 0.01, 0.005, 0.008, 0.005};

  using std::pow;
  auto k = [](Real t)->Real { return 1.0/(1.0 + t*t*t*t); };

  Real f = 0;
  for (size_t i = 0; i < 11; ++i) {
    f += h[i]*k((x-t[i])/w[i]);
  }
  return f;
}

template<class Real>
Real heavisine(Real t)
{
  using std::sin;
  using boost::math::constants::pi;

  return 4*sin(4*pi<Real>()*t) - sign(t - 0.3)  - sign(0.72 - t);
}

template<class Real>
Real doppler(Real t)
{
    using std::sqrt;
    Real eps = 0.05;

    return sqrt(t*(1-t))*sin(2*pi<Real>()*(1+eps)/(t+eps));
}

int main()
{
    double a = 0;
    double b = 1;
    std::string title = "blocks";
    std::string filename = "examples/blocks.svg";
    quicksvg::graph_fn blck(a, b, title, filename, /* samples = */ 2048);
    blck.add_fn(blocks<double>);
    blck.write_all();

    title = "bumps";
    filename = "examples/bumps.svg";

    quicksvg::graph_fn bmps(a, b, title, filename, 2048);
    bmps.add_fn(bumps<double>);
    bmps.write_all();

    title = "heavisine";
    filename = "examples/heavisine.svg";
    quicksvg::graph_fn hsin(a, b, title, filename, 2048);
    hsin.add_fn(heavisine<double>);
    hsin.write_all();

    title = "doppler";
    filename = "examples/doppler.svg";
    quicksvg::graph_fn dop(a, b, title, filename, 2048);
    dop.add_fn(doppler<double>);
    dop.write_all();
}
