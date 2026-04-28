#pragma once

#include <array>

namespace Config::Tuning
{
    // Background subtraction
    inline constexpr std::array<int, 3> kValidKSizes{3, 5, 7};
    inline constexpr int kKSize = 5;
    inline constexpr double kThreshDev = 2;
    inline constexpr int kHistMask = 2;

    // Preprocessing

    // Contour filtering
    inline constexpr double kMinBallArea = 500.0;  // FIXME
    inline constexpr double kMaxBallArea = 1000.0; // FIXME
    inline constexpr double kMinCircularity = 0.6; // 1.0 = perfect circle

    // Kalman filter
    inline constexpr double kPNoise = 10.0; // Air drag and Magnus effect
    inline constexpr double kMNoise = 0.02; // Based on camera calibration

    // Metrics Calculation
    inline constexpr double kTStep = 0.001;      // Euler integration step (s)
    inline constexpr double kMaxFlightDur = 10;  // Exit threshold for Eulre Integration
}
