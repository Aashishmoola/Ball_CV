#include <filesystem>
#include <fstream>
#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp> //FIXME Is this needed

#include "back_sub.hpp"
#include "h_transform.hpp"
#include "preproc.hpp"
#include "data_val_f.hpp"
#include "types/image_state.hpp"
#include "met_calc.hpp"
#include "val.hpp"
#include "video_ex.hpp"

namespace fs = std::filesystem;

const std::string kImgLibPath{"../images/test_sample_1"};
const std::string kCsvOutputPath{"../csv_out"};

void printBallDet(const BallDetCount &count)
{
  std::cout << "Successful det: " << count.success
            << "  Failed Det: " << count.fail << '\n';
}

std::string procImgName(std::string A, std::string B)
{
  int start_i = B.rfind("_");
  int end_i = B.size() - 1;
  return A.append(B.substr(start_i, end_i - start_i + 1));
}

void dumpBallPathCsv(const BallsPos &balls_pos, const std::string &dir_path,
                     const std::string &filename)
{
  std::ofstream f(dir_path + '/' + filename + ".csv");
  f << "x,y\n";
  for (const auto &s : balls_pos)
  {
    f << s(0, 0) << ',' << s(1, 0) << '\n';
  }
}

void testPreProc()
{
  Images raw_images;
  Val::readImgFiles(raw_images, kImgLibPath);

  for (auto &[name, image] : raw_images)
  {
    image = ImgPreproc::convertGrayscale(image);
  }
  Images &grayed_images = raw_images;

  Images proc_images;
  for (auto it = grayed_images.begin(); it != grayed_images.end() - 1; it++)
  {
    auto [name_1, image_1] = *it;
    auto [name_2, image_2] = *(it + 1);

    Image img_map = std::make_pair(procImgName(name_1, name_2),
                                   BgSub::subAlgo(image_1, image_2));

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

void testBallDet()
{
  Images raw_images;
  Val::readImgFiles(raw_images, kImgLibPath);

  for (auto &[name, image] : raw_images)
  {
    image = ImgPreproc::convertGrayscale(image);
  }
  Images &grayed_images = raw_images;

  Images proc_images;
  for (auto it = grayed_images.begin(); it != grayed_images.end() - 1; it++)
  {
    auto [name_1, image_1] = *it;
    auto [name_2, image_2] = *(it + 1);

    Image img_map = std::make_pair(procImgName(name_1, name_2),
                                   BgSub::subAlgo(image_1, image_2));

    proc_images.push_back(img_map);
  }

  // Runs hough transform and print image before and after circle drawn
  Circles circles;
  BallDetCount count;
  for (auto &[name, image] : proc_images)
  {
    Trans::findBallCand(image, circles, count);
    cv::imshow(name + "after_ball_det", image);

    std::cout << "Showing image: " << name << '\n';
  }

  printBallDet(count);
  cv::waitKey(0);
}

void testMain()
{
  Images raw_images;
  Val::readImgFiles(raw_images, kImgLibPath);

  for (auto &[name, image] : raw_images)
  {
    image = ImgPreproc::convertGrayscale(image);
  }
  Images &grayed_images = raw_images;

  Images proc_images;
  for (auto it = grayed_images.begin(); it != grayed_images.end() - 1; it++)
  {
    auto [name_1, image_1] = *it;
    auto [name_2, image_2] = *(it + 1);

    Image img_map = std::make_pair(procImgName(name_1, name_2),
                                   BgSub::subAlgo(image_1, image_2));

    proc_images.push_back(img_map);
  }

  // Runs hough transform and print image before and after circle drawn
  BallDetCount count;
  Circles circles;
  for (auto &[name, image] : proc_images)
  {
    Trans::findBallCand(image, circles, count);
    // cv::imshow(name + "after_ball_det", image);

    // std::cout << "Showing image: " << name << '\n';
  }

  printBallDet(count);

  BallsPos balls_pos;
  DataValF::circlesToBallsPos(circles, balls_pos);
  dumpBallPathCsv(balls_pos, kCsvOutputPath, "before_kf");

  DataValF::runKFilter(balls_pos);

  dumpBallPathCsv(balls_pos, kCsvOutputPath, "after_kf");

  BallStates ball_states;
  DataValF::ballsPosToState(balls_pos, ball_states);

  // Calc the ball metrics, max disp from the ball centre
  BallDisp max_disp = MetCalc::calMet(ball_states);

  // cv::waitKey(0);
}

int main()
{
  testMain();
  return 0;
}
