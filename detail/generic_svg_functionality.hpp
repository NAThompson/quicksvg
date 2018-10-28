#ifndef QUICKSVG_DETAIL_GENERIC_SVG_FUNCTIONALITY
#define QUICKSVG_DETAIL_GENERIC_SVG_FUNCTIONALITY

#include <iomanip>
#include <fstream>

namespace quicksvg { namespace detail {

void write_prelude(std::ofstream& fs, std::string title, int width, int height, int margin_top)
{
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

template<class F1, class F2>
void write_gridlines(std::ofstream& fs, int horizontal_lines, int vertical_lines,
                     F1 x_scale, F2 y_scale, double min_x, double max_x, double min_y, double max_y,
                     int graph_width, int graph_height, int margin_left)
{
  // Make a grid:
  for (int i = 1; i <= horizontal_lines; ++i) {
      double y_cord_dataspace = min_y +  ((max_y - min_y)*i)/horizontal_lines;
      double y = y_scale(y_cord_dataspace);
      fs << "<line x1='0' y1='" << y << "' x2='" << graph_width
         << "' y2='" << y
         << "' stroke='gray' stroke-width='1' opacity='0.5' stroke-dasharray='4' />\n";

      fs << "<text x='" <<  -margin_left/2 + 5 << "' y='" << y - 3
         << "' font-family='times' font-size='10' fill='white' transform='rotate(-90 "
         << -margin_left/2 + 11 << " " << y + 5 << ")'>"
         <<  std::fixed << std::setprecision(2) << y_cord_dataspace << "</text>\n";
   }

   for (int i = 1; i <= vertical_lines; ++i) {
       double x_cord_dataspace = min_x +  ((max_x - min_x)*i)/vertical_lines;
       double x = x_scale(x_cord_dataspace);
       fs << "<line x1='" << x << "' y1='0' x2='" << x
          << "' y2='" << graph_height
          << "' stroke='gray' stroke-width='1' opacity='0.5' stroke-dasharray='4' />\n";

        fs << "<text x='" <<  x - 10  << "' y='" << graph_height + 10
             << "' font-family='times' font-size='10' fill='white'>"
             <<  std::fixed << std::setprecision(2) << x_cord_dataspace << "</text>\n";
    }
}

}}
#endif
