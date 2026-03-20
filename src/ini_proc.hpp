#pragma once

#include <opencv2/imgproc.hpp>

namespace Img_P {
    // Declared types
    typedef std::vector<cv::Vec3f> Circle;

    // Declared constants
    const int THRESHOLD_VAL{148};

    cv::Mat convert_grayscale(cv::Mat&);
    cv::Mat apply_blur(cv::Mat&);
    cv::Mat apply_threshold(cv::Mat&);
    void print_circle_cand(const Img_P::Circle&);
    void draw_circles(const Img_P::Circle&, cv::Mat&, double text_font_scale = 1.0);
}
