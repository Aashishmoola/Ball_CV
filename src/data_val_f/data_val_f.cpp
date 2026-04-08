#include "data_val_f.hpp"

// Give the true state of the position and velocity of the ball even when there is noisy measured data(ball detection errors) and process noise (acceleration --> del v)
// Converts form circle(returned from ball det) -> ball_pos_state (used by the filter itself) -> ball_state (used in met_calc)


constexpr double P_NOISE {10}; // Air drag and Magnus effect q~10-50 at 4 frames/ 60fps or 5 - 15 at 12 frames /60 fps
constexpr double M_NOISE {0}; // Need in both x and y dir, based on camera calibration



/**
 * @param q Process Noise
 * @param m_x Measured Noise; x_dir
 * @param m_y Meaured Noise; y_dir
 */
K_fil_2D::K_fil_2D(double dt, double q, double m_x, double m_y, double g){

    // Initialise F, Q, H 
    F_ = {1, 0, dt, 0,
          0, 1, 0, dt,
          0, 0, 1, 0,  
          0, 0, 0, 1};
    
    double dt2 = std::pow(dt, 2);
    double dt3 = std::pow(dt, 3);
    double dt4 = std::pow(dt, 4);

    cv::Matx44d Q_unweighted = {
        dt4/4, 0,     dt3/2, 0,
        0,     dt4/4, 0,     dt3/2,
        dt3/2, 0,     dt2,   0,
        0,     dt3/2, 0,     dt2,
    };

    Q_ = q * Q_unweighted;

    H_ = {1, 0, 0, 0,
          0, 1, 0, 0};

    R_ = {m_x * m_x, 0,
           0, m_y * m_y};

    P_ = cv::Matx44d::eye() * 1000.0; // .eye() -> creates identity matrix 

    x_ = {0, 0, 0, 0};

    cv::Matx41d B_unweighted = {0, -dt2/2, 0, -dt};
    B_ = B_unweighted * g;
}

cv::Matx21d K_fil_2D::predict(){
    // State pediction:
    x_ = F_ * x_ + B_;

    // Covariance Prediction: (Transformatiion:  F * P * F_tranform --> Deeper mathematical significance)
    P_ = F_ * P_ * F_.t() + Q_;

    return H_ * x_;
}

/**
 * @param z Current Measured State
 */
cv::Matx21d K_fil_2D::update(const cv::Matx21d& z){
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

void circles_to_ball_pos_states(const Circles& circles, Ball_pos_states& states){
    for (const auto& circle : circles){
        Ball_pos_state state {circle[0], circle[1]};
        states.push_back(state);
    }
}

void ball_pos_to_state(const Ball_pos_states& pos_states, Ball_states states) {
    for (auto it = pos_states.begin(); it != pos_states.end()-1; it++){
        auto curr = *it;
        auto next = *(it+1);

        auto new_v_x = (next(0, 0) - curr(0, 0)) / C::T_STEP_FRAME;
        auto new_v_y = (next(0, 1) - curr(0, 1)) / C::T_STEP_FRAME;

        // Access with col, row syntax for 2D matrixes
        ball_state_t new_state{next(0, 0), next(0, 1), new_v_x, new_v_y, true};
        states.push_back(new_state);
    }
}


void run_k_filter(const Circles& circles, Ball_states& ball_states){
    Ball_pos_states pos_states;
    Ball_states states;

    circles_to_ball_pos_states(circles, pos_states);

    /*
        predict()

        if ball is det:
            update(meas)
            push update result
        else :
            push predict result
    */

}



