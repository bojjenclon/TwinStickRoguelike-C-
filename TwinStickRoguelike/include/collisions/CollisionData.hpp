#ifndef COLLISION_DATA_HPP
#define COLLISION_DATA_HPP

#include <EntityFactory.hpp>

struct CollisionData
{
  Entity::Type type = Entity::Obstacle;
  ECS::Entity* entity = nullptr;
};

#endif
