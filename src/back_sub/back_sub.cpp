#include "back_sub.hpp"

#include <opencv2/imgproc.hpp>


const uchar test_threshold{140};
const int k_size{3};

// Can use cv::absdiff instead in needed.
cv::Mat comp_and_threshold(const cv::Mat& img_1, const cv::Mat& img_2, const uchar threshold){
    
    
    // Initializing with the first image
    cv::Mat out_img = cv::Mat::zeros(img_1.size(), img_2.type());

    const int img_rows{img_1.rows};
    const int img_cols{img_1.cols};

    // at method does bounds checking in debug mode
    // int safe as only ++ing
    for (int i=0; i<img_rows; i++){
        for (int j=0; j<img_cols; j++) {
            // del_b --> change in brightness
            int del_b = static_cast<int>(img_2.at<uchar>(i, j)) - static_cast<int>(img_1.at<uchar>(i, j));
            // nwing conv is fine here as it is not nrwing
            
            uchar del_b_char = static_cast<uchar>(std::abs(del_b));
            out_img.at<uchar>(i, j) = (del_b_char > threshold) ? 255 : 0;
        }
    }

    return out_img;
}

// x and y are already relative indexes to the padded image, starting without the padding itself.  
bool morph_min_max(const Bg_sub::pt_t& pt, const cv::Mat& k, const cv::Mat& padded_img, const uchar comp_val){
    int k_size{k.rows};
    int offset{-k_size/2}; // Both padding size and offset must be the same.

    // (x or y) + offset--> Adjusting for image index relative to kernal index

    for (int i=0; i<k_size; i++){
        for (int j=0; j<k_size; j++){
            if (k.at<uchar>(i, j) == 1 && padded_img.at<uchar>(pt.x+offset+i,pt.y+offset+j) == comp_val) return true;
        }
    }
    return false;
}

cv::Mat morph_process(cv::Mat& img, Bg_sub::BIN_P proc_type, const int k_size=3) {
    // Image type validation should already be done.
    if (!(k_size == 3 || k_size == 5 || k_size == 7)) {
        throw std::invalid_argument("The kernal size should of size 3, 5 or 7.");
    }   

    // Create the return mat
    cv::Mat out_img = cv::Mat::zeros(img.size(), img.type());

    // Make the comp_val calc more readable 
    uchar comp_val;
    if(proc_type == Bg_sub::BIN_P::DIALATION) comp_val = 255;
    else if (proc_type == Bg_sub::BIN_P::EROSION) comp_val = 0;
    else throw std::invalid_argument("Unknown Morph process type"); // Shoul never exe

    cv::Mat k{cv::getStructuringElement(cv::MORPH_RECT, cv::Size(k_size, k_size))};

    int p_size{k_size/2}; // Will round down by truncation
    cv::Mat padded_img;
    cv::copyMakeBorder(img, padded_img, p_size, p_size, p_size, p_size, cv::BORDER_CONSTANT, cv::Scalar(0));

    //out_image.size == img.size
    for (int i=p_size; i<padded_img.rows-p_size; i++){
        for (int j=p_size; j<padded_img.cols-p_size; j++){
            out_img.at<uchar>(i-p_size,j-p_size) = morph_min_max(Bg_sub::pt_t{i, j}, k, padded_img, comp_val) ? 255 : 0;
        }
    }
    return out_img;
}

cv::Mat Bg_sub::sub_algo(const cv::Mat& img_1, const cv::Mat& img_2){
    // 1. Calculate del B --> Map of del in pixel brighness due to motion
    // 2. Threshold --> filtering of large and small del B into binary values --> makes 1 more obvious 
    // 3. Removing noise and holes
        // Opening --> Erosing >> Dialation
        // Closing --> Dialation >> Erosion

    // Type --> No. of channels (in each pixel) (1 --> greyscale, 3 --> BGR) 
    // + Data type of each channel value (CV_8U --> Unsigned int, 8 bits long)
    if (img_1.size() != img_2.size() || img_1.type() != img_2.type()){
        throw std::invalid_argument("Both inputted images are not of the same type");
    }

    cv::Mat proc_img= comp_and_threshold(img_1, img_2, test_threshold);
    morph_process(morph_process(proc_img, Bg_sub::BIN_P::DIALATION, 3), Bg_sub::BIN_P::EROSION, 3))

}


    