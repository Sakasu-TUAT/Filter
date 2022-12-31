#include "Parameter.hpp"

using namespace std;

KarumanFilter<2> kf(filterParam);

int main(){
    kf.makeElemData(13, 23);
    kf.showElemData();
    cout << kf.param.priorX[0] <<", " << kf.process.var<< endl;
    return 0;
};

