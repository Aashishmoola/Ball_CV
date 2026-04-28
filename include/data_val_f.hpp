#pragma once

#include <vector>

#include <opencv2/core.hpp>

#include "types/ball_state.hpp"

using BallPos = cv::Matx21d;
using BallsPos = std::vector<BallPos>;

namespace DataValF
{
    void circlesToBallsPos(const Circles &circles, BallsPos &balls_pos);
    void ballsPosToState(const BallsPos &balls_pos, BallStates &states);
    void runKFilter(BallsPos &balls_pos);

}
