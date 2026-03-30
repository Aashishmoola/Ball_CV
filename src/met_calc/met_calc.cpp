#include "met_calc.hpp"
#include "../helpers/ball_state.hpp"
#include <iostream>
#include <cmath>

struct k_t{
    double k_x;
    double k_y;
};

/*
    Use camera configuration to get focal length
    Use initial radar reading to get the approximate reading
    (Assume no 3D motion yet, the size of the ball/ streak changes size --> movement along z dir, need to handle later)
*/
/**
 * @param d Distance to obj detected from camera focal pt
 * @param p_len Pixel length of obj
 * @param f_len focal length of camera
 */

double pix_to_m(double p_len, double d, double f_len) {
    return p_len * d / f_len;
}


// Calc drag coefficent with least sqaures.  k = Σ(-a_i * v_i²) / Σ(v_i⁴)
// Check negative sign, v > 0 while a < 0 and k > 0
// convert to the m first to account for gravity
k_t get_drag_coef(const Ball_states& ball_states) {
    double top_acc_x{0};
    double bot_acc_x{0};
    double top_acc_y{0};
    double bot_acc_y{0};

    for (auto b = ball_states.begin(); b != ball_states.end(); ++b){
        if (std::next(b) == ball_states.end()) break;
        auto& curr_b = *b;
        auto& next_b = *std::next(b);

        auto a_y = next_b.x_v - curr_b.x_v - gravity; // Use a globally declared const for gravity
        auto a_x = next_b.y_v - curr_b.y_v;

        top_acc_x += (-a_x) * pow(curr_b.x_v, 2);
        top_acc_y += (-a_y) * pow(curr_b.y_v, 2);

        bot_acc_x += pow(curr_b.x_v, 4);
        bot_acc_y += pow(curr_b.y_v, 4);
    }

    return k_t{top_acc_x/bot_acc_x, top_acc_y/bot_acc_y};

}


//  Euler's method iteration



