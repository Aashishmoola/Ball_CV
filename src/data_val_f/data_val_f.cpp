#include "data_val_f.hpp"

/*
    Use camera configuration to get focal length
    Use initial radar reading to get the approximate reading
    (Assume no 3D motion yet, the size of the ball/ streak changes size --> movement along z dir, need to handle later)
*/

/// @param x_val from top-left of camera frame in px
/// @param y_val from top-right of camera frame in px
/// @return pos of centre of ball from centre of frame in m
void px_to_m_cam(double x_px, double y_px, Ball_pos &pos_state)
{
    double z_cam = Radar::get_Z_cam(Config::Hardware::Z_M, Config::Hardware::X_R_OFFSET, Config::Hardware::Y_R_OFFSET);

    double scale_x = z_cam / Config::Hardware::FX_PX;
    double scale_y = z_cam / Config::Hardware::FY_PX;

    double x_m = (x_px - Config::Hardware::X_IMG_PX / 2) * scale_x;
    double y_m = (y_px - Config::Hardware::Y_IMG_PX / 2) * scale_y + Config::Hardware::Y_OFFSET;

    pos_state(x_m, y_m);
}

void Data_val_f::circles_to_balls_pos(const Circles &circles, Balls_pos &balls_pos)
{
    for (const auto &circle : circles)
    {
        Ball_pos pos;
        px_to_m_cam(circle[0], circle[1], pos);
        balls_pos.push_back(pos);
    }
}

void Data_val_f::balls_pos_to_state(const Balls_pos &balls_pos, Ball_states states)
{
    for (auto it = balls_pos.begin(); it != balls_pos.end() - 1; it++)
    {
        auto curr = *it;
        auto next = *(it + 1);

        auto new_v_x = (next(0, 0) - curr(0, 0)) / Config::Hardware::T_STEP_FRAME;
        auto new_v_y = (next(0, 1) - curr(0, 1)) / Config::Hardware::T_STEP_FRAME;

        // Access with col, row syntax for 2D matrixes
        ball_state_t new_state{next(0, 0), next(0, 1), new_v_x, new_v_y};
        states.push_back(new_state);
    }
}

void Data_val_f::run_k_filter(Balls_pos &balls_pos)
{
    K_fil_2D kf{Config::Hardware::T_STEP_FRAME, Config::Tuning::P_NOISE, Config::Tuning::M_NOISE, Config::Tuning::M_NOISE, Config::Physics::G_FIELD_CONST};

    for (auto &pos : balls_pos)
    {
        Ball_pos predict_pos = kf.predict();
        Ball_pos derived_pos;
        // TODO: Should be a check if the ball has been detected and kalman filter has converged
        if (true)
            pos = kf.update(predict_pos);
        else
            pos = predict_pos;
    }
}
