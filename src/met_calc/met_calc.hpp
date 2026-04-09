#pragma once

#include "../helpers/ball_state.hpp"
#include "../helpers/physics_consts.hpp"
#include "../helpers/hardware_config.hpp"

#include <iostream>
#include <cmath>

namespace Met_calc
{
    ball_disp_t cal_met(Ball_states &b_states);
}