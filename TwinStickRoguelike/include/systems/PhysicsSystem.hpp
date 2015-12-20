#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class PhysicsSystem : public ECS::IteratingSystem<PhysicsSystem>
{
public:
  PhysicsSystem();

  void update(float p_dt) override;
  void processEntity(ECS::Entity* p_entity, float p_dt) override;
};

#endif

