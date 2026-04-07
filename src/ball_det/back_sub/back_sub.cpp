#include "back_sub.hpp"
#include "../../val/val.hpp"

#include <opencv2/imgproc.hpp>


// TODO: Replace Bg_pt with cv::Point 
// TODO: Seperate validation into a different layer. Validation for these function types.

// Prod consts
constexpr std::array<int, 3> VALID_K_SIZES {3, 5, 7};

// Dev consts
// constexpr uchar TEST_THRESH{140};
constexpr int K_SIZE{3};
constexpr double THRESH_DEV{1.5};

/**
 * @param img unthesholded del B img, bright or not bright.
 * @param deviation How much should the treshold value deviate from the mean
 * @return Returns two estimated threshold value that is ${devation} stddev from mean 
 */
uchar Bg_sub::threshold_calc(const cv::Mat img, double deviation){
    // By nature of comp of del B, foreground will be lighter pixels
    cv::Scalar mean, stddev;
    cv::meanStdDev(img, mean, stddev);
    uchar threshold = static_cast<uchar>(cvRound(mean[0] + deviation * stddev[0]));
    std::cout << "Calc threshold val: " << static_cast<int>(threshold) << '\n';
    return threshold;
}

/**
 * @param mask_size // Masks the first x values of the histogram {most freq background pixels} before normalization
 */
cv::Mat Bg_sub::create_histogram(const cv::Mat& img, uchar thresh_indicator, int mask_size,  bool should_print){
    if (mask_size < 0 || mask_size > 256) {
        throw std::invalid_argument("Mask size is not within uchar range.");
    }
    int mask_index{mask_size - 1};

    cv::Mat hist_img;
    std::vector<int> channels{0}; // Channel index not number
    std::vector<int>hist_size{256};
    std::vector<float>hist_range{0.0, 256.0};

    cv::calcHist(std::vector<cv::Mat>{img}, channels, cv::Mat(), hist_img, hist_size, hist_range);
    
    if (should_print) {
        // Hist will start printing from the top left of the image
        constexpr int line_width{5};

        cv::Scalar white_color{255};
        cv::Scalar gray_color{128};
        
        int hist_img_cols{hist_size[0]};
        int img_disp_cols{hist_img_cols * line_width};
        
        int img_disp_rows_scale = 400;
        
        int v_offset = 100;
        int img_disp_rows = img_disp_rows_scale + v_offset;
        
        // Mask the histogram
        for(int i=0;i<mask_index;i++){
            hist_img.at<float>(i) = 0;
        }

        cv::Mat hist_img_norm;
        cv::normalize(hist_img, hist_img_norm, 0, 1.0, cv::NORM_MINMAX);
        
        
        cv::Mat hist_img_disp = cv::Mat::zeros(cv::Size{img_disp_cols, img_disp_rows}, CV_8UC1);
        
        for (int i=0;i<hist_img_cols;i++){
            int freq_count{static_cast<int>(hist_img_norm.at<float>(i) * img_disp_rows_scale)};
            for (int j=i*line_width;j<i*line_width+line_width;j++){
                if (i==static_cast<int>(thresh_indicator)){
                    cv::line(hist_img_disp, cv::Point{j, 0}, cv::Point{j, freq_count}, white_color, 1);
                } else {
                    cv::line(hist_img_disp, cv::Point{j, 0}, cv::Point{j, freq_count}, gray_color, 1);
                }
            }
        }


        return hist_img_disp;
    }
    return hist_img;
}

bool is_k_size_valid(int k_size){
    for (const auto valid_k_size: VALID_K_SIZES){
        if (k_size ==  valid_k_size) return true;
    }
    return false;
}

/**
 * @param should_threshold Implemented and used only for testing purposed and not for anything else.
 */
