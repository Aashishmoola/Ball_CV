#include "../include/preproc.hpp"

#include <iostream>

#include <opencv2/imgproc.hpp>

// cv::Mat --> n-dimm arr that stores many types of data (images, vectors, histograms)

cv::Mat Img_P::convert_grayscale(cv::Mat &img)
{
    cv::Mat grayed_img;
    cv::cvtColor(img, grayed_img, cv::COLOR_BGR2GRAY);

    // Printing out type convertion
    std::cout << "Type: " << cv::typeToString(img.type()) << " --> " << cv::typeToString(grayed_img.type()) << '\n';
    return grayed_img;
};

cv::Mat Img_P::apply_blur(cv::Mat &img)
{
    cv::Mat blurred_img;
    cv::GaussianBlur(img, blurred_img, cv::Size(3, 3), 0);
    return blurred_img;
};
