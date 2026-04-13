#pragma once

#include <array>

namespace Config::Tuning {
// Background subtraction
inline constexpr std::array<int, 3> VALID_K_SIZES{3, 5, 7};
inline constexpr int K_SIZE = 5;
inline constexpr double THRESH_DEV = 2;
inline constexpr int HIST_MASK = 2;

// Preprocessing

// Contour filtering
inline constexpr double MIN_BALL_AREA = 500.0;   // FIXME
inline constexpr double MAX_BALL_AREA = 1000.0;  // FIXME
inline constexpr double MIN_CIRCULARITY = 0.6;   // 1.0 = perfect circle

// Kalman filter
inline constexpr double P_NOISE = 10.0;  // Air drag and Magnus effect
inline constexpr double M_NOISE = 0.02;  // Based on camera calibration
}  // namespace Config::Tuning
