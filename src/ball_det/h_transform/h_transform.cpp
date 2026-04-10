#include "h_transform.hpp"
#include "../../helpers/tuning_params.hpp"

void Trans::print_circle_cand(const Circles &circle_cand)
{
    if (circle_cand.empty())
    {
        std::cout << "The are no circle candidates found in the hough transform" << '\n';
    }

    for (const auto &circle : circle_cand)
    {
        std::cout << "Circle: "
                  << "Centre x: " << circle[0] << ", "
                  << "Centre y: " << circle[1] << ", "
                  << "Radius: " << circle[2]
                  << std::endl;
    }
}

cv::Scalar getColor(std::string color)
{
    if (color == "green")
        return cv::Scalar{0, 255, 0};
    else if (color == "red")
        return cv::Scalar{255, 0, 0};
    else if (color == "white")
        return cv::Scalar{255};
    else if (color == "grey")
        return cv::Scalar{128};
    else
        return cv::Scalar{0, 0, 0}; // Black
}

void Trans::draw_circles(const Circles &circle_cand, cv::Mat &img, double text_font_scale)
{
    for (const auto &circle : circle_cand)
    {
        cv::Point centre(cvRound(circle[0]), cvRound(circle[1]));
        int r{cvRound(circle[2])};

        cv::Point textPt(centre.x + r + 10, centre.y + r + 10);
        cv::String textStr = "(" + std::to_string(centre.x) + ", " + std::to_string(centre.y) + ")" + " ,R=" + std::to_string(r);

        cv::circle(img, centre, r, getColor("grey"), 1);
        cv::circle(img, centre, 3, getColor("grey"), -1); // -1 means filled
        cv::putText(img, textStr, textPt, cv::FONT_HERSHEY_COMPLEX, text_font_scale, getColor("red"));
    }
}

// Prints and draws the circles detected also
// bool Trans::transform(cv::Mat& img){
//     Trans::Circle circle_cand{};
//     // Trans::Circle test_circle_cand{{1.0,2.0,3.0}, {2.0, 2.0, 2.0}};

//     // This algo is still trash, need to account for motion blur?
//     cv::HoughCircles(img, circle_cand, cv::HOUGH_GRADIENT_ALT,
//         1,
//         50,
//         335,
//         0.50,
//         5,
//         25
//     );

//     Trans::print_circle_cand(circle_cand);
//     Trans::draw_circles(circle_cand, img, 0.35);
//     return true;
// }

/**
 * @param img Takes in thresholded image
 * @return An Vec3f that holds x_coord, y_coord and radius (all floats) in order
 */
void find_circle_cand(const cv::Mat &img, Circles &out_circles, ball_det_count_t &count)
{

    Contours raw_contours;
    cv::findContours(img, raw_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter out unlikely circle candidates
    Contours circle_contours;
    for (const auto &contour : raw_contours)
    {
        double area = cv::contourArea(contour);
        double perimeter = cv::arcLength(contour, true);

        double circularity = (4 * M_PI * area) / (perimeter * perimeter);

        if (circularity < Config::Tuning::MIN_CIRCULARITY)
        {
            std::cout << "A contour of low circulatrity: " << circularity << " has been eliminated." << '\n';
            count.cir++;
            count.fail++;
            continue;
        }

        if (area < Config::Tuning::MIN_BALL_AREA || area > Config::Tuning::MAX_BALL_AREA)
        {
            std::cout << "A ball artifact of to small or large area: " << area << " has been eliminated. " << '\n';
            count.area++;
            count.fail++;
            continue;
        }

        circle_contours.push_back(contour);
        std::cout << "A ball artifact of area: " << area << " has been detected successfully." << '\n';
        count.success++;
    }

    // Get approx radius and center of circle
    for (const auto &contour : circle_contours)
    {
        cv::Point2f centre;
        float radius;
        cv::minEnclosingCircle(contour, centre, radius);

        cv::Vec3f circle{centre.x, centre.y, radius};
        out_circles.push_back(circle);
    }
}

void Trans::find_ball_cand(cv::Mat &img, Circles &all_circles, ball_det_count_t &count)
{
    Circles curr_circles;
    find_circle_cand(img, curr_circles, count);

    Trans::print_circle_cand(curr_circles);
    Trans::draw_circles(curr_circles, img);

    // Inserts current circles found in the image to all the circles
    all_circles.insert(all_circles.end(), curr_circles.begin(), curr_circles.end());
}

// End of countour file