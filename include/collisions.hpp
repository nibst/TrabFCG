#include "matrices.h"
#include "entity.hpp"
namespace Collisions
{
  bool boundingSpheresCollisionTest(glm::vec4 firstObjPosition, float firstObjRadius, glm::vec4 secondObjPosition, float secondObjRadius);
  bool boundingSphereBoundingBoxCollisionTest(glm::vec4 spherePosition, float sphereRadius, Entity box);
}
