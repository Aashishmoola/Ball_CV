#include "ini_proc.hpp"
#include "video_ex/video_ex.hpp"
#include "back_sub/back_sub.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>


void ball_dec_fine(){
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
    // Img_P::draw_circles(circle_cand, blurred_img, 0.35);
    
    cv::imshow("Blurred Img", blurred_img);
    cv::waitKey(0);
}

void video_ex(){
    // Vid_ex::process_frames();
}

void test_back_sub(){
    cv::Mat img_1 = cv::imread("../images/back_sub/test_1.png");
    cv::Mat img_2 = cv::imread("../images/back_sub/test_2.png");

    cv::Mat grayed_img_1 = Img_P::convert_grayscale(img_1);
    cv::Mat grayed_img_2 = Img_P::convert_grayscale(img_2);

    cv::Mat delta{grayed_img_1.size(), grayed_img_1.type()};
    Back_sub::sub_algo(grayed_img_1, grayed_img_2, delta);

    cv::imshow("Img_1", img_1);
    cv::imshow("Img_2", img_2);
    cv::imshow("delta_brightness_img", delta);
    cv::waitKey(0);
}


int main(){
    // video_ex();
    test_back_sub();
    return 0;
}