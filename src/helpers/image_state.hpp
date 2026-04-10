#pragma once

#include <opencv2/core.hpp>

#include <vector>
#include <utility>
#include <string>

struct ball_det_count_t
{
    int cir;
    int area;
    int success;
    int fail;
};

using Image = std::pair<std::string, cv::Mat>;
using Images = std::vector<Image>;