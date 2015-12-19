#ifndef LIFETIME_COMPONENT_HPP
#define LIFETIME_COMPONENT_HPP

#include <ecstasy/core/Component.h>

struct LifetimeComponent : public ECS::Component<LifetimeComponent>
{
  float timeAlive = 0.0f;
  float maxLifetime = 5.0f;

  void reset() override
  {
    timeAlive = 0.0f;
    maxLifetime = 5.0f;
  }
};

#endif

