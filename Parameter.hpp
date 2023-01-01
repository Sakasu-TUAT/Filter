#include "Karuman.hpp"

constexpr KarumanFilter<2>::Parameter filterParam = {
    .priorX = {25.f, 1000.f}, //実際は最初に観測した点の座標に置き換える
    .priorY = {25.f, 1000.f}, //実際は最初に観測した点の座標に置き換える
    .process = {0.f, 0.05f * 0.05f}, //点群の振る舞い..ほぼ動かず標準偏差0.05m
    .likelihoodX = {0.f, 0.015f * 0.015f}, //観測した点群の誤差(0.03m)/2を標準偏差にした
    .likelihoodY = {0.f, 0.015f * 0.015f} //観測した点群の誤差(0.03m)/2を標準偏差にした
};