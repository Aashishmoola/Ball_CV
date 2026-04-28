#pragma once

#include <vector>

#include <opencv2/core.hpp>

// position and velocity of the ball in pixels, velocity is calc in the kalman filter itself
struct BallState
{
    double x;
    double y;
    double x_v;
    double y_v;
};

struct BallDisp
{
    double x;
    double y;
};

using BallStates = std::vector<BallState>;

using Contour = std::vector<cv::Point>;
using Contours = std::vector<Contour>;

using Circle = cv::Vec3f;
using Circles = std::vector<Circle>;
