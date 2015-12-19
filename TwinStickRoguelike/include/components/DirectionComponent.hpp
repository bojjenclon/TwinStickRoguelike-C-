#ifndef DIRECTION_COMPONENT_HPP
#define DIRECTION_COMPONENT_HPP

#include <ecstasy/core/Component.h>

enum Direction
{
  Up,
  Down,
  Left,
  Right
};

struct DirectionComponent : public ECS::Component<DirectionComponent>
{
  Direction direction = Down;

  void reset() override
  {
    direction = Down;
  }
};

#endif
