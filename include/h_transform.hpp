#pragma once

#include <opencv2/core.hpp>

#include "types/ball_state.hpp"
#include "types/image_state.hpp"

namespace Trans
{
    // bool transform(cv::Mat& img);
    void print_circle_cand(const Circles &);
    void draw_circles(const Circles &, cv::Mat &, double text_font_scale = 1.0);
    void find_ball_cand(cv::Mat &img, Circles &circles, ball_det_count_t &count);
}