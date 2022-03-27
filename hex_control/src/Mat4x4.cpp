#include "Mat4x4.h"
#include "Quaternion.h"


void Mat4x4::rotateAboutOrigin(vec3d v, float a){
  *this = *this * Quaternion(v, a).matForm();
}
