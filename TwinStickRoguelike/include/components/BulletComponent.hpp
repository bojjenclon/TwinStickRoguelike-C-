#ifndef BULLET_COMPONENT_HPP
#define BULLET_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <functional>
#include <ecstasy/core/Entity.h>
#include <collisions/CollisionData.hpp>

struct BulletComponent : public ECS::Component<BulletComponent>
{
  //  updateCallback
  //    parameters:
  //      ECS::Entity* p_entity -> the bullet's parent entity
  //      float p_dt -> the elapsed time between frames
  //
  //    returns:
  //      void -> return nothing
  std::function<void(ECS::Entity* p_entity, float p_dt)> updateCallback = nullptr;
  std::function<void(CollisionData* p_dataA, CollisionData* p_dataB)> collisionCallback = nullptr;

  void reset() override
  {
    updateCallback = nullptr;
    collisionCallback = nullptr;
  }
};

#endif
