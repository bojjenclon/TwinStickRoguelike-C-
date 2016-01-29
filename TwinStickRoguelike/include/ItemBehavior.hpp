#ifndef ITEM_BEHAVIOR_HPP
#define ITEM_BEHAVIOR_HPP

#include <ecstasy/core/Entity.h>

// return value is if the item should be destroyed upon being picked up
namespace ItemBehavior
{
  bool testItem(ECS::Entity* p_pickedUpby);
};

#endif
