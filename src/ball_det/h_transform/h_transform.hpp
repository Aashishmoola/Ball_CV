#pragma once

#include "../../helpers/ball_state.hpp"

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

namespace Trans {
    // bool transform(cv::Mat& img);
    void print_circle_cand(const Circles&);
    void draw_circles(const Circles&, cv::Mat&, double text_font_scale = 1.0);
    void find_ball_cand(cv::Mat& img, Circles& circles);
}