Bg_sub::del_B_mats_t Bg_sub::comp_and_threshold(const cv::Mat& img_1, const cv::Mat& img_2, bool should_threshold, uchar threshold){
    
    // Initializing with the first image //FIXME There has to be a better way to do this.
    cv::Mat out_img_bright{cv::Mat::zeros(img_1.size(), img_1.type())};
    cv::Mat out_img_shadow{cv::Mat::zeros(img_1.size(), img_1.type())};
    cv::Mat out_img_unthresh{cv::Mat::zeros(img_1.size(), img_1.type())};
    cv::Mat out_img_unthresh_bright{cv::Mat::zeros(img_1.size(), img_1.type())};

    const int img_rows{img_1.rows};
    const int img_cols{img_1.cols};

    // at method does bounds checking in debug mode
    for (int i=0; i<img_rows; i++){
        for (int j=0; j<img_cols; j++) {
            // del_b --> change in brightness
            int del_b = static_cast<int>(img_2.at<uchar>(i, j)) - static_cast<int>(img_1.at<uchar>(i, j));
                        
            //FIXME Checks every iteration
            if (should_threshold) {
                if (del_b > 0) {
                    out_img_bright.at<uchar>(i,j) = (static_cast<uchar>(del_b) > threshold) ? 255 : 0;
                } else if  (del_b < 0) {
                    out_img_shadow.at<uchar>(i,j) = (static_cast<uchar>(-del_b) > threshold) ? 255 : 0;
                } else continue; // Ignores static parts of the image -> del_b == 0;
            } 

            out_img_unthresh.at<uchar>(i, j) = static_cast<uchar>(std::abs(del_b));
            if (del_b > 0) out_img_unthresh_bright.at<uchar>(i,j) = static_cast<uchar>(del_b);
        }
    }

    return del_B_mats_t{out_img_bright, out_img_shadow, out_img_unthresh, out_img_unthresh_bright};
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

cv::Mat morph_process(const cv::Mat& img, Bg_sub::BIN_P proc_type, const int k_size) {
    // Image type validation should already be done.
    assert (is_k_size_valid(k_size) && "The kernal size should of size 3, 5 or 7.");
 
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

cv::Mat morph_opening(const cv::Mat& img, double erosion_k_size, double dialation_k_size){
    cv::Mat erosed_img = morph_process(img, Bg_sub::BIN_P::EROSION, erosion_k_size);
    cv::Mat dialated_img = morph_process(erosed_img, Bg_sub::BIN_P::DIALATION, dialation_k_size);

    return dialated_img;
}

cv::Mat morph_closing(const cv::Mat& img, double erosion_k_size, double dialation_k_size){
    cv::Mat dialated_img = morph_process(img, Bg_sub::BIN_P::DIALATION, dialation_k_size);
    cv::Mat erosed_img = morph_process(dialated_img, Bg_sub::BIN_P::EROSION, erosion_k_size);

    return erosed_img;
}

cv::Mat Bg_sub::sub_algo(const cv::Mat& img_1, const cv::Mat& img_2){
    Val::validate_images(img_1, img_2);

    // 1. Calculate del B --> Map of del in pixel brighness due to motion
    // 2. Threshold --> filtering of large and small del B into binary values --> makes 1 more obvious 
    // 3. Removing noise and holes
        // Opening(removes small foreground artifacts) --> Erosing (Shrinks forground artifacts and boundaries) >> Dialation (Expands boundaries back to recreate original boundaries)
        // Closing(covers up small holes in the foreground) --> Dialation >> Erosion

    // Opening with same kernal size to remove small foreground artifacts
    Bg_sub::del_B_mats_t del_B_mats = comp_and_threshold(img_1, img_2, false);
    uchar threshold = threshold_calc(del_B_mats.unthresh_bright, THRESH_DEV);

    // Thersholding in another layer as threshold value needs to be dyn calc
    cv::Mat thresholded_bright;
    cv::threshold(del_B_mats.unthresh_bright, thresholded_bright, threshold, 255, cv::THRESH_BINARY);

    // Showing hist also
    cv::imshow("Created histogram", create_histogram(del_B_mats.unthresh_bright, threshold, 20, true));

    return morph_opening(thresholded_bright, K_SIZE, K_SIZE);
}


    