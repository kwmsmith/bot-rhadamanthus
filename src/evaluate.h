#ifndef _EVALUATE_H_
#define _EVALUATE_H_

#include "gamestate.h"
#include "board.h"
#include <cmath>

inline float approx_log(float x)
{
    return log(x);
}

// inline float approx_log(float x)
// {
    // const static float 
        // m0 = 0.25584278811044953,
        // b0 = -0.25584278811044953,
        // m1 = 0.069314718055994498,
        // b1 = 1.6094379124341009,
        // m2 = 0.026435245677401832,
        // b2 = 2.4670273600059542;
    
    // if ( x >= 20.) {
        // return m2 * x + b2;
    // } else if (x >= 10.) {
        // return m1 * x + b1;
    // } else {
        // return m0 * x + b0;
    // }
// }

float evaluate(const GameState& gs);

float eval_material(const GameState& gs);

#endif
