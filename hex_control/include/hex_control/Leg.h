#ifndef LEG
#define LEG

#include "hex_control/vec3d.h"
#include "hex_control/Mat4x4.h"
#include "hex_control/Quaternion.h"

class Leg {
private:

  uint8_t ID;
  vec3d home = vec3d(0.10, 0, -0.02);
  vec3d worldHome;
  vec3d footPos = home;
  vec3d interpTarget;
  float interpSpeed;
  float maxInterpSpeed = 0.5;
  vec3d interpArcCenter;
  bool arcInterp = false;


  Mat4x4 localToWorldMat;
  Mat4x4 worldToLocalMat;

  float hipAxisOffset = 0.034;
  float femurLength = 0.076;
  float shinLength = 0.110475;

  vec3d servoOffsets = vec3d(90, 90, 0);
  float servoBounds[3][2] = {{50, 130}, {10, 180}, {0, 180}};
  vec3d servoTrim = vec3d(0, 20, 20);

  bool servoIsReversed[3] = {true, false, false};

public:

  Leg(){}

  Leg(uint8_t id){
    ID = id;
  }

  //Setter for interpolation values. If k (curvature) is zero, system set to lerp; otherwise, arc interp.
  void setInterp(vec3d target, float spd, vec3d arcCenter);
  void setInterp(vec3d target, float spd);

  bool update(float dt, float* servoAngles);

  void setLegOrigin(vec3d pos, Quaternion q);
  void setLegOrigin(vec3d pos, float angle);

  uint8_t getID();

  vec3d getAngles();

  void translateLocalSpace(vec3d v);

  vec3d setServos(){
    return setServos(getAngles());
  }

  vec3d setServos(vec3d a){

    a += servoOffsets;
    for(int i = 0; i < 3; i++)
      if(servoIsReversed[i])
        a.n[i] = 180 - a.n[i];
    a += servoTrim;

//    for(byte i = 0; i < 3; i++){
//      bool inBounds = true;
//      float violation;
//      if(a.n[i] < servoBounds[i][0]){
//        inBounds = false;
//        violation = a.n[i];
//        a.n[i] = servoBounds[i][0];
//      }
//      else if(a.n[i] > servoBounds[i][1]){
//        inBounds = false;
//        violation = a.n[i];
//        a.n[i] = servoBounds[i][1];
//      }
//    }
    return a;
  }

  vec3d getPos(){
    return footPos;
  }
  vec3d getHome()
  {
    return worldHome;
  }


};

#endif
