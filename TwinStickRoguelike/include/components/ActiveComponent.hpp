#ifndef ACTIVE_COMPONENT_HPP
#define ACTIVE_COMPONENT_HPP

#include <ecstasy/core/Component.h>

struct ActiveComponent : public ECS::Component<ActiveComponent>
{
  bool isActive = true;

  void reset() override
  {
    isActive = true;
  }
};

#endif

