#ifndef PHYSICS_SYSTEM_HPP
#define PHYSICS_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>
#include <Box2D/Dynamics/b2World.h>

class PhysicsSystem : public ECS::IteratingSystem<PhysicsSystem>
{
public:
  PhysicsSystem(std::unique_ptr<b2World>& p_world);

  void update(float p_dt) override;
  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  std::unique_ptr<b2World>& m_world;

  float m_accumulator;
  float m_fixedStepLimit;

  void copyCurrentPosition() const;
};

#endif

