#include "Karuman.hpp"

constexpr KarumanFilter<2>::Parameter filterParam = {
    .priorX = {25.f, 1000.f},
    .priorY = {25.f, 1000.f},
    .process = {0.f, 0.05f},
};