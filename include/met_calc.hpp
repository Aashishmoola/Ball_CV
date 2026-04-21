#pragma once

#include "types/ball_state.hpp"
#include "constants/physics_consts.hpp"
#include "config/tuning_params.hpp"
#include "config/hardware_config.hpp"

#include <iostream>
#include <iomanip>
#include <cmath>

namespace Met_calc
{
    ball_disp_t cal_met(Ball_states &b_states);
}