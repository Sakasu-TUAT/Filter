#pragma once
#include <iostream>
#include <vector>
#include <utility>

template<int N>
class KarumanFilter {

public:     
    //x~N(μ, σ^2)
    struct Gaussian {
        float mean, var;
        Gaussian(const float &mean, const float &var): mean(mean), var(var){};
        Gaussian() : mean(0.f), var(0.f){};
        float *begin() {return &this->mean;};
        float *end() {return &this->var+1;};
    };

    //x,y扱う用
    template<class T>
    struct Pos {
        T x, y; 
        Pos(T x, T y) : x(x), y(y){};
        Pos() {
            x=T(x), y=T(y);
        };
        T *begin() {return &this->x;};
        T *end() {return &this->y+1;};
    };

    struct Parameter {
        float priorX[N];
        float priorY[N];
        float process[N];
        float likelihoodX[N];
        float likelihoodY[N];
    };

    Parameter param; //パラメータ
    Gaussian process; //モデルの分布..点群の分散
    Pos<Gaussian> prior; //事前分布:=推定値k-1 + 運動モデルの分布
    Pos<Gaussian> posterior; //事後分布(推定値k) := 事前分布*尤度
    Pos<Gaussian> likelihood; //尤度 := LiDARによる現在の観測値の分布
    float K; //カルマンゲイン
    std::vector<float> x;
    //LiDARの観測値Z=(x,y)を保持
    std::vector<Pos<float>> z;

private:


public:
    KarumanFilter(const Parameter &parameter):
        param(parameter),
        prior(Gaussian(param.priorX[0], param.priorX[1]), Gaussian(param.priorY[0], param.priorY[1])),
        process(param.process[0], param.process[1]),
        likelihood(
            Gaussian(param.likelihoodX[0], param.likelihoodX[1]),
            Gaussian(param.likelihoodY[0], param.likelihoodY[1])
        )
    {};
public:
    //事前推定の計算
    inline Pos<Gaussian> predict(){
        Pos<Gaussian> p
        (   
            Gaussian(posterior.x.mean + process.mean, posterior.x.var + process.var),
            Gaussian(posterior.y.mean + process.mean, posterior.y.var + process.var)
        );

        return p;
    };
    //二つのガウス分布の積の計算
    inline Gaussian gaussianMultiply(Gaussian g1, Gaussian g2){
        float mean = (g1.var * g2.mean + g2.var * g1.mean) / (g1.var + g2.var);
        float variance = (g1.var * g2.var) / (g1.var + g2.var);
        return Gaussian(mean, variance);
    }

    //事前分布と尤度から事後分布の計算
    inline Pos<float> update(Pos<float> lidarPos){
        likelihood.x.mean = lidarPos.x;
        likelihood.y.mean = lidarPos.y;
        posterior.x = gaussianMultiply(prior.x, likelihood.x);
        posterior.y = gaussianMultiply(prior.y, likelihood.y);
        return Pos<float>(posterior.x.mean, posterior.y.mean);
    }
    inline Pos<float> getTargetPos(){
        return Pos<float>(posterior.x.mean, posterior.y.mean);
    }
    inline void calcKarumanFiltering(){
        posterior.x.mean = z[0].x; //初期値を最初の観測値にしておくと良い 
        posterior.y.mean = z[0].y;
        x.emplace_back(posterior.x.mean);
        for (const auto &lidarPos : z){
            prior = predict(); //前の事後分布と点群の分散から事前分布を計算
            update(lidarPos); //事前分布と尤度の積で事後分布を更新
        }
        // clearObservedPos();
    }
    //LiDARで得た点のx,yを保持
    inline void recordObservedPos(float x, float y){
        z.emplace_back(Pos<float>(x,y));
    };
    inline void clearObservedPos(){
        z.clear();
    };
    inline void showObservedData(){
        for(const auto &[x,y] : z){
            std::cout << x << ", " << y << std::endl;
        }
    }
};
