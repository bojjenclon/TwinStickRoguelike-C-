#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <Box2D/Box2D.h>
#include <iostream>

struct PhysicsComponent : public ECS::Component<PhysicsComponent>
{
  b2Body* body = nullptr;

  bool hasMaxSpeed = true;
  b2Vec2 maxSpeed = b2Vec2(2, 2);

  b2Vec2 previousPosition = b2Vec2(0, 0);
  float previousAngle = 0.0f;

  void reset() override
  {
    if (body != nullptr)
    {
      auto world = body->GetWorld();
      world->DestroyBody(body);

      body = nullptr;
    }

    hasMaxSpeed = true;
    maxSpeed = b2Vec2(2, 2);

    previousPosition = b2Vec2(0, 0);
    previousAngle = 0.0f;
  }
};

#endif
