#include "collisions.hpp"
#include <iostream>

namespace Collisions
{
  bool spheresCollisionTest(glm::vec4 firstObjPosition, float firstObjRadius, glm::vec4 secondObjPosition, float secondObjRadius)
  {
    float distance = glm::length(firstObjPosition - secondObjPosition);

    if (distance < (firstObjRadius + secondObjRadius))
    {
      return true;
    }

    return false;
  }

  bool sphereBoxCollisionTest(glm::vec4 spherePosition, float sphereRadius, Entity box)
  {
    glm::vec4 bboxMin = glm::vec4(box.getAABBMin(), 1.0f);
    glm::vec4 bboxMax = glm::vec4(box.getAABBMax(), 1.0f);


    glm::vec3 closestPoint;
    closestPoint.x = glm::clamp(spherePosition.x, bboxMin.x, bboxMax.x);
    closestPoint.y = glm::clamp(spherePosition.y, bboxMin.y, bboxMax.y);
    closestPoint.z = glm::clamp(spherePosition.z, bboxMin.z, bboxMax.z);

    float distance = glm::distance(closestPoint, glm::vec3(spherePosition));

    if (distance <= sphereRadius)
    {
      return true;
    }

    return false;
  }
}
