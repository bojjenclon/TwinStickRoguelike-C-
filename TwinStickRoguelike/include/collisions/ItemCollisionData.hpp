#ifndef ITEM_COLLISION_DATA_HPP
#define ITEM_COLLISION_DATA_HPP

#include <collisions/CollisionData.hpp>

struct ItemCollisionData : public CollisionData
{
  std::string name = "None";
};

#endif
