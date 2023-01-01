#include "Parameter.hpp"
#include <math.h>
#include "matplotlib.hpp"

using namespace std;

KarumanFilter<2> kf(filterParam);

int main(){
    kf.recordObservedPos(3.17636,-2.21138);
    kf.recordObservedPos(3.15423, -2.1312);
    kf.recordObservedPos(3.18159,-2.1528);
    kf.recordObservedPos(3.1567 , -2.1404);
    kf.recordObservedPos(3.15265,-2.21138);
    kf.recordObservedPos(3.16303,-2.19897);
    kf.recordObservedPos(3.15947,-2.21138);
    kf.recordObservedPos(3.15423,-2.18025);
    kf.recordObservedPos(3.17636,-2.16606);
    kf.recordObservedPos(3.15423,-2.1462);
    kf.showObservedData();
    kf.calcKarumanFiltering();
    auto pos = kf.getTargetPos();
    cerr << "Filterd Pos: " << pos.x << ", " << pos.y << endl;
    cout << 100*sqrt(fabs((pos.x-3.15)*(pos.x-3.15) + (pos.y-(-2.18))*(pos.y-(-2.18)))) <<"[cm]"<< endl;

    matplotlib g;
	g.open();
    int cnt=0;
    for(auto x : kf.x){
        g.point(x, cnt); 
        cnt++;
        cout << x << endl;
    }
    cnt=0;
    for(auto z : kf.z){
        g.point(z.x, cnt); 
        cnt++;
    }
    g.save("test.pdf");
    return 0;
};

