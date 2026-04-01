#pragma once

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <vector>
#include <iostream>

namespace Val{
    struct images_t{
        std::vector<cv::Mat> imgs_mat; 
        std::vector<std::string> f_names;
        std::string path;
        int size{0};
    };

    bool read_img_files(images_t& images);
}