#pragma once
#include <iostream>
#include <vector>
#include <utility>

template<int N>
class KarumanFilter {

public:     
    struct Gaussian {
        float mean, var;
        Gaussian(const float &mean, const float &var): mean(mean), var(var){};
        Gaussian() : mean(0.f), var(0.f){};
    };

    template<class T>
    struct Elem {
        T x, y; 
        Elem(T x, T y) : x(x), y(y){};
        Elem() {
            x=T(x), y=T(y);
        };
    };

    struct Parameter {
        float priorX[N];
        float priorY[N];
        float process[N];
    };

    Parameter param;
    Elem<Gaussian> prior;
    Gaussian process;

private:
    std::vector<Elem<float>> elem;

public:
    KarumanFilter(const Parameter &parameter) :param(parameter)
    {
        auto g1=Gaussian(param.priorX[0], param.priorX[1]);
        auto g2=Gaussian(param.priorY[0], param.priorY[1]);
        auto p = Gaussian(param.process[0],  param.process[1]);
        prior=Elem<Gaussian>(
            g1,g2
        );
        process=p;
    };
public:
    inline Gaussian predict(const Gaussian &Elem, const Gaussian &movement){
        return Gaussian(Elem.mean + movement.mean, Elem.var + movement.var);
    };
    inline void makeElemData(float x, float y){
        elem.emplace_back(Elem<float>(x,y));
    };
    inline void showElemData(){
        for(auto [x,y] : elem){
            std::cout << x << ", " << y << std::endl;
        }
    }
};
