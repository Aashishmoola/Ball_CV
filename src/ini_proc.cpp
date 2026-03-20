#include "ini_proc.hpp"

#include <iostream>

// cv::Mat --> n-dimm arr that stores many types of data (images, vectors, histograms)

// Do out params need to be const? NO reassignment of img, returning new cv::Mat obj instead

cv::Mat Img_P::convert_grayscale(cv::Mat& img){
    cv::Mat grayed_img;
    cv::cvtColor(img, grayed_img, cv::COLOR_BGR2GRAY);
    return grayed_img;
};

cv::Mat Img_P::apply_blur(cv::Mat& img){
    cv::Mat blurred_img;
    cv::GaussianBlur(img, blurred_img, cv::Size(3,3), 0);
    return blurred_img;
};

cv::Mat Img_P::apply_threshold(cv::Mat& img){
    cv::Mat binary_img;
    cv::threshold(img, binary_img, Img_P::THRESHOLD_VAL, 255, cv::THRESH_BINARY // | cv::THRESH_OTSU
    );
    return binary_img;
};

void Img_P::print_circle_cand(const Img_P::Circle& circle_cand){
    for (const auto& circle: circle_cand){
        std::cout << "Circle: "
        << "Centre x: " << circle[0] << ", "
        << "Centre y: " << circle[1] << ", "
        << "Radius: " << circle[2]
        << std::endl;
    }
}

cv::Scalar getColor(std::string color){
    if (color == "green") return cv::Scalar{0, 255, 0};
    else if (color == "red") return cv::Scalar{255, 0, 0};
    else return cv::Scalar{0, 0, 0}; // Black
} 

void Img_P::draw_circles(const Img_P::Circle& circle_cand, cv::Mat& img, double text_font_scale){
    for (const auto& circle: circle_cand){
        float x = circle[0];
        float y = circle[1];
        float r = circle[2];

        cv::Point pt{x, y};

        cv::Point textPt{x + r + 10, y + r + 10};
        cv::String textStr = "(" + std::to_string(x) + ", " + std::to_string(y) + ")" + " ,R=" + std::to_string(r);

        cv::circle(img, pt, r, getColor("green"), 1);
        cv::circle(img, pt, 3, getColor("red"), -1); // -1 means filled
        cv::putText(img, textStr, textPt, cv::FONT_HERSHEY_COMPLEX, text_font_scale, getColor("red"));
    }
}

