#pragma once

namespace Config::Hardware
{
    inline constexpr double FPS = 30.0;
    inline constexpr double T_STEP_FRAME = 1.0 / FPS;
    inline constexpr double Y_IMG_PX = 1632.0;
    inline constexpr double X_IMG_PX = 260.0;
    inline constexpr double Y_SEN_MM = 2.7;
    inline constexpr double X_SEN_MM = 3.6;
    inline constexpr double Z_M = 5;        // Dummy distance from the ball to ball radar
    inline constexpr double F_LEN_MM = 4.8; // ESP-32 cam default focal length (mm)
    inline constexpr double Y_OFFSET = 1.0; // Distance of ball centre from ground (m) -- needs calibration
    inline constexpr double X_R_OFFSET = 0; // Dummy radar offset for camera radar module
    inline constexpr double Y_R_OFFSET = 0.05;

    inline constexpr double FX_PX = F_LEN_MM * X_IMG_PX / X_SEN_MM;
    inline constexpr double FY_PX = FX_PX; // Same for sqaure pixels

}
