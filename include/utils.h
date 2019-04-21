#pragma once
#include <cstdlib>

template <typename FloatNumericType> FloatNumericType random(FloatNumericType min, FloatNumericType max) {
    FloatNumericType randomFraction = ((FloatNumericType)rand()) / ((FloatNumericType)RAND_MAX);
    FloatNumericType range = max - min;

    return min + (range * randomFraction);
}

template <> int random(int min, int max) { return rand() % (max - min + 1) + min; }