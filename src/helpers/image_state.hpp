#pragma once

#include <opencv2/core.hpp> 

#include <vector>
#include <utility>
#include <string>

using Image = std::pair<std::string, cv::Mat>;
using Images = std::vector<Image>;