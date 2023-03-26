#include "collisions.hpp"

namespace Collisions
{
  bool boundingBoxesCollision(glm::vec4 firstObjPosition, glm::vec3 firstObjBboxMin, glm::vec3 firstObjBboxMax, glm::vec4 secondObjPosition, glm::vec3 secondObjBboxMin, glm::vec3 secondObjBboxMax)
  {
    // coordenadas do bounding box do primeiro objeto
    float bboxFirstObjMinX = (firstObjPosition.x + firstObjBboxMin.x);
    float bboxFirstObjMaxX = (firstObjPosition.x + firstObjBboxMax.x);
    float bboxFirstObjMinZ = (firstObjPosition.z + firstObjBboxMin.z);
    float bboxFirstObjMaxZ = (firstObjPosition.z + firstObjBboxMax.z);

    // coordenadas do bounding box do segundo objeto
    float bboxSecondObjMinX = (secondObjPosition.x + secondObjBboxMin.x);
    float bboxSecondObjMaxX = (secondObjPosition.x + secondObjBboxMax.x);
    float bboxSecondObjMinZ = (secondObjPosition.z + secondObjBboxMin.z);
    float bboxSecondObjMaxZ = (secondObjPosition.z + secondObjBboxMax.z);

    // se o X não se intercepta
    if (!(bboxFirstObjMinX >= bboxSecondObjMinX && bboxFirstObjMinX <= bboxSecondObjMaxX ||
          bboxFirstObjMinX <= bboxSecondObjMinX && bboxFirstObjMinX >= bboxSecondObjMaxX ||
          bboxFirstObjMaxX >= bboxSecondObjMinX && bboxFirstObjMaxX <= bboxSecondObjMaxX ||
          bboxFirstObjMaxX <= bboxSecondObjMinX && bboxFirstObjMaxX >= bboxSecondObjMaxX))
    {
      return false;
    }

    // se o Z não se intercepta
    if (!(bboxFirstObjMinZ >= bboxSecondObjMinZ && bboxFirstObjMinZ <= bboxSecondObjMaxZ ||
          bboxFirstObjMinZ <= bboxSecondObjMinZ && bboxFirstObjMinZ >= bboxSecondObjMaxZ ||
          bboxFirstObjMaxZ >= bboxSecondObjMinZ && bboxFirstObjMaxZ <= bboxSecondObjMaxZ ||
          bboxFirstObjMaxZ <= bboxSecondObjMinZ && bboxFirstObjMaxZ >= bboxSecondObjMaxZ))
    {
      return false;
    }

    return true;
  }

}
