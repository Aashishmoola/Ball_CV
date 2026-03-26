#include "h_transform.hpp"

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
cv::Mat H_trans::transform(cv::Mat& img){
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

    return img;
    
}