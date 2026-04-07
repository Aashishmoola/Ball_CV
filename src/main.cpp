#include "ball_det/preproc/preproc.hpp"
#include "video_ex/video_ex.hpp"
#include "ball_det/back_sub/back_sub.hpp"
#include "ball_det/h_transform/h_transform.hpp"
#include "val/val.hpp"
#include "helpers/image_state.hpp"

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

std::string proc_img_name(std::string A, std::string B){
    int start_i = B.rfind("_");
    int end_i = B.size() - 1;
    return A.append(B.substr(start_i, end_i - start_i + 1));
}

void test_ball_det(){
    const std::string img_lib_path{"../images/back_sub"}; 

    Images raw_images;
    Val::read_img_files(raw_images, img_lib_path);

    for (auto& [name, image]: raw_images){
        image = Img_P::convert_grayscale(image);
    }
    Images& grayed_images = raw_images;
    
    Images proc_images;
    for (auto it = grayed_images.begin(); it != grayed_images.end() - 1; it++){
        auto [name_1, image_1] = *it;
        auto [name_2, image_2] = *(it + 1);

        Image img_map = std::make_pair(proc_img_name(name_1, name_2), Bg_sub::sub_algo(image_1, image_2));

        proc_images.push_back(img_map);
    }

    // Runs hough transform and print image before and after circle drawn
    for (auto& [name, image]: proc_images){
        cv::imshow(name + "_before H_trans", image);
        // FIXME not working due to inverse threshold; ball must be white.
        H_trans::findBallCand(image);
        cv::imshow(name + "after_H_trans", image);

        std::cout << "Showing image: " << name << '\n';
    }

    cv::waitKey(0);
}


int main(){

    test_ball_det();
    return 0;
}