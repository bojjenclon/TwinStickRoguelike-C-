#ifndef VELOCITY_COMPONENT_HPP
#define VELOCITY_COMPONENT_HPP

#include <ecstasy/core/Component.h>

struct VelocityComponent : public ECS::Component<VelocityComponent>
{
  float vx = 0;
  float vy = 0;

  void reset() override
  {
    vx = vy = 0;
  }
};

#endif
