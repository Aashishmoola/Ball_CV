#pragma once
#include "types/image_state.hpp"

#include <opencv2/core.hpp>

namespace Val
{

    bool readImgFiles(Images &images, const std::string &lib_path);
    void validateImages(const cv::Mat &img_1, const cv::Mat &img_2);
}
