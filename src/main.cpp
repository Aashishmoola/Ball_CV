#include "ini_proc.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>


int main(){
    Img_P::Circle circle_cand{};
    Img_P::Circle test_circle_cand{{1.0,2.0,3.0}, {2.0, 2.0, 2.0}};

    // Read file:
    cv::Mat img = cv::imread("/home/aashishrapsodo/rapsodo/repos/ball_cv/images/golf_test_img.jpg");
    cv::Mat grayed_img = Img_P::convert_grayscale(img);
    cv::Mat blurred_img = Img_P::apply_blur(grayed_img);
    // cv::Mat binary_img = Img_P::apply_threshold(blurred_img);
    // cv::imshow("Binary Img", binary_img);


    // This algo is still trash, need to account for motion blur?
    cv::HoughCircles(blurred_img, circle_cand, cv::HOUGH_GRADIENT_ALT,
        1, 
        50, 
        335,
        0.50,
        5,
        25
    );

    Img_P::print_circle_cand(circle_cand);
    Img_P::draw_circles(circle_cand, blurred_img, 0.35);

    cv::imshow("Blurred Img", blurred_img);
    cv::waitKey(0);

    return 0;
}