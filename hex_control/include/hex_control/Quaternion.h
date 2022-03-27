#ifndef QUATERNION
#define QUATERNION

#include "vec3d.h"
#include "Mat4x4.h"

class Quaternion {
public:
  union {
    struct { float a, b, c, d; };
    struct { float n[4]; };
  };

  Quaternion(){
    a = 1;
    b = 0;
    c = 0;
    d = 0;
  }

  Quaternion(float a1, float b1, float c1, float d1){
    a = a1;
    b = b1;
    c = c1;
    d = d1;
  }

  Quaternion(vec3d& v, float deg){
    setByVecDeg(v, deg);
  }

  Quaternion(vec3d& v){
    setByVecDeg(v, v.mag());
  }

  Quaternion(vec3d& v, vec3d& u){
    vec3d axis = v.cross(u);
    axis /= (v.mag() * u.mag());
    float deg = asin(axis.mag()) * 180 / 3.141592653589793;
    setByVecDeg(axis, deg);
  }

  Quaternion operator*(Quaternion q){
    Quaternion r;
    r.a = a * q.a - b * q.b - c * q.c - d * q.d;
    r.b = a * q.b + b * q.a + c * q.d - d * q.c;
    r.c = a * q.c - b * q.d + c * q.a + d * q.b;
    r.d = a * q.d + b * q.c - c * q.b + d * q.a;
    return r;
  }

  void operator=(const Quaternion q){
    for (int i = 0; i < 4; i++)
      n[i] = q.n[i];
  }

  Quaternion inverse(){
    Quaternion q(a, -b, -c, -d);
    return q;
  }

  vec3d operator*(vec3d& v){
    Quaternion qin( a, -b, -c, -d );
    Quaternion qVec( 0, v.x, v.y, v.z );
    Quaternion temp = (*this * qVec) * qin;
    vec3d r = { temp.b, temp.c, temp.d };
    return r;
  }

  void setByVecDeg(vec3d& vin, float angle, float percent = 1.0){
    float halfrad = 0.5 * angle * percent * 3.141592653589793 / 180;
    vec3d v = vin.unit();
    a = cos(halfrad);
    b = sin(halfrad) * v.x;
    c = sin(halfrad) * v.y;
    d = sin(halfrad) * v.z;
  }

  Quaternion slerp(float t){
    Quaternion q;
    float theta = 2 * acos(a);

    if(theta == 0)
      return *this;

    float scalar = sin(t * theta) / sin(theta);

    q.a = (sin((1 - t) * theta) / sin(theta)) + a * scalar;
    q.b = b * scalar;
    q.c = c * scalar;
    q.d = d * scalar;
    return q;
  }

  void normalize(){
    float length = sqrt(a * a + b * b + c * c + d * d);
    a /= length;
    b /= length;
    c /= length;
    d /= length;
  }

  vec3d attitude(){ //BROKEN fix before use!!!
    float pitch, yaw, roll;
    vec3d zprime = *this * vec3d::Z;
    vec3d temp = zprime;
    temp.y = 0;
    pitch = 90 - vec3d::Y.angleBetween(zprime);
    yaw = vec3d::Z.angleBetween(temp);
    temp = *this * vec3d::X;
    roll = 90 - vec3d::Y.angleBetween(temp);
    return vec3d(pitch, yaw, roll);

  }

  Mat4x4 matForm(){
    Mat4x4 R;
    float a2, b2, c2, d2;
    a2 = a * a; b2 = b * b; c2 = c * c; d2 = d * d;
    R.m[0][0] = a2 + b2 - c2 - d2;
    R.m[1][1] = a2 - b2 + c2 - d2;
    R.m[2][2] = a2 - b2 - c2 + d2;

    R.m[0][1] = 2 * b * c - 2 * a * d;
    R.m[0][2] = 2 * b * d + 2 * a * c;
    R.m[1][0] = 2 * b * c + 2 * a * d;
    R.m[1][2] = 2 * c * d - 2 * a * b;
    R.m[2][0] = 2 * b * d - 2 * a * c;
    R.m[2][1] = 2 * c * d + 2 * a * b;

    return R;
  }

  void print(int decimal = 2){
    Serial.print(" { ");
    Serial.print(a, decimal);
    Serial.print(", ");
    Serial.print(b, decimal);
    Serial.print(", ");
    Serial.print(c, decimal);
    Serial.print(", ");
    Serial.print(d, decimal);
    Serial.print(" } ");
  }

  void println(int decimal = 2){
    print(decimal);
    Serial.println();
  }

};

#endif
