#include <iostream>
#include <vector>
#include <opencv2/imgproc.hpp>

namespace Bg_sub{
    const std::vector<int> valid_k_sizes {3, 5, 7};
    struct pt_t {
        int x;
        int y;
    };
    enum class BIN_P {
        DIALATION,
        EROSION,
    };
    cv::Mat sub_algo(const cv::Mat& img_in_1, const cv::Mat& img_in_2);
    cv::Mat comp_and_threshold(const cv::Mat& img_1, const cv::Mat& img_2, const uchar threshold, bool should_threshold=true);
    cv::Mat create_histogram(const cv::Mat& img, int thresh_index, bool should_print=false);
}