#pragma once

#include "../helpers/ball_state.hpp"
#include "../helpers/hardware_config.hpp"
#include "../helpers/physics_consts.hpp"
#include "../helpers/tuning_params.hpp"
#include "../radar/radar.hpp"

#include <opencv2/core.hpp>

// FIXME Check is process noise it significant to model drag that is consistent
class K_fil_2D
{
public:
    // Process noise and measued noise needs to be initialised as a high value before model can correct itself
    K_fil_2D(double dt, double process_noise, double meas_noise_x, double meas_noise_y, double g);
    cv::Matx21d predict();                    // Check types
    cv::Matx21d update(const cv::Matx21d &z); // Check types

private:
    // Matrixes to measure state
    cv::Matx41d x_; // state 4*1 for pos and v for 2D
    cv::Matx44d F_; // state transition matrix: maps x(k-1) to x(k), accounting for changes in pos due to velocity

    // Ones for process noise
    cv::Matx44d P_; // Maps covariance (uncertainty) for each var in state to each other, the velocity estimates carry uncertainty that bleeds into position uncertainty through the covariance matrix P --> Takes cov(pos_x, v_x) etc into account
    cv::Matx44d Q_; // Adds process noise (acceleration due to other forces, mainly air resistance)

    // Ones for measured (only pos not velocity) noise
    cv::Matx<double, 2, 4> H_; // Gets only the pos vars for observation
    cv::Matx22d R_;            // Covariance matrix for measured values

    cv::Matx41d B_; // state offset (v_y) matrix : acceleration due to gravity
};