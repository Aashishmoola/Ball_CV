#pragma once

#include <array>

namespace Config::Tuning
{
    // Background subtraction
    inline constexpr std::array<int, 3> VALID_K_SIZES{3, 5, 7};
    inline constexpr int K_SIZE = 3;
    inline constexpr double THRESH_DEV = 1.5;

    // Preprocessing
    inline constexpr int THRESHOLD_VAL = 148;

    // Contour filtering
    inline constexpr double MIN_BALL_AREA = 3000.0;
    inline constexpr double MAX_BALL_AREA = 10000.0;
    inline constexpr double MIN_CIRCULARITY = 0.3; // 1.0 = perfect circle

    // Kalman filter
    inline constexpr double P_NOISE = 10.0; // Air drag and Magnus effect
    inline constexpr double M_NOISE = 0.0; // Based on camera calibration
}
