#include <bits/stdc++.h>
using namespace std;
const double EPSILON = 1e-9;
const double PI = acos(-1);

struct vector2 {
    double x, y;
    vector2(double x_=0, double y_=0) :x(x_), y(y_){} 
    bool operator==(const vector2& jcu) const {
        return x == jcu.x && y == jcu.y;
    }
    bool operator<(const vector2& jcu) const {
        return x != jcu.x ? x < jcu.x : y < jcu.y;
    }
    vector2 operator+(const vector2& jcu) const {
        return vector2(x + jcu.x, y + jcu.y);
    }
    vector2 operator-(const vector2& jcu) const {
        return vector2(x - jcu.x, y - jcu.y);
    }
    vector2 operator*(double jcu) const {
        return vector2(x * jcu, y * jcu);
    }
    double norm() const { return hypot(x,y); }
    double dot(const vector2& jcu) const {
        return x * jcu.x + y * jcu.y;
    }
    double cross(const vector2& jcu) const {
        return x * jcu.y - jcu.x * y;
    }
};

double ccw(vector2 p, vector2 a, vector2 b) {
    double ret = (a-p).cross(b-p);
    return ret;
}

double area(const vector<vector2>&p){
    double ret=0;
    for(int i=0;i<p.size();i++) {
        int j=(i+1)%p.size();
        ret+=p[i].cross(p[j]);
    }
    return fabs(ret)/2.0;
}

//두 벡터의 사이각(rad) *180/PI 해줘야 각도 나옴
double intervalAngle(vector2 a, vector2 b) {
	return acos(a.dot(b) / (a.norm()*b.norm()));
}

int N;
vector<vector2> v;

int main() {
    cin >> N; // 점 개수

    for(int i=0; i<N; i++){
        double a,b; cin>>a>>b;
        v.emplace_back(vector2(a,b)); // 점 좌표 저장
    }

    double res=area(v);
    cout<<res<<'\n';

    // 두 벡터 사이 각 출력 예시
    vector2 a,b;
    a = vector2(1,1);
    b = vector2(1,0);
    cout<<intervalAngle(a,b)*180/PI;
}
