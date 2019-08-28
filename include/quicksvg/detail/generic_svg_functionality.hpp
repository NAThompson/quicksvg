#ifndef QUICKSVG_DETAIL_GENERIC_SVG_FUNCTIONALITY
#define QUICKSVG_DETAIL_GENERIC_SVG_FUNCTIONALITY

#include <iomanip>
#include <fstream>
#include <cmath>

namespace quicksvg { namespace detail {

void write_prelude(std::ofstream& fs, std::string const & title, int width, int height, int margin_top)
{
    using std::floor;
    fs << "<?xml version=\"1.0\" encoding='UTF-8' ?>\n"
       << "<svg xmlns='http://www.w3.org/2000/svg' width='"
       << width << "' height='"
       << height << "'>\n"
       // Black background; I don't want to go blind:
       << "<style>svg { background-color: black; }\n"
       << "</style>\n"
       // Title:
       << "<text x='" << floor(width/2)
       << "' y='" << floor(margin_top/2)
       << "' font-family='Palatino' font-size='25' fill='white'  alignment-baseline='middle' text-anchor='middle'>"
       << title
       << "</text>\n";
}

void write_xlabel(std::ofstream& fs, std::string const & x_label, int width, int height, int margin_bottom)
{
    using std::floor;
    fs << "<text x='" << floor(width/2)
       << "' y='" << floor(height - margin_bottom/4)
       << "' font-family='Palatino' font-size='15' fill='white'  alignment-baseline='middle' text-anchor='middle'>"
       << x_label
       << "</text>\n";
}

void write_ylabel(std::ofstream& fs, std::string const & y_label, int width, int height, int margin_left)
{
    using std::floor;
    fs << "<text x='0' y='0' font-family='Palatino' font-size='15' fill='white' alignment-baseline='middle' text-anchor='middle' transform='translate("
       << margin_left/4 << ", " << height/2 << ")rotate(-90)'>"
       << y_label
       << "</text>\n";
}


template<class F1, class F2, class Real>
void write_gridlines(std::ofstream& fs, int horizontal_lines, int vertical_lines,
                     F1 x_scale, F2 y_scale, Real min_x, Real max_x, Real min_y, Real max_y,
                     int graph_width, int graph_height, int margin_left)
{
  // Make a grid:
  for (int i = 1; i <= horizontal_lines; ++i) {
      Real y_cord_dataspace = min_y +  ((max_y - min_y)*i)/horizontal_lines;
      auto y = y_scale(y_cord_dataspace);
      fs << "<line x1='0' y1='" << y << "' x2='" << graph_width
         << "' y2='" << y
         << "' stroke='gray' stroke-width='1' opacity='0.5' stroke-dasharray='4' />\n";

      fs << "<text x='" <<  -margin_left/4 + 5 << "' y='" << y - 3
         << "' font-family='times' font-size='10' fill='white' transform='rotate(-90 "
         << -margin_left/4 + 11 << " " << y + 5 << ")'>"
         << std::setprecision(4) << y_cord_dataspace << "</text>\n";
   }

   for (int i = 1; i <= vertical_lines; ++i) {
       Real x_cord_dataspace = min_x +  ((max_x - min_x)*i)/vertical_lines;
       Real x = x_scale(x_cord_dataspace);
       fs << "<line x1='" << x << "' y1='0' x2='" << x
          << "' y2='" << graph_height
          << "' stroke='gray' stroke-width='1' opacity='0.5' stroke-dasharray='4' />\n";

        fs << "<text x='" <<  x - 10  << "' y='" << graph_height + 10
             << "' font-family='times' font-size='10' fill='white'>"
             << std::setprecision(4) << x_cord_dataspace << "</text>\n";
    }
}

}}
#endif
