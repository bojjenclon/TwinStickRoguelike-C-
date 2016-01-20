#ifndef EXIT_COLLISION_DATA_HPP
#define EXIT_COLLISION_DATA_HPP

#include <collisions/CollisionData.hpp>
#include <map/Exit.hpp>

struct ExitCollisionData : public CollisionData
{
  Exit* exit = nullptr;
};

#endif
