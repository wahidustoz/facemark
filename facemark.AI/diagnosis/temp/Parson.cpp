/*
함수들은 모두 프로토타입임.
멤버들의 접근은 임시로 모두 public으로 설정
*/

//#include "json/json.h" //jsoncpp 사용예정
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
using namespace std;
const double EPS = 1e-8; // EPSILON
const double PI = acos(-1);

class POINT {
public:
    double x, y;
    POINT(double x_=0, double y_=0) :x(x_), y(y_){}
    bool operator==(const POINT& pt);
    bool operator<(const POINT& pt);
    POINT operator+(const POINT& pt);
    POINT operator-(const POINT& pt);
    POINT operator*(double pt);
    double norm();
    double dot(const POINT& pt);
    double cross(const POINT& pt);
};

class ConvexHull{
public:
    vector<POINT> &input;
    vector<POINT> &ouput;
    ConvexHull();
    void setPoint(POINT &pt);
    void calc();
};

class Vector2{ // Vector 2D: 헤더에 들어갈 메소드
private:
    ConvexHull* cb = new ConvexHull;
public:
    int ccw(POINT p, POINT a, POINT b); // 방향만 출력(-1:반시계, 0:평행, 1:시계)
    double ccw2(POINT p, POINT a, POINT b); // 넓이 출력(-,+)
    double pointToPoint(POINT a, POINT b); // 점과 점사이거리
    double intervalAngle(POINT a, POINT b); // 두 벡터의 사이각(rad) (각도: 180/PI 곱)
    bool lineIntersection(POINT a, POINT b, POINT c, POINT d, POINT& x); // 두 직선 교차여부 판별. (true:교차, false:평행)
    double area(const vector<POINT>&p); // 다각형 p의 넓이를 구함.
    bool paralleSegments(POINT a, POINT b, POINT c, POINT d, POINT& p); // 평행한 두 직선이 교차하는지 판단
    bool inBoundingRectangle(POINT p, POINT a, POINT b); // 선분(ab)위 점(p)인지 확인
    bool segmentIntersection(POINT a, POINT b, POINT c, POINT d, POINT& p); // 두직선이 겹치거나 교차하는지 판단. if(true) set 교점p
    int segmentIntersects(POINT a, POINT b, POINT c, POINT d); // 두 직선 교차여부만 판별(교차x, 교차o, 무수히 많은 점에서 교차)
    double areaIntersects(POINT &a, POINT &b){ // 두 면적이 겹치는 부분 계산
        cb->setPoint(a);
        cb->calc();
        cb->setPoint(b);
        cb->calc();
        // 함수 구현 미완
    }
};

class Parson{
public:
    string landmarkjson;
    Parson(string lj);
    Landmark* read(){
        Landmark* lm = new Landmark;
        lm->setMark();
        lm->divideMark();
        return lm;
    }
    string write(Diagnosis* data); // 의존관계
};

class Diagnosis{ // 계산 결과data를 가지고 있는 진단 클래스
public:
    double allAsymmetryRate;
    double eyeAsymmetryRate;
    double noseAsymmetryRate;
    double mouthAsymmetryRate;
    double jawAsymmetryRate;
    Diagnosis(Feature* ft[4]); // Length, Angle, Area, Line
    void calResult(); // 생성자에서 수행
};

class Landmark{
public:
    POINT all[68];
    POINT rEyebrow[5];
    POINT lEyebrow[5];
    POINT rEye[6];
    POINT lEye[6];
    POINT nose[9];
    POINT mouthOut[12];
    POINT mouthIn[8]; // No use
    POINT jaw[17];

    POINT leftFace[39];
    POINT rightFace[39];
    
    void setMark(); // Landmark좌표 초기화
    void divideMark();
};

class Figure{
public:
    double stdDev; // standard deviation
    double variance;
    double mean;
    double mode;
    double median;
    double percentile;
    double skewness;
    
    void calValues(double (&diff)[100000]);
};

class Feature{ // 추상클래스
public:
    Landmark* lm;
    Vector2* v2d;
    Figure* fg = new Figure;
    double diff[100000]{};
    
    Feature(Landmark* lmIn, Vector2* v2dIn);
    virtual void calDiff() = 0; // 추상메소드: 각feature마다 필요한 계산 수행
    virtual void calFig() = 0;  // 추상메소드: diff배열에 저장된 차이값들에 대한 통계수치 계산
};

// 아래의 Angle~Line클래스는 Feature와 일반화 관계
class Length : public Feature{
public:
    Length(Landmark* lmIn, Vector2* v2d);  // 생성자: 부모클래스 생성자 호출
    virtual void calDiff(){ // 오버라이딩: 39C2 계산 후 diff에 저장
        // 사용예시
        double length = v2d->pointToPoint(lm->leftFace[0], lm->leftFace[1]);
    }
    virtual void calFig(){ // 오버라이딩: 통계수치 계산
        //사용예시
        fg->calValues(diff);
    }
};

class Angle : public Feature{
public:
    Angle(Landmark* lmIn, Vector2* v2d);   // 생성자: 부모클래스 생성자 호출
    virtual void calDiff(){ // 오버라이딩: 39C3 계산 후 diff에 저장
        double angle = v2d->intervalAngle(lm->leftFace[1]-lm->leftFace[0], lm->leftFace[1]-lm->leftFace[2]);
    }
    virtual void calFig(){ // 오버라이딩: 통계수치 계산
        //사용예시
        fg->calValues(diff);
    }
};

class Area : public Feature{
public:
    double eyeDiff;
    double noseDiff;
    double mouthDiff;
    double jawDiff;

    Area(Landmark* lmIn, Vector2* v2d);    // 생성자: 부모클래스 생성자 호출
    virtual void calDiff(){ // 오버라이딩: 39C4 계산 후 diff에 저장
        //double area = v2d->areaIntersects(lm->lEye, lm->rEye);
    }
    virtual void calFig(){ // 오버라이딩: 통계수치 계산
        //사용예시
        fg->calValues(diff);
    }
    void calFaceByPart(){ // 얼굴 부위별 면적차 계산
        double angle = v2d->intervalAngle(lm->leftFace[1]-lm->leftFace[0], lm->leftFace[1]-lm->leftFace[2]);
    }
};

class Line : public Feature{
public:
    POINT verti[8]{};
    Line(Landmark* lmIn, Vector2* v2d);    // 생성자: 부모클래스 생성자 호출, setVerticalPOINT() 호출
    virtual void calDiff(){ // 오버라이딩: 39C1, 8C
        double angle = v2d->intervalAngle(lm->leftFace[1]-lm->leftFace[0], lm->leftFace[1]-lm->leftFace[2]);
    }
    virtual void calFig(){ // 오버라이딩: 통계수치 계산
        //사용예시
        fg->calValues(diff);
    }
    void setVerticalPOINT(); // verti배열 초기화: (27 28 29 30 33 51 57 8)점의 좌표 초기화
};

int main() {
    Parson* ps = new Parson("landmark data");
    Landmark* lm = ps->read();
    Vector2* v2d = new Vector2;

    Feature* ft[4];

    ft[0] = new Length(lm,v2d);
    ft[1] = new Angle(lm,v2d);
    ft[2] = new Area(lm,v2d);
    ft[3] = new Line(lm,v2d);

    for(int i=0; i<4; i++) {
        ft[i]->calDiff();
        ft[i]->calFig();
    }

    Diagnosis* result = new Diagnosis(ft);
    string jsonResult =  ps->write(result); // 결과
}