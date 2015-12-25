#ifndef COLLISION_DATA_HPP
#define COLLISION_DATA_HPP

#include <ecstasy/core/Entity.h>
#include <EntityInfo.hpp>

struct CollisionData
{
  EntityInfo::Type type = EntityInfo::Obstacle;
  ECS::Entity* entity = nullptr;
};

#endif
