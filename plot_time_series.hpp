#ifndef PLOT_TIME_SERIES_HPP
#define PLOT_TIME_SERIES_HPP

#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <algorithm>

namespace quicksvg {

class plot_time_series
{
public:
    plot_time_series(double start_time, double time_step, std::string const & title,
                     std::string const & filename, int width = 1100) :
                    m_start_time{start_time},
                    m_end_time{std::numeric_limits<double>::lowest()},
                    m_time_step{time_step},
                    m_min_y{std::numeric_limits<double>::max()},
                    m_max_y{std::numeric_limits<double>::lowest()},
                    m_is_written{false}

    {
        // Golden ratio dude.
        int height = floor(double(width)/1.61803);
        m_fs.open(filename);

        m_margin_top = 35;
        m_margin_left = 25;
        m_margin_bottom = 20;
        m_margin_right = 20;
        m_graph_height = height - m_margin_bottom - m_margin_top;
        m_graph_width = width - m_margin_left - m_margin_right;


        detail::write_prelude(m_fs, title, width, height);


    }

    void add_dataset(std::vector<double> const & v, bool connect_the_dots = true,
                     std::string connect_color = "steelblue", std::string dot_color="orange")
    {
        auto result = std::minmax_element(v.begin(), v.end());
        if (*result.first < m_min_y)
        {
            m_min_y = *result.first;
        }
        if (*result.second > m_max_y)
        {
            m_max_y = *result.second;
        }

        double end_time = m_start_time + m_time_step*(v.size() - 1);
        if (end_time > m_end_time)
        {
            m_end_time = end_time;
        }
        m_connect.push_back(connect_the_dots);
        m_connect_color.push_back(connect_color);
        m_dot_color.push_back(dot_color);
        m_dataset.push_back(v);

    }

    void write_all()
    {
        if (m_is_written)
        {
            throw std::logic_error("Data is already written to the svg.\n");
        }
        // Maps [a,b] to [0, graph_width]
        auto x_scale = [this](double x)->double
        {
            return ((x-m_start_time)/(m_end_time- m_start_time))*static_cast<double>(m_graph_width);
        };

        auto y_scale = [this](double y)-> double
        {
          return ((m_max_y - y)/(m_max_y - m_min_y) )*static_cast<double>(m_graph_height);
        };

          // Construct SVG group to simplify the calculations slightly:
        m_fs << "<g transform='translate(" << m_margin_left << ", " << m_margin_top << ")'>\n";
             // y-axis:
        m_fs  << "<line x1='0' y1='0' x2='0' y2='" << m_graph_height
              << "' stroke='gray' stroke-width='1' />\n";
        // x-axis: If 0 is between the min a max height, place the axis at zero.
        // Otherwise, place is at the bottom of the graph.
        double x_axis_loc = m_graph_height;
        if (m_min_y <= 0 && m_max_y >= 0)
        {
            x_axis_loc = y_scale(0);
        }
        m_fs << "<line x1='0' y1='" << x_axis_loc
             << "' x2='" << m_graph_width << "' y2='" << x_axis_loc
             << "' stroke='gray' stroke-width='1' />\n";

        detail::write_gridlines(m_fs, 8, 10, x_scale, y_scale, m_start_time, m_end_time,
                                m_min_y, m_max_y, m_graph_width, m_graph_height, m_margin_left);


        for (size_t i = 0; i < m_connect.size(); ++i)
        {
            bool connect_the_dots = m_connect[i];
            auto const & v = m_dataset[i];
            std::string const & stroke = m_connect_color[i];
            std::string const & dot_color = m_dot_color[i];
            if(connect_the_dots)
            {
                m_fs << "<path d='M" << x_scale(m_start_time) << " " << y_scale(v[0]);
                for (size_t j = 1; j < v.size(); ++j)
                {
                    double t = x_scale(m_start_time + j*m_time_step);
                    m_fs << " L" << t << " " << y_scale(v[j]);
                }
                m_fs << "' stroke='" << stroke << "' stroke-width='3' fill='none'></path>\n";
            }

            for (size_t j = 0; j < v.size(); ++j)
            {
                double t = x_scale(m_start_time + j*m_time_step);
                m_fs << "<circle cx='" << t << "' cy='" << y_scale(v[j])
                     << "' r='2' fill='" << dot_color << "' />\n";
            }
        }

        m_fs << "</g>\n"
           << "</svg>\n";
        m_fs.close();

        m_is_written = true;

    };

    ~plot_time_series()
    {
        if (!m_is_written)
        {
            std::cerr << "Warning: You did not write your data to disk!\n";
        }
    }

private:
    std::ofstream m_fs;
    double m_start_time;
    double m_end_time;
    double m_time_step;
    double m_min_y;
    double m_max_y;
    bool m_is_written;
    std::vector<bool> m_connect;
    std::vector<std::vector<double>> m_dataset;
    std::vector<std::string> m_connect_color;
    std::vector<std::string> m_dot_color;
    int m_margin_top;
    int m_margin_left;
    int m_margin_bottom;
    int m_margin_right;
    int m_graph_width;
    int m_graph_height;
};

} // namespace

#endif
