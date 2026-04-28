#include "../include/met_calc.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>

#include "constants/physics_consts.hpp"
#include "config/tuning_params.hpp"
#include "config/hardware_config.hpp"

/*  Calc drag coefficent with least sqaures.  k = Σ(-a_i * v_i²) / Σ(v_i⁴), where k >= 0; k ~ K/mass
    Ensure that signs are retained as per convention that upwards and right is positive
    k is a constant so it should be the same for both the x and y directions.
*/

double getDragComp(const BallStates &ball_states, double dt)
{
    double numerator_sum{0};   // Σ(-a_i * v_i²)
    double denominator_sum{0}; // Σ(v_i⁴)

    for (auto b = ball_states.begin(); b != ball_states.end(); ++b)
    {
        if (std::next(b) == ball_states.end())
            break;
        auto &curr_b = *b;
        auto &next_b = *std::next(b);

        if (std::abs(next_b.x_v) >= std::abs(curr_b.x_v))
        {
            std::cout << "Warning: Ball state discarded as v_x is not decreasing.";
            continue;
        }

        bool is_curr_pos = (curr_b.x_v > 0);
        bool is_next_pos = (next_b.x_v > 0);

        if (is_curr_pos != is_next_pos)
        {
            {
                throw std::runtime_error("Error: Ball state measurement is indicating ball is moving in opp directions between frames");
            }
        }

        double a_x = (next_b.x_v - curr_b.x_v) / dt;
        // double a_y = (next_b.y_v - curr_b.y_v) / dt - Config::Physics::kGFieldConst;

        // Using current v_x
        double vx_2 = (curr_b.x_v * curr_b.x_v);

        double vx_4 = vx_2 * vx_2;

        // Σ(-a_i * v_i²), TODO: Error here.
        numerator_sum += (-a_x * (is_curr_pos ? 1 : -1)) * vx_2;
        denominator_sum += vx_4;
    }

    if (denominator_sum == 0)
        throw std::runtime_error("Error: Divide by zero error. All ball states discarded");

    double k = numerator_sum / denominator_sum;
    if (k < 0)
        throw std::runtime_error("Unable to compute drag coeff that is > 0");

    return k;
}

/**
 * @brief Euler's method iteration, use last state from kalman filter
 * @param ball_states Ball States returned from kalman filter converted to metric
 * @param dt Being negative will extrapolate in -ve x direction
 */

BallDisp getMaxDisp(const BallStates &b_states, double k, double dt, double y_offset)
{
    auto &b = b_states.back();

    double t_total{0};

    double v_x{b.x_v};
    double v_y{b.y_v};

    double max_disp_y{y_offset};

    double disp_x{0};
    double disp_y{y_offset};

    while (disp_y > 0)
    {
        // v -> Magnitude of total velocity.
        double v = std::sqrt((v_x * v_x) + (v_y * v_y));

        double a_x = -1 / k * v * v_x;
        v_x = v_x + a_x * dt;
        disp_x = disp_x + v_x * dt;

        double a_y = -1 / k * v * v_y - Config::Physics::kGFieldConst;
        v_y = v_y + a_y * dt;
        disp_y = disp_y + v_y * dt;

        if (max_disp_y < disp_y)
            max_disp_y = disp_y;
        t_total += dt;
    }
    std::cout << "Total Flight Duration" << static_cast<int>(t_total);

    return BallDisp{disp_x, max_disp_y};
}

void printBallMaxDisp(const BallDisp &b)
{
    std::cout << "Ball Displacement: Max X Distance: " << b.x << "m"
              << "Max Y Distance: " << b.y << "m" << '\n';
}

/**
 * @brief Gets the max displacemnt of the ball form last state of the ball (centre of ball) captured.
 *
 */
BallDisp MetCalc::calMet(BallStates &b_states)
{
    double k = getDragComp(b_states, Config::Hardware::kTStepFrame);

    auto ball_disp = getMaxDisp(b_states, k, Config::Tuning::kTStep, Config::Hardware::kYOffset);
    printBallMaxDisp(ball_disp);
    return ball_disp;
}
