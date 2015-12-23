#ifndef BULLET_COLLISION_DATA_HPP
#define BULLET_COLLISION_DATA_HPP

#include <collisions/CollisionData.hpp>

struct BulletCollisionData : public CollisionData
{
  Entity::Type owner = Entity::Player;
};

#endif
