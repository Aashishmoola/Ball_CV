#pragma once

#include <opencv2/core.hpp>

#include "types/ball_state.hpp"
#include "types/image_state.hpp"

namespace Trans
{
    // bool transform(cv::Mat& img);
    void printCircleCand(const Circles &);
    void drawCircles(const Circles &, cv::Mat &, double text_font_scale = 1.0);
    void findBallCand(cv::Mat &img, Circles &circles, BallDetCount &count);
}
