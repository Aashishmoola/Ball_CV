#include "../include/video_ex.hpp"
// TODO: Use more specific header during final compilation
#include <opencv2/opencv.hpp>

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * @brief Extracts and saves all frames from a video into relative subdir "frames".
 * @param filepath filepath can be relative or absolute?
 * @note Timestamp labelled in frame --> Precision 6 digits after dec pt.
 */
void VidEx::processFrames(const std::string &filepath)
{

    fs::path vid_path(filepath);
    if (!fs::exists(vid_path))
        throw std::invalid_argument("Invalid Video Path.");

    auto vid_cap = cv::VideoCapture(vid_path.string());

    // frame_idx / fps * 1000 --> Time Elapsed in ms, Alt in case of frame drift
    const auto fps = vid_cap.get(cv::CAP_PROP_FPS);

    int frame_idx{0};
    while (true)
    {
        cv::Mat frame;
        if (!vid_cap.read(frame))
            break;

        const auto timestamp_ms = vid_cap.get(cv::CAP_PROP_POS_MSEC);
        // Precision of std::to_string() for double --> narrowing to 6 digits after dec pt.
        fs::path frame_path{vid_path.parent_path() / "frames" / ("frame_" + std::to_string(timestamp_ms) + ".png")};
        cv::imwrite(frame_path.string(), frame);

        frame_idx++;
    }

    const auto timestamp_ms_c = frame_idx / fps * 1000;

    // Temp Printing of time elaped for debugging purposes
    std::cout << "Frame: " << frame_idx
              << "ms: " << timestamp_ms_c << '\n';

    // Destructor is automatically called is vid_cap goes out of scope
    vid_cap.release();
}
