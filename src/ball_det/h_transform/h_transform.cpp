#include "h_transform.hpp"


constexpr double MIN_BALL_AREA{3000.0};
constexpr double MAX_BALL_AREA{10000.0};
constexpr double MIN_CIRCULARITY{0.3}; // 1 is perfect, 0 is imperfect



void H_trans::print_circle_cand(const H_trans::Circle& circle_cand){
    if (circle_cand.empty()) {
        std::cout << "The are no circle candidates found in the hough transform" << '\n';
    }
    
    for (const auto& circle: circle_cand){
        std::cout << "Circle: "
        << "Centre x: " << circle[0] << ", "
        << "Centre y: " << circle[1] << ", "
        << "Radius: " << circle[2]
        << std::endl;
    }
}

cv::Scalar getColor(std::string color){
    if (color == "green") return cv::Scalar{0, 255, 0};
    else if (color == "red") return cv::Scalar{255, 0, 0};
    else return cv::Scalar{0, 0, 0}; // Black
} 

void H_trans::draw_circles(const H_trans::Circle& circle_cand, cv::Mat& img, double text_font_scale){
    for (const auto& circle: circle_cand){
        cv::Point centre{cvRound(circle[0]), cvRound(circle[1])};
        int r {cvRound(circle[2])};

        cv::Point textPt{centre.x + r + 10, centre.y + r + 10};
        cv::String textStr = "(" + std::to_string(centre.x) + ", " + std::to_string(centre.y) + ")" + " ,R=" + std::to_string(r);

        cv::circle(img, centre, r, getColor("green"), 1);
        cv::circle(img, centre, 3, getColor("red"), -1); // -1 means filled
        cv::putText(img, textStr, textPt, cv::FONT_HERSHEY_COMPLEX, text_font_scale, getColor("red"));
    }
}

// Prints and draws the circles detected also
bool H_trans::transform(cv::Mat& img){
    H_trans::Circle circle_cand{};
    // H_trans::Circle test_circle_cand{{1.0,2.0,3.0}, {2.0, 2.0, 2.0}};
    
    // This algo is still trash, need to account for motion blur?
    cv::HoughCircles(img, circle_cand, cv::HOUGH_GRADIENT_ALT,
        1, 
        50, 
        335,
        0.50,
        5,
        25
    );
    
    H_trans::print_circle_cand(circle_cand);
    H_trans::draw_circles(circle_cand, img, 0.35);
    return true;
}


// Start of contours file:
using Contour = std::vector<cv::Point>;
using Contours = std::vector<Contour>;
using Circles = std::vector<cv::Vec3f>;

/**
 * @param img Takes in thresholded image
 * @return An Vec3f that holds x_coord, y_coord and radius (all floats) in order
 */
void findCircleCand(const cv::Mat& img, Circles& out_circles){
    Contours raw_contours;
    cv::findContours(img, raw_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Filter out unlikely circle candidates
    Contours circle_contours;
    for (const auto& contour : raw_contours){
        double area = cv::contourArea(contour);
        double perimeter = cv::arcLength(contour, true);
        
        double circularity = (4 * M_PI * area) / (perimeter * perimeter);

        if (circularity < MIN_CIRCULARITY){
            std::cout << "A contour has been eliminated due to low circularity." << '\n';
            continue;
        }

        std::cout << area << '\n';
        if (area < MIN_BALL_AREA || area > MAX_BALL_AREA) {
            std::cout << "A ball artifact that is too small or large has been eliminated. " << '\n';
            continue;
        }

        circle_contours.push_back(contour);
        std::cout << "A ball artifact has been detected successfully." << '\n';
    }

    // Get approx radius and center of circle 
    for (const auto& contour : circle_contours){
        cv::Point2f centre;
        float radius;
        cv::minEnclosingCircle(contour, centre, radius);

        cv::Vec3f circle{centre.x, centre.y, radius};
        out_circles.push_back(circle);
    }
}

void H_trans::findBallCand(cv::Mat& img){
    Circles found_circles;
    findCircleCand(img, found_circles);

    H_trans::print_circle_cand(found_circles);
    H_trans::draw_circles(found_circles, img);
}

// End of countour file