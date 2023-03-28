#include "collisions.hpp"
namespace Collisions
{
  bool boundingBoxesCollision(Entity firstObj, Entity secondObj)
  {

    glm::vec4 bboxFirstObjMin = glm::vec4(firstObj.getObject().getBboxMin(),1.0f);
    glm::vec4 bboxFirstObjMax = glm::vec4(firstObj.getObject().getBboxMax(),1.0f);
    // coordenadas do bounding box do primeiro objeto
    bboxFirstObjMin = firstObj.getTransformationMatrix() * bboxFirstObjMin;
    bboxFirstObjMax = firstObj.getTransformationMatrix() * bboxFirstObjMax;


    glm::vec4 bboxSecondObjMin = glm::vec4(secondObj.getObject().getBboxMin(),1.0f);
    glm::vec4 bboxSecondObjMax = glm::vec4(secondObj.getObject().getBboxMax(),1.0f);
    // coordenadas do bounding box do segundo objeto
    bboxSecondObjMin = secondObj.getTransformationMatrix() * bboxSecondObjMin;
    bboxSecondObjMax = secondObj.getTransformationMatrix() * bboxSecondObjMax;

    // se o X não se intercepta
    if (!(bboxFirstObjMin.x >= bboxSecondObjMin.x && bboxFirstObjMin.x <= bboxSecondObjMax.x ||
          bboxFirstObjMin.x <= bboxSecondObjMin.x && bboxFirstObjMin.x >= bboxSecondObjMax.x ||
          bboxFirstObjMax.x >= bboxSecondObjMin.x && bboxFirstObjMax.x <= bboxSecondObjMax.x ||
          bboxFirstObjMax.x <= bboxSecondObjMin.x && bboxFirstObjMax.x >= bboxSecondObjMax.x))
    {
      return false;
    }

    // se o Y não se intercepta
    if (!(bboxFirstObjMin.y >= bboxSecondObjMin.y && bboxFirstObjMin.y <= bboxSecondObjMax.y ||
          bboxFirstObjMin.y <= bboxSecondObjMin.y && bboxFirstObjMin.y >= bboxSecondObjMax.y ||
          bboxFirstObjMax.y >= bboxSecondObjMin.y && bboxFirstObjMax.y <= bboxSecondObjMax.y ||
          bboxFirstObjMax.y <= bboxSecondObjMin.y && bboxFirstObjMax.y >= bboxSecondObjMax.y))
    {
      return false;
    }

    // se o Z não se intercepta
    if (!(bboxFirstObjMin.z >= bboxSecondObjMin.z && bboxFirstObjMin.z <= bboxSecondObjMax.z ||
          bboxFirstObjMin.z <= bboxSecondObjMin.z && bboxFirstObjMin.z >= bboxSecondObjMax.z ||
          bboxFirstObjMax.z >= bboxSecondObjMin.z && bboxFirstObjMax.z <= bboxSecondObjMax.z ||
          bboxFirstObjMax.z <= bboxSecondObjMin.z && bboxFirstObjMax.z >= bboxSecondObjMax.z))
    {
      return false;
    }

    return true;
  }

}
