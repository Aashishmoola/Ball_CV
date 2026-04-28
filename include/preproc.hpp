#pragma once

#include <opencv2/core.hpp>

#include "config/tuning_params.hpp"

namespace ImgPreproc
{
    cv::Mat convertGrayscale(cv::Mat &);
    cv::Mat applyBlur(cv::Mat &);
    cv::Mat applyThreshold(cv::Mat &);

}
