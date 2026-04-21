#include "radar.hpp"

double Radar::get_Z_cam(double Z_r, double x_offset, double y_offset)
{
    return std::sqrt((Z_r * Z_r - x_offset * x_offset - y_offset * y_offset));
}
