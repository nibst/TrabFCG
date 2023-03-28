#include "collisions.hpp"
#include <iostream>
namespace Collisions
{
  bool boundingSpheresCollisionTest(glm::vec4 firstObjPosition, float firstObjRadius, glm::vec4 secondObjPosition, float secondObjRadius)
  {
    float distance = glm::length(firstObjPosition - secondObjPosition);

    if (distance < (firstObjRadius + secondObjRadius))
    {
      return true;
    }

    return false;
  }
}
