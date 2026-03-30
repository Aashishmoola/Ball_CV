#pragma once

#include <vector>

// position and velocity of the ball in pixels, velocity is calc in the kalman filter itself
struct ball_state_t
{
    double x;
    double y;
    double x_v;
    double y_v;
};

using Ball_states = std::vector<ball_state_t>;