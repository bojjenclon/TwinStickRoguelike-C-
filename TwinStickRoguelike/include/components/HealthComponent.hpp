#ifndef HEALTH_COMPONENT_HPP
#define HEALTH_COMPONENT_HPP

#include <ecstasy/core/Component.h>

struct HealthComponent : public ECS::Component<HealthComponent>
{
  int currentHealth = 5;
  int maxHealth = 5;

  void reset() override
  {
    currentHealth = 5;
    maxHealth = 5;
  }
};

#endif

