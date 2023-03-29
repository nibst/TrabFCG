#include "matrices.h"
#include "entity.hpp"
namespace Collisions
{
  bool spheresCollisionTest(glm::vec4 firstObjPosition, float firstObjRadius, glm::vec4 secondObjPosition, float secondObjRadius);
  bool sphereBoxCollisionTest(glm::vec4 spherePosition, float sphereRadius, Entity box);
}
