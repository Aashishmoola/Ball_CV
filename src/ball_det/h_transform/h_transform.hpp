#pragma once

#include <opencv2/imgproc.hpp>
#include <iostream>
#include <vector>

namespace H_trans {
    typedef std::vector<cv::Vec3f> Circle;

    cv::Mat transform(cv::Mat& img);
    void print_circle_cand(const Circle&);
    void draw_circles(const Circle&, cv::Mat&, double text_font_scale = 1.0);
}