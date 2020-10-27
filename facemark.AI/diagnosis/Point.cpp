#include <iostream>
#include <math.h>
#include <vector>
#include <memory.h>
using namespace std;
const double EPS = 1e-8; // EPSILON
const double PI = acos(-1);

struct Point {
    double x, y;
    Point(double x_=0, double y_=0) :x(x_), y(y_){}
    bool operator==(const Point& pt) const {return x == pt.x && y == pt.y;}
    bool operator<(const Point& pt) const {return x != pt.x ? x < pt.x : y < pt.y;}
    Point operator+(const Point& pt) const {return Point(x + pt.x, y + pt.y);}
    Point operator-(const Point& pt) const {return Point(x - pt.x, y - pt.y);}
    Point operator*(double pt) const {return Point(x * pt, y * pt);}
    double norm() const { return hypot(x,y); } //벡터길이 반환(점-점 거리)
    Point normalize() const {return Point(x/norm(), y/norm());} //단위벡터 반환, 영벡터입력X
    double dot(const Point& pt) const {return x * pt.x + y * pt.y;}
    double cross(const Point& pt) const {return x * pt.y - pt.x * y;}
};

class Vector2{
public:
    int ccw(Point p, Point a, Point b); // 방향만 출력(-1:반시계, 0:평행, 1:시계)
    double ccw2(Point p, Point a, Point b); // 넓이 출력(-,+)
    double intervalAngle(Point a, Point b); // 두 벡터의 사이각(rad) (각도: 180/PI 곱)
    bool lineIntersection(Point a, Point b, Point c, Point d, Point& x); // 두 직선 교차여부 판별. (true:교차, false:평행)
    double area(const vector<Point>& poly); // 다각형 p의 넓이를 구함.
    bool paralleSegments(Point a, Point b, Point c, Point d, Point& p); // 평행한 두 직선이 교차하는지 판단
    bool inBoundingRectangle(Point p, Point a, Point b); // 선분(ab)위 점(p)인지 확인
    bool segmentIntersection(Point a, Point b, Point c, Point d, Point& p); // 두직선이 겹치거나 교차하는지 판단. if(true) set 교점p
    int segmentIntersects(Point a, Point b, Point c, Point d); // 두 직선 교차여부만 판별(교차x, 교차o, 무수히 많은 점에서 교차)
    vector<Point> convexHull(vector<Point>& poly); // 볼록 다각형 반환. 컨벡스 헐(그라함 스캔 O(NlogN))이용
    double areaIntersects(Point &a, Point &b); // 두 면적이 겹치는 부분 계산
};

// a가 b보다 p에 얼마나 가까운지
double howMuchCloser(Point p, Point a, Point b){
    return (b-p).norm() - (a-p).norm();
}

// 반환을 0,1,-1로 안해주면 밑의 다른함수에서 ccw()*ccw()계산이 4제곱이기 때문에 오버플로우 발생가능
int ccw(Point p, Point a, Point b) {
    double ret = (a-p).cross(b-p);
    if (fabs(ret) < EPS) return 0;
    else if(ret < 0) return -1;
    else return 1;
}
// 반환범위 오버플로우 생각해야 함
double ccw2(Point p, Point a, Point b) { // 면적계산용( BOJ_2166 참고 )
    double ret = (a-p).cross(b-p);
    return ret;
}

//두 벡터의 사이각(rad) *180/PI 해줘야 각도 나옴
double intervalAngle(Point a, Point b) {
	return acos(a.dot(b) / (a.norm()*b.norm()));
}

//단순 다각형 p의 넓이를 구한다.
double area(const vector<Point>&p){
    double ret=0;
    for(int i=0;i<p.size();i++) {
        int j=(i+1)%p.size();
        ret+=p[i].cross(p[j]);
    }
    return fabs(ret)/2.0;
}

// 두 직선이 평행할때(기울기만 평행, 같은직선) false, 교차시 true리턴
bool lineIntersection(Point a, Point b, Point c, Point d, Point& x) {
    double det = (b - a).cross(d - c);
    if (fabs(det) < EPS) return false;
    x = a + (b - a) * ((c - a).cross(d - c) / det);
    return true;
}

// 평행한 두 직선이 input, 교차판단
bool paralleSegments(Point a, Point b, Point c, Point d, Point& p) {
    if (b < a) swap(a, b); // 벡터ab 방향이 ba라면 ab로 바꿔줌
    if (d < c) swap(c, d); // 벡터cd 방향이 dc라면 cd로 바꿔줌
    // 같은 직선에 있지않으면 ccw에 의해 false, 같은 직선위지만 선분ab,cd가 최소 한 점에서 교차하지 않으면 false
    if (ccw(a, b, c) != 0 || b < c || d < a) return false;
    if (a < c) p = c;
    else p = a;
    return true;
}

// 선분(ab)위 점(p)인지 확인
bool inBoundingRectangle(Point p, Point a, Point b) {
    if (b < a) swap(a, b);
    return p == a || p == b || (a < p && p < b);
}

bool segmentIntersection(Point a, Point b, Point c, Point d, Point& p) {
    if (!lineIntersection(a, b, c, d, p))
        return paralleSegments(a, b, c, d, p);
    return inBoundingRectangle(p, a, b) && inBoundingRectangle(p, c, d);
}

// 교차여부만 판별
int segmentIntersects(Point a, Point b, Point c, Point d) {
    if (ccw(a, b, c) == 0 && ccw(a, b, d) == 0) { // 두선분 모두 한 직선위 일때
        if (b < a) swap(a, b);
        if (d < c) swap(c, d);
        if (b < c || d < a)  return 0; // 교차하지 않을때
        else if (b == c || d == a) // 한점에서 교차할때
            return 1;
        else return 2; // 선분교차 - 무수히 많은점 교차
    } else { // 한직선 위가 아닐때
        int ab = ccw(a, b, c) * ccw(a, b, d); // 직선ab와 선분cd 교차 확인시 ab만 사용
        int cd = ccw(c, d, a) * ccw(c, d, b);
        return ab <= 0 && cd <= 0;
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    Point a = Point(0,0);
    Point b = Point(3,4);
    Point c = a-b;
    cout<<c.norm();
}