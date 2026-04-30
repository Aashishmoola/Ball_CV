#include "../include/val.hpp"

#include <cassert>
#include <cctype>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "spdlog/spdlog.h"
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include <../include/constants/io_consts.hpp>

namespace fs = std::filesystem;

// Internal helper functions

/** @return Returns -1 on failure; compareByFnum can continue */
int extractFirstNum(const std::string &f_name) {
  auto it = std::find_if(f_name.begin(), f_name.end(), isdigit);
  if (it == f_name.end()) {
    spdlog::warn(f_name + " has no integer found.");
    return -1;
  }

  return std::stoi(f_name.substr(std::distance(f_name.begin(), it)));
}

/** @brief Callback func for sort
    @return boolean for a < b
    @note filenames with no num will propogate to the top
*/
bool compareByFnum(const Image &a, const Image &b) {
  return extractFirstNum(a.first) < extractFirstNum(b.first);
}

bool findInStrVec(const std::vector<std::string> &vec, std::string val) {
  auto it = std::find(vec.begin(), vec.end(), val);
  return it != vec.end();
}

// External helper functions
void Val::validateImages(const cv::Mat &img_1, const cv::Mat &img_2) {
  assert(!img_1.empty());
  assert(!img_2.empty());
  assert(img_1.size() == img_2.size());
  assert(img_1.type() == img_2.type());
}

void Val::readImgFiles(Images &images, const std::string &lib_path,
                       cv::Size img_size, int img_type) {
  // AUtomatically converts string to path obj
  if (!fs::exists(lib_path)) {
    throw std::runtime_error("Image dir does not exist: " + lib_path);
  }

  cv::Size det_mat_size;
  int det_mat_type;

  for (const auto &img_f : fs::directory_iterator(lib_path)) {
    std::string img_f_path_str = img_f.path().string();

    if (!img_f.is_regular_file()) {
      std::cerr << img_f_path_str << ": Is not a valid file" << '\n';
      continue;
    }

    if (!findInStrVec(kValidFileExt, img_f.path().extension().string())) {
      std::cerr << img_f_path_str << ": extension is not valid for a image file"
                << '\n';
      continue;
    }

    cv::Mat img_mat = cv::imread(img_f_path_str);

    if (img_mat.empty()) {
      std::cerr << img_f_path_str << ": Unable to read image file" << '\n';
      continue;
    }

    if (img_mat.type() != img_type)
      std::runtime_error("Image types do not match");
    if (img_mat.size() != img_size)
      spdlog::warn(
          "Image size does not match. Continuing with true image size");

    std::pair<std::string, cv::Mat> img_map{img_f.path().stem().string(),
                                            img_mat};
    images.push_back(img_map);
  }

  if (images.empty()) {
    throw std::runtime_error("No images have been successfully read from " +
                             lib_path);
    return;
  }

  // Sort the images
  std::sort(images.begin(), images.end(), compareByFnum);

  for (const auto &img : images) {
    std::cout << img.first << " :Image has been successfully read" << '\n';
  }
  std::cout << images.size() << ": Images have been read from " << lib_path
            << '\n';

  return true;
}
