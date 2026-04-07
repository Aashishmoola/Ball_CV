#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <vector>
#include <iostream>

// FIXME Should this be here?
using Images = std::vector<std::pair<std::string, cv::Mat>>;

namespace Val{

    bool read_img_files(Images& images, const std::string& lib_path);
    void validate_images(const cv::Mat& img_1, const cv::Mat& img_2);
}