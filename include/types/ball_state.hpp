#pragma once

#include <opencv2/core.hpp>
#include <vector>

// position and velocity of the ball in pixels, velocity is calc in the kalman filter itself
struct ball_state_t
{
    double x;
    double y;
    double x_v;
    double y_v;
};

struct ball_disp_t
{
    double x;
    double y;
};

using Ball_states = std::vector<ball_state_t>;

using Contour = std::vector<cv::Point>;
using Contours = std::vector<Contour>;

using Circle = cv::Vec3f;
using Circles = std::vector<Circle>;
