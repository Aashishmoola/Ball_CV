#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>

namespace Bg_sub{
    struct pt_t {
        int x;
        int y;
    };
    struct del_B_mats_t{
        cv::Mat bright;
        cv::Mat shadow;
        cv::Mat unthresh;
        cv::Mat unthresh_bright;
    };

    enum class BIN_P {
        DIALATION,
        EROSION,
    };
    
    uchar threshold_calc(const cv::Mat img, double deviation);
    cv::Mat sub_algo(const cv::Mat& img_in_1, const cv::Mat& img_in_2);
    del_B_mats_t comp_and_threshold(const cv::Mat& img_1, const cv::Mat& img_2, bool should_threshold, uchar threshold=0);
    cv::Mat create_histogram(const cv::Mat& img, uchar threshold_indicator, int mask_size=0, bool should_print=false);
}