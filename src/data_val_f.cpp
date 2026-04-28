#include "../include/data_val_f.hpp"

#include "../include/types/ball_state.hpp"

#include "../include/kf_2d.hpp"
#include "../include/config/hardware_config.hpp"
#include "../include/config/tuning_params.hpp"
#include "../include/constants/physics_consts.hpp"
#include "../include/radar.hpp"

/*
    Use camera configuration to get focal length
    Use initial radar reading to get the approximate reading
    (Assume no 3D motion yet, the size of the ball/ streak changes size -->
   movement along z dir, need to handle later)
*/

/// @param x_val from top-left of camera frame in px
/// @param y_val from top-right of camera frame in px
/// @return pos of centre of ball from centre of frame in m
BallPos pxToMCam(double x_px, double y_px)
{
    double z_cam =
        Radar::getZCam(Config::Hardware::kZM, Config::Hardware::kXROffset,
                       Config::Hardware::kYROffset);

    double scale_x = z_cam / Config::Hardware::kFxPx;
    double scale_y = z_cam / Config::Hardware::kFyPx;

    double x_m = (x_px - Config::Hardware::kXImgPx / 2) * scale_x;
    double y_m = (Config::Hardware::kYImgPx / 2 - y_px) * scale_y +
                 Config::Hardware::kYOffset;

    BallPos pos(x_m, y_m);
    return pos;
}

void DataValF::circlesToBallsPos(const Circles &circles, BallsPos &balls_pos)
{
    for (const auto &circle : circles)
    {
        balls_pos.push_back(pxToMCam(circle[0], circle[1]));
    }
}

void DataValF::ballsPosToState(const BallsPos &balls_pos, BallStates &states)
{
    if (balls_pos.size() < 2)
        throw std::invalid_argument(" Too few ball pos mat for conversion, must be >2");
    for (auto it = balls_pos.begin(); it != balls_pos.end() - 1; it++)
    {
        auto curr = *it;
        auto next = *(it + 1);

        auto new_v_x = (next(0, 0) - curr(0, 0)) / Config::Hardware::kTStepFrame;
        auto new_v_y = (next(1, 0) - curr(1, 0)) / Config::Hardware::kTStepFrame;

        // Access with col, row syntax for 2D matrixes
        BallState new_state{next(0, 0), next(1, 0), new_v_x, new_v_y};
        states.push_back(new_state);
    }
}

void DataValF::runKFilter(BallsPos &balls_pos)
{
    KFil2D kf{Config::Hardware::kTStepFrame, Config::Tuning::kPNoise, Config::Tuning::kMNoise,
              Config::Tuning::kMNoise, Config::Physics::kGFieldConst};

    for (auto &pos : balls_pos)
    {
        BallPos predict_pos = kf.predict();
        // TODO: Should be a check if the ball has been detected and kalman filter
        // has converged
        if (true)
            pos = kf.update(pos);
        else
            pos = predict_pos;
    }
}
