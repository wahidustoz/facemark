// N개의 점 이은 면적 계산
// 점은 순서대로 저장되어 있어야 함
// BOJ_2166 다각형의 면적 코드

#include <bits/stdc++.h>
using namespace std;
#define xx first
#define yy second
typedef long long ll;

struct vector2 {
    ll x, y;
    vector2(ll x_=0, ll y_=0) :x(x_), y(y_){} 
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
    vector2 operator*(ll jcu) const {
        return vector2(x * jcu, y * jcu);
    }
    ll dot(const vector2& jcu) const {
        return x * jcu.x + y * jcu.y;
    }
    ll cross(const vector2& jcu) const {
        return x * jcu.y - jcu.x * y;
    }
};

ll ccw(vector2 p, vector2 a, vector2 b) {
    ll ret = (a-p).cross(b-p);
    return ret;
}

int N;
vector<vector2> v;

int main() {
    cin >> N; // 점 개수

    for(int i=0; i<N; i++){
        ll a,b; cin>>a>>b;
        v.emplace_back(vector2(a,b)); // 점 좌표 저장
    }

    ll res=0;
    for(int i=1; i<N-1; i++){
        res += ccw(v[0],v[i],v[i+1]); // ccw, 외적을 이용한 면적계산
    }
    
    // 출력
    cout << fixed; // 소수점 고정
    cout.precision(1); // 한자리
    cout << fabs((double)res / 2) << '\n'; // fabs(k) : double형 절대값
}
