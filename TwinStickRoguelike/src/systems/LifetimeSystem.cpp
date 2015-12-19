#include <systems/LifetimeSystem.hpp>
#include <components/LifetimeComponent.hpp>

LifetimeSystem::LifetimeSystem(ECS::Engine* p_engine) : IteratingSystem(ECS::Family::all<LifetimeComponent>().get()), m_engine(p_engine)
{
}

void LifetimeSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cLifetime = p_entity->get<LifetimeComponent>();

  cLifetime->timeAlive += p_dt;
  if (cLifetime->timeAlive >= cLifetime->maxLifetime)
  {
    m_engine->removeEntity(p_entity);
  }
}
