#include <iostream>
#include <vector>
#include <opencv2/core.hpp>

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
    void sub_algo(const cv::Mat& img_in_1, const cv::Mat& img_in_2, cv::Mat& img_out);
}