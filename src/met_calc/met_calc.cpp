#include "met_calc.hpp"

/*
    Use camera configuration to get focal length
    Use initial radar reading to get the approximate reading
    (Assume no 3D motion yet, the size of the ball/ streak changes size --> movement along z dir, need to handle later)
*/

// Prod consts
constexpr double G_FIELD_CONST{9.80665};

// Dev consts

// Dummy consts
constexpr double BALL_MASS = 0.057; // In kg
constexpr double F_LEN = 1;
constexpr double Y_OFFSET = 1;

/*
    Need to take all the states from the kalman filter and make them relative to point of origin
    pt of origin --> y -> distance of centre of ball from ground
                     x -> distance of centre of ball from centre of frame
*/

void convert_ball_states_metric(Ball_states& b_states){

}

/*  Calc drag coefficent with least sqaures.  k = Σ(-a_i * v_i²) / Σ(v_i⁴)
    Check negative sign, v > 0 while a < 0 and k > 0
    k is a constant so it should be the same for both the x and y directions.
*/ 

double get_drag_comp(const Ball_states& ball_states, double dt) {
    double numerator_sum{0}; // Σ(-a_i * v_i²)
    double denominator_sum{0}; // Σ(v_i⁴)

    for (auto b = ball_states.begin(); b != ball_states.end(); ++b){
        if (std::next(b) == ball_states.end()) break;
        auto& curr_b = *b;
        auto& next_b = *std::next(b);
 
        double a_x = (next_b.x_v - curr_b.x_v)/dt; 
        double a_y = (next_b.y_v - curr_b.y_v)/dt - G_FIELD_CONST;

        double a = std::sqrt((a_x * a_x) + (a_y * a_y));

        // Using current v
        double v = std::sqrt((curr_b.x_v * curr_b.x_v) + (curr_b.y_v * curr_b.y_v));

        double v2 = v * v;
        double v4 = v2 * v2;
        
        // Σ(-a_i * v_i²)
        numerator_sum += (-a) * v2;
        denominator_sum += v4;
    }

    return numerator_sum/denominator_sum;

}

/**
 * @brief Euler's method iteration, use last state from kalman filter
 * @param ball_states Ball States returned from kalman filter converted to metric
 * @param dt Being negative will extrapolate in -ve x direction
 */

ball_disp_t get_max_disp(const Ball_states& b_states, double m, double k, double dt, double y_offset){
    auto& b = b_states.back();

    double t_total{0};

    double v_x{b.x_v};
    double v_y{b.y_v};

    double max_disp_y{y_offset};
    
    double disp_x{0};
    double disp_y{y_offset};

    while (disp_y > 0) {
        // v -> Magnitude of total velocity.
        double v = std::sqrt((v_x * v_x) + (v_y * v_y));

        double a_x = -1/m * k * v * v_x;
        v_x = v_x + a_x * dt;
        disp_x = disp_x + v_x * dt;

        double a_y = -1/m * k * v * v_y - G_FIELD_CONST;
        v_y = v_y + a_y * dt;
        disp_y = disp_y + v_y * dt;

        if (max_disp_y < disp_y) max_disp_y = disp_y;
        t_total += dt;
    }

    return ball_disp_t{disp_x, max_disp_y};
}

void print_ball_max_disp(const ball_disp_t& b){
    std::cout << "Ball Displacement: Max X Distance: " << b.x << "m" 
    << "Max Y Distance: " << b.y << "m" << '\n';
}

/**
 * @brief Gets the max displacemnt of the ball form last state of the ball (centre of ball) captured.
 * 
 */
ball_disp_t cal_met(Ball_states& b_states){
    convert_ball_states_metric(b_states);
    double k = get_drag_comp(b_states, C::T_STEP_FRAME);

    auto ball_disp = get_max_disp(b_states, BALL_MASS, k, C::T_STEP, Y_OFFSET);
    print_ball_max_disp(ball_disp);
    return ball_disp;
}


