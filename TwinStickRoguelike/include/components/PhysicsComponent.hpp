#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <Box2D/Dynamics/b2Body.h>

struct PhysicsComponent : public ECS::Component<PhysicsComponent>
{
  b2Body* body = nullptr;

  void reset() override
  {
    body = nullptr;
  }
};

#endif
