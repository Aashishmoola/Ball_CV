#pragma once

#include <opencv2/imgproc.hpp>

namespace Img_P {
    // Declared constants
    const int THRESHOLD_VAL{148};

    cv::Mat convert_grayscale(cv::Mat&);
    cv::Mat apply_blur(cv::Mat&);
    cv::Mat apply_threshold(cv::Mat&);
 
}
