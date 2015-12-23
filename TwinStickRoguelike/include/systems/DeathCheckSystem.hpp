#ifndef DEATH_CHECK_SYSTEM_HPP
#define DEATH_CHECK_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>
#include <Box2D/Dynamics/b2World.h>

class DeathCheckSystem : public ECS::IteratingSystem<DeathCheckSystem>
{
public:
  DeathCheckSystem(std::unique_ptr<ECS::Engine>& p_engine, std::unique_ptr<b2World>& p_world);

  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  std::unique_ptr<ECS::Engine>& m_engine;
  std::unique_ptr<b2World>& m_world;
};

#endif

