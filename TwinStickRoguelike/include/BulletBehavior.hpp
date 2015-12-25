#ifndef BULLET_BEHAVIOR_HPP
#define BULLET_BEHAVIOR_HPP

#include <ecstasy/core/Entity.h>
#include <collisions/CollisionData.hpp>

namespace BulletBehavior
{
  //void basicBulletUpdate(ECS::Entity* p_entity, float p_dt);
  void basicBulletCollision(CollisionData* p_dataA, CollisionData* p_dataB);
};

#endif
