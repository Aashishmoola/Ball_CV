#pragma once

namespace Config::Hardware
{
    inline constexpr double kFps = 30.0;
    inline constexpr double kTStepFrame = 1.0 / kFps;
    inline constexpr double kYImgPx = 1632.0;
    inline constexpr double kXImgPx = 260.0;
    inline constexpr double kYSenMm = 2.7;
    inline constexpr double kXSenMm = 3.6;
    inline constexpr double kZM = 5;        // Dummy distance from the ball to ball radar
    inline constexpr double kFLenMm = 4.8;  // ESP-32 cam default focal length (mm)
    inline constexpr double kYOffset = 1.0; // Distance of ball centre from ground (m) -- needs calibration
    inline constexpr double kXROffset = 0;  // Dummy radar offset for camera radar module
    inline constexpr double kYROffset = 0.05;

    inline constexpr double kFxPx = kFLenMm * kXImgPx / kXSenMm;
    inline constexpr double kFyPx = kFxPx; // Same for sqaure pixels

}
