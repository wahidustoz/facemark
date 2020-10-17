// 벡터 C++ 코드

#include <math.h>
#include <algorithm>
#include <vector>
using namespace std;
#define xx first
#define yy second
const double EPSILON = 1e-8;
const double PI = acos(-1);

struct POINT {
    double x, y;
    POINT(double x_=0, double y_=0) :x(x_), y(y_){}
    bool operator==(const POINT& jcu) const {
        return x == jcu.x && y == jcu.y;
    }
    bool operator<(const POINT& jcu) const {
        return x != jcu.x ? x < jcu.x : y < jcu.y;
    }
    
    POINT operator+(const POINT& jcu) const {
        return POINT(x + jcu.x, y + jcu.y);
    }
    POINT operator-(const POINT& jcu) const {
        return POINT(x - jcu.x, y - jcu.y);
    }
    POINT operator*(double jcu) const {
        return POINT(x * jcu, y * jcu);
    }
    //벡터길이 반환
    double norm() const { return hypot(x,y); }
    // 내적,외적
    double dot(const POINT& jcu) const {
        return x * jcu.x + y * jcu.y;
    }
    double cross(const POINT& jcu) const {
        return x * jcu.y - jcu.x * y;
    }

    /********************Temp st**********************/
    //단위벡터 반환, 영벡터에 대해 호출한 경우 반환 값은 정의되지 않음
    POINT normalize() const {
        return POINT(x/norm(), y/norm());
    }
    //이 벡터를 jcu에 사영한 결과
    POINT project(const POINT& jcu) const{
        POINT r = jcu.normalize();
        return r*r.dot(*this);
    }
    // x축의 양의 방향으로부터 이 벡터까지 반시계 방향으로 잰 각도
    double polar() const {return fmod(atan2(y, x)+2*PI, 2*PI); }

    /********************Temp end**********************/
};

// 직선 ab에 대해 점p의 위치 확인 (시계,반시계)
int ccw(POINT p, POINT a, POINT b) {
    double ret = (a-p).cross(b-p);
    if (fabs(ret) < EPSILON) return 0;
    else if(ret < 0) return -1;
    else return 1;
}
// 외적값 그대로 출력 -> 넓이출력
double ccw2(POINT p, POINT a, POINT b) {
    double ret = (a-p).cross(b-p);
    return ret;
}

//두 벡터의 사이각(rad) *180/PI 해줘야 각도 나옴
double intervalAngle(POINT a, POINT b) {
	return acos(a.dot(b) / (a.norm()*b.norm()));
}

// 같은 직선상에 놓인경우(기울기 같은경우) false리턴
bool lineIntersection(POINT a, POINT b, POINT c, POINT d, POINT& x) {
    double det = (b - a).cross(d - c);
    if (fabs(det) < EPSILON) return false;
    x = a + (b - a) * ((c - a).cross(d - c) / det);
    return true;
}

//단순 다각형 p의 넓이를 구한다.
double area(const vector<POINT>&p){
    double ret=0;
    for(int i=0;i<p.size();i++)
    {
        int j=(i+1)%p.size();
        ret+=p[i].cross(p[j]);
    }
    return fabs(ret)/2.0;
}

// lineIntersection결과가 false: 두직선이 평행할때
bool paralleSegments(POINT a, POINT b, POINT c, POINT d, POINT& p) {
    if (b < a) swap(a, b); // 벡터ab 방향이 ba라면 ab로 바꿔줌
    if (d < c) swap(c, d); // 벡터cd 방향이 dc라면 cd로 바꿔줌
    // 평행하지만 같은 직선에 있지않으면 ccw에 의해, 같은 직선위지만 선분ab,cd가 최소 한 점에서 교차하지 않으면 false
    if (ccw(a, b, c) != 0 || b < c || d < a) return false;
    if (a < c) p = c;
    else p = a;
    return true;
}
// 선분(ab)위 점(p)인지 확인
bool inBoundingRectangle(POINT p, POINT a, POINT b) {
    if (b < a) swap(a, b);
    return p == a || p == b || (a < p && p < b);
}

// 선분 ab , 선분cd의 교차점 p판별
bool segmentIntersection(POINT a, POINT b, POINT c, POINT d, POINT& p) {
    if (!lineIntersection(a, b, c, d, p))
        return paralleSegments(a, b, c, d, p);
    return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);
}

// 교차여부만 판별
const int MAX = 4;
int segmentIntersects(POINT a, POINT b, POINT c, POINT d) {
    if (ccw(a, b, c) == 0 && ccw(a, b, d) == 0) { // 두선분 모두 한 직선위 일때
        if (b < a) swap(a, b);
        if (d < c) swap(c, d);
        if (b < c || d < a)  return 0; // 교차하지 않을때
        else if (b == c || d == a) // 한점에서 교차할때
            return 1;
        else return MAX; // 선분교차 - 무수히 많은점 교차
    } else { // 한직선 위가 아닐때
        int ab = ccw(a, b, c) * ccw(a, b, d); // 직선ab와 선분cd 교차 확인시 ab만 사용
        int cd = ccw(c, d, a) * ccw(c, d, b);
        return ab <= 0 && cd <= 0;
    }
}


/*----------------------------------------------------------------------------------*/
// temp func

// a가 b보다 p에 얼마나 가까운지
double howMuchCloser(POINT p, POINT a, POINT b){
    return (b-p).norm() - (a-p).norm();
}

// 점 p에서 (a,b)직선에 내린 수선의 발 구함
POINT perpendicularFoot(POINT p, POINT a, POINT b){
    return a + (p-a).project(b-a);
}
//점p와 (a,b)직선 사이의 거리를 구한다.
double pointToLine(POINT p, POINT a, POINT b) {
    return (p - perpendicularFoot(p,a,b)).norm();
}