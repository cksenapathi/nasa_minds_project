#include "hex_control/Leg.h"
#include "hex_control/vec3d.h"

void Leg::translateLocalSpace(vec3d v)
{
  footPos += v;
}

vec3d Leg::getAngles()
{
  float theta, q1, q2, xprime;

  theta = atan2(footPos.y, footPos.x);
  xprime = sqrt(pow(footPos.x, 2) + pow(footPos.y, 2)) - hipAxisOffset;

  q2 = acos((pow(xprime,2) + pow(footPos.z,2) - pow(femurLength,2) - pow(shinLength,2)) / (2 * shinLength * femurLength));
  q1 = atan2(footPos.z, xprime) + atan2(shinLength * sin(q2), (femurLength + shinLength * cos(q2)));

  vec3d angles(theta, q1, q2);
  angles *= 180 / PI;

  return angles;
}

void Leg::setInterp(vec3d target, float spd, vec3d arcCenter)
{
  arcInterp = true;
  interpTarget = worldToLocalMat * target; //get target's position in local coords
  interpArcCenter = worldToLocalMat * arcCenter;
  if(spd > maxInterpSpeed)
    spd = maxInterpSpeed;
  interpSpeed = spd;
}

void Leg::setInterp(vec3d target, float spd)
{
  arcInterp = false;
  interpTarget = worldToLocalMat * target; //get target's position in local coords
  if(spd > maxInterpSpeed)
    spd = maxInterpSpeed;
  interpSpeed = spd;
}

bool Leg::update(float dt, float* servoAngles)
{
  vec3d stepdir = interpTarget - footPos;
  float stepSize = interpSpeed * dt;
  if(stepdir.mag() > stepSize){
    stepdir = stepdir.unit() * stepSize;
    translateLocalSpace(stepdir);
  }
  else{
    translateLocalSpace(stepdir);
  }

  vec3d a = getAngles();
  for(int i = 0; i < 3; i++) servoAngles[i] = a.n[i];

  if(footPos == interpTarget) return true;
  return false;
}

void Leg::setLegOrigin(vec3d pos, Quaternion q)
{
  worldToLocalMat.setPos(pos*-1);
  worldToLocalMat = q.inverse().matForm() * worldToLocalMat;

  localToWorldMat = worldToLocalMat.coordInverse();

  worldHome = localToWorldMat * home;
}

void Leg::setLegOrigin(vec3d pos, float angle)
{
  setLegOrigin(pos, Quaternion(vec3d::Z, angle));
}

uint8_t Leg::getID()
{
  return ID;
}
