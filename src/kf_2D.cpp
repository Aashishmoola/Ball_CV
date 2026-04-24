#include "../include/kf_2D.hpp"

// Give the true state of the position and velocity of the ball even when there is noisy measured data(ball detection errors) and process noise (acceleration --> del v)
// Converts form circle(returned from ball det) -> ball_pos_state (used by the filter itself) -> ball_state (used in met_calc)

/**
 * @param q Process Noise
 * @param m_x Measured Noise; x_dir
 * @param m_y Meaured Noise; y_dir
 */
K_fil_2D::K_fil_2D(double dt, double q, double m_x, double m_y, double g)
{

    // Initialise F, Q, H
    F_ = {1, 0, dt, 0,
          0, 1, 0, dt,
          0, 0, 1, 0,
          0, 0, 0, 1};

    double dt2 = std::pow(dt, 2);
    double dt3 = std::pow(dt, 3);
    double dt4 = std::pow(dt, 4);

    cv::Matx44d Q_unweighted = {
        dt4 / 4,
        0,
        dt3 / 2,
        0,
        0,
        dt4 / 4,
        0,
        dt3 / 2,
        dt3 / 2,
        0,
        dt2,
        0,
        0,
        dt3 / 2,
        0,
        dt2,
    };

    Q_ = q * Q_unweighted;

    H_ = {1, 0, 0, 0,
          0, 1, 0, 0};

    R_ = {m_x * m_x, 0,
          0, m_y * m_y};

    P_ = cv::Matx44d::eye() * 1000.0; // .eye() -> creates identity matrix

    x_ = {0, 0, 0, 0};

    cv::Matx41d B_unweighted = {0, -dt2 / 2, 0, -dt};
    B_ = B_unweighted * g;
}

cv::Matx21d K_fil_2D::predict()
{
    // State pediction:
    x_ = F_ * x_ + B_;

    // Covariance Prediction: (Transformatiion:  F * P * F_tranform --> Deeper mathematical significance)
    P_ = F_ * P_ * F_.t() + Q_;

    return H_ * x_;
}

/**
 * @param z Current Measured State
 */
cv::Matx21d K_fil_2D::update(const cv::Matx21d &z)
{
    // Getting state innovation (actual measured pos values  - predicted pos values), negate velocity values
    cv::Matx21d y = z - H_ * x_;

    // Covariance prediction (getting the variance of pos values only and adding the measured pos variance values)
    cv::Matx22d S = H_ * P_ * H_.t() + R_;

    // Main kalman formula --> Deep maths
    cv::Matx<double, 4, 2> K = P_ * H_.t() * S.inv();

    // State Update
    x_ = x_ + K * y;

    // Covariance update
    auto I = cv::Matx44d::eye();
    P_ = (I - K * H_) * P_;

    return H_ * x_;
}
