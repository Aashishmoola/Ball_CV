#pragma once

#include <opencv2/core.hpp>

namespace BgSub
{
    struct Pt
    {
        int x;
        int y;
    };
    struct DelBMats
    {
        cv::Mat bright;
        cv::Mat shadow;
        cv::Mat unthresh;
        cv::Mat unthresh_bright;
    };

    enum class BinP
    {
        kDialation,
        kErosion,
    };

    uchar thresholdCalc(const cv::Mat img, double deviation);
    cv::Mat subAlgo(const cv::Mat &img_in_1, const cv::Mat &img_in_2);
    DelBMats compAndThreshold(const cv::Mat &img_1, const cv::Mat &img_2, bool should_threshold, uchar threshold = 0);
    cv::Mat createHistogram(const cv::Mat &img, uchar threshold_indicator, int mask_size = 0, bool should_print = false);
}
