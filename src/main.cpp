#include "ball_det/preproc/preproc.hpp"
#include "video_ex/video_ex.hpp"
#include "ball_det/back_sub/back_sub.hpp"
#include "ball_det/h_transform/h_transform.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>



cv::Mat test_img_preproc(const std::string& full_src_name){
    // Read file:
    cv::Mat img = cv::imread(full_src_name);
    cv::Mat grayed_img = Img_P::convert_grayscale(img);
    cv::Mat blurred_img = Img_P::apply_blur(grayed_img);
    // cv::Mat binary_img = Img_P::apply_threshold(blurred_img);

    return blurred_img;
}

void test_video_ex(){
    // Vid_ex::process_frames();
}

void test_back_sub(){
    constexpr int thresh_val{35};
    cv::Mat img_1 = cv::imread("../images/back_sub/test_1.png");
    cv::Mat img_2 = cv::imread("../images/back_sub/test_2.png");

    cv::Mat grayed_img_1 = Img_P::convert_grayscale(img_1);
    cv::Mat grayed_img_2 = Img_P::convert_grayscale(img_2);

    cv::Mat del_B_img = Bg_sub::comp_and_threshold(grayed_img_1, grayed_img_2, 0, false);
    cv::Mat del_B_img_thresh = Bg_sub::comp_and_threshold(grayed_img_1, grayed_img_2, thresh_val, true);

    cv::Mat hist_del_B_img = Bg_sub::create_histogram(del_B_img, thresh_val, true);

    cv::imshow("del_b_img Not Thresholded", del_B_img);
    cv::imshow("del_b_img Thresholded", del_B_img_thresh);
    cv::imshow("hist_del_B_img", hist_del_B_img);



    // cv::Mat processed_img = Bg_sub::sub_algo(grayed_img_1, grayed_img_2);

    // cv::Mat combined_preproc;
    // cv::Mat combined_postproc;
    // cv::vconcat(img_1, img_2, combined_preproc);
    // cv::vconcat(del_B_threshold_img, processed_img, combined_postproc);

    // cv::imshow("Combined Preprocessed", combined_preproc);
    // cv::imshow("Combined PostProcessed", combined_postproc);

    cv::waitKey(0);
}


int main(){
    // test_img_preproc("/home/aashishrapsodo/rapsodo/repos/ball_cv/images/golf_test_img.jpg");
    // test_video_ex();

    test_back_sub();
    return 0;
}