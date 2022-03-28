#ifndef MAT4X4
#define MAT4X4

#include "vec3d.h"


class Mat4x4{
public:
  float m[3][4] =
  {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0}
  };

  void translate(vec3d v){
    for(int i = 0; i < 3; i++){
      m[i][3] += v.n[i];
    }
  }

  void setPos(vec3d v){
    for(int i = 0; i < 3; i++){
      m[i][3] = v.n[i];
    }
  }

  vec3d getPos(){
    vec3d v;
    for(int i = 0; i < 3; i++){
      v.n[i] = m[i][3];
    }
    return v;
  }

  vec3d operator*(vec3d v){
    vec3d u;
    for(int i = 0; i < 3; i++){
      u.n[i] = v.x * m[i][0] + v.y * m[i][1] + v.z * m[i][2] + m[i][3];
    }
    return u;
  }

  vec3d rotateVec(vec3d v){
    vec3d u;
    for(int i = 0; i < 3; i++){
      u.n[i] = v.x * m[i][0] + v.y * m[i][1] + v.z * m[i][2];
    }
    return u;
  }

  Mat4x4 operator*(const Mat4x4& B) {
    Mat4x4 r;
    for (int row = 0; row < 3; row++) {
      for (int collumn = 0; collumn < 3; collumn++) {
        r.m[row][collumn] = B.m[0][collumn] * m[row][0] + B.m[1][collumn] * m[row][1] + B.m[2][collumn] * m[row][2];
      }
      r.m[row][3] = B.m[0][3] * m[row][0] + B.m[1][3] * m[row][1] + B.m[2][3] * m[row][2] + m[row][3];
    }
    return r;
  }

  Mat4x4 notrans(){
    Mat4x4 r = *this;
    r.m[0][3] = 0;
    r.m[1][3] = 0;
    r.m[2][3] = 0;
    return r;
  }

  Mat4x4 coordInverse(){
    Mat4x4 r;

    vec3d u(m[0][0], m[1][0], m[2][0]);
    vec3d v(m[0][1], m[1][1], m[2][1]);
    vec3d w(m[0][2], m[1][2], m[2][2]);
    vec3d t(m[0][3], m[1][3], m[2][3]);

    r.m[0][0] = u.x; r.m[0][1] = u.y; r.m[0][2] = u.z; r.m[0][3] = u.dot(t) * -1;
    r.m[1][0] = v.x; r.m[1][1] = v.y; r.m[1][2] = v.z; r.m[1][3] = v.dot(t) * -1;
    r.m[2][0] = w.x; r.m[2][1] = w.y; r.m[2][2] = w.z; r.m[2][3] = w.dot(t) * -1;

    return r;
  }

  void rotateAboutOrigin(vec3d, float);

  void print(){
    for(int i = 0; i < 3; i++){
        printf(" { ");
        for(int j = 0; j < 4; j++){
          printf("%f ", m[i][j]);
        }
        printf("} \n\n");
    }
  }


};

#endif
