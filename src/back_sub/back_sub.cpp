#include "back_sub.hpp"

// I am not scared of uncertainty.

void comp_and_threshold(const cv::Mat& img_in_1, const cv::Mat& img_in_2, cv::Mat& b_img_out, const uchar threshold){
    // Type --> No. of channels (in each pixel) (1 --> greyscale, 3 --> BGR) 
    // + Data type of each channel value (CV_8U --> Unsigned int, 8 bits long)
    if (img_in_1.size() != img_in_2.size() || img_in_1.type() != img_in_2.type()){
        throw std::invalid_argument("Both inputted images are not of the same type");
    }

    // Can use cv::absdiff instead in needed.
    
    // Initialisning size with the firs t image
    const int img_rows{img_in_1.rows};
    const int img_cols{img_in_1.cols};

    // at method does bounds checking in debug mode
    // int safe as only ++ing
    for (int i=0; i<img_rows; i++){
        for (int j=0; j<img_cols; j++) {
            // del_b --> change in brightness
            int del_b = static_cast<int>(img_in_2.at<uchar>(i, j)) - static_cast<int>(img_in_1.at<uchar>(i, j));
            // nwing conv is fine here as it is not nrwing
            
            uchar del_b_char = static_cast<uchar>(std::abs(del_b));
            b_img_out.at<uchar>(i, j) = (del_b_char > threshold) ? 255 : 0;
        }
    }

    // Increasing uchar mat value by factor 5 and offset 0
    b_img_out.convertTo(b_img_out, -1, 1.2 , 0);

    

}

void Back_sub::sub_algo(const cv::Mat& img_in_1, const cv::Mat& img_in_2, cv::Mat& img_out){
    first_round(img_in_1, img_in_2, img_out);
}

