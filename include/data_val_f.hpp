#pragma once

#include <vector>

#include <opencv2/core.hpp>

#include "types/ball_state.hpp"

using Ball_pos = cv::Matx21d;
using Balls_pos = std::vector<Ball_pos>;

namespace Data_val_f
{
    void circles_to_balls_pos(const Circles &circles, Balls_pos &balls_pos);
    void balls_pos_to_state(const Balls_pos &balls_pos, Ball_states &states);
    void run_k_filter(Balls_pos &balls_pos);

}
