#pragma once

#include <opencv2/imgproc.hpp>
#include "config/tuning_params.hpp"

namespace Img_P {
    cv::Mat convert_grayscale(cv::Mat&);
    cv::Mat apply_blur(cv::Mat&);
    cv::Mat apply_threshold(cv::Mat&);

}
