#ifndef LIFETIME_SYSTEM_HPP
#define LIFETIME_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>

class LifetimeSystem : public ECS::IteratingSystem<LifetimeSystem>
{
public:
  LifetimeSystem(std::unique_ptr<ECS::Engine>& p_engine);

  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  std::unique_ptr<ECS::Engine>& m_engine;
};

#endif

