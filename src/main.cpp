#include "ball_det/preproc/preproc.hpp"
#include "video_ex/video_ex.hpp"
#include "ball_det/back_sub/back_sub.hpp"
#include "ball_det/h_transform/h_transform.hpp"
#include "val/val.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include <filesystem>
#include <iostream>
namespace fs = std::filesystem;

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
    const std::string path_lib{"../images/back_sub"}; 

    Val::images_t images;
    images.path = path_lib;

    Val::read_img_files(images);

    for (auto& image: images.imgs_mat){
        image = Img_P::convert_grayscale(image);
    }
    Val::images_t& grayed_images = images;
    

    for (auto& it = grayed_images.imgs_mat.begin(); it != grayed_images.imgs_mat.end(); it++){}

    cv::Mat grayed_img_1 = Img_P::convert_grayscale(images.imgs_mat.at(0));
    cv::Mat grayed_img_2 = Img_P::convert_grayscale(images.imgs_mat.at(1));
    

    Bg_sub::del_B_mats_t del_B_mats = Bg_sub::comp_and_threshold(grayed_img_1, grayed_img_2, thresh_val, true);
    uchar calc_thresh_val{Bg_sub::threshold_calc(del_B_mats.unthresh_bright, thresh_calc_dev)};

 
    
    cv::Mat hist_del_B_img = Bg_sub::create_histogram(del_B_mats.unthresh, thresh_val, true);

    cv::imshow("del_b_img Bright", del_B_mats.bright);
    cv::imshow("del_b_img Shadow", del_B_mats.shadow);
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