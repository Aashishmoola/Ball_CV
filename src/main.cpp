#include <filesystem>
#include <fstream>
#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "back_sub.hpp"
#include "h_transform.hpp"
#include "preproc.hpp"
#include "data_val_f.hpp"
#include "types/image_state.hpp"
#include "met_calc.hpp"
#include "val.hpp"
#include "video_ex.hpp"

namespace fs = std::filesystem;

const std::string IMG_LIB_PATH{"../images/test_sample_1"};
const std::string CSV_OUTPUT_PATH{"../csv_out"};

void print_ball_det(const ball_det_count_t &count)
{
  std::cout << "Successful det: " << count.success
            << "  Failed Det: " << count.fail << '\n';
}

std::string proc_img_name(std::string A, std::string B)
{
  int start_i = B.rfind("_");
  int end_i = B.size() - 1;
  return A.append(B.substr(start_i, end_i - start_i + 1));
}

void dump_ball_path_csv(const Balls_pos &balls_pos, const std::string &dir_path,
                        const std::string &filename)
{
  std::ofstream f(dir_path + '/' + filename + ".csv");
  f << "x,y\n";
  for (const auto &s : balls_pos)
  {
    f << s(0, 0) << ',' << s(1, 0) << '\n';
  }
}

void test_pre_proc()
{
  Images raw_images;
  Val::read_img_files(raw_images, IMG_LIB_PATH);

  for (auto &[name, image] : raw_images)
  {
    image = Img_P::convert_grayscale(image);
  }
  Images &grayed_images = raw_images;

  Images proc_images;
  for (auto it = grayed_images.begin(); it != grayed_images.end() - 1; it++)
  {
    auto [name_1, image_1] = *it;
    auto [name_2, image_2] = *(it + 1);

    Image img_map = std::make_pair(proc_img_name(name_1, name_2),
                                   Bg_sub::sub_algo(image_1, image_2));

    proc_images.push_back(img_map);
  }

  // Print the images generated
  for (auto &[name, image] : proc_images)
  {
    cv::imshow(name + "before_ball_det", image);

    std::cout << "Showing image: " << name << '\n';
  }

  cv::waitKey(0);
}

void test_ball_det()
{
  Images raw_images;
  Val::read_img_files(raw_images, IMG_LIB_PATH);

  for (auto &[name, image] : raw_images)
  {
    image = Img_P::convert_grayscale(image);
  }
  Images &grayed_images = raw_images;

  Images proc_images;
  for (auto it = grayed_images.begin(); it != grayed_images.end() - 1; it++)
  {
    auto [name_1, image_1] = *it;
    auto [name_2, image_2] = *(it + 1);

    Image img_map = std::make_pair(proc_img_name(name_1, name_2),
                                   Bg_sub::sub_algo(image_1, image_2));

    proc_images.push_back(img_map);
  }

  // Runs hough transform and print image before and after circle drawn
  Circles circles;
  ball_det_count_t count;
  for (auto &[name, image] : proc_images)
  {
    Trans::find_ball_cand(image, circles, count);
    cv::imshow(name + "after_ball_det", image);

    std::cout << "Showing image: " << name << '\n';
  }

  print_ball_det(count);
  cv::waitKey(0);
}

void test_main()
{
  Images raw_images;
  Val::read_img_files(raw_images, IMG_LIB_PATH);

  for (auto &[name, image] : raw_images)
  {
    image = Img_P::convert_grayscale(image);
  }
  Images &grayed_images = raw_images;

  Images proc_images;
  for (auto it = grayed_images.begin(); it != grayed_images.end() - 1; it++)
  {
    auto [name_1, image_1] = *it;
    auto [name_2, image_2] = *(it + 1);

    Image img_map = std::make_pair(proc_img_name(name_1, name_2),
                                   Bg_sub::sub_algo(image_1, image_2));

    proc_images.push_back(img_map);
  }

  // Runs hough transform and print image before and after circle drawn
  ball_det_count_t count;
  Circles circles;
  for (auto &[name, image] : proc_images)
  {
    Trans::find_ball_cand(image, circles, count);
    // cv::imshow(name + "after_ball_det", image);

    // std::cout << "Showing image: " << name << '\n';
  }

  print_ball_det(count);

  Balls_pos balls_pos;
  Data_val_f::circles_to_balls_pos(circles, balls_pos);
  dump_ball_path_csv(balls_pos, CSV_OUTPUT_PATH, "before_kf");

  Data_val_f::run_k_filter(balls_pos);

  dump_ball_path_csv(balls_pos, CSV_OUTPUT_PATH, "after_kf");

  Ball_states ball_states;
  Data_val_f::balls_pos_to_state(balls_pos, ball_states);

  // Calc the ball metrics, max disp from the ball centre
  ball_disp_t max_disp = Met_calc::cal_met(ball_states);

  // cv::waitKey(0);
}

int main()
{
  test_main();
  return 0;
}