#include <systems/LifetimeSystem.hpp>
#include <components/LifetimeComponent.hpp>
#include <components/ActiveComponent.hpp>
#include <components/PhysicsComponent.hpp>
#include <components/RenderComponent.hpp>

LifetimeSystem::LifetimeSystem(std::unique_ptr<ECS::Engine>& p_engine, std::unique_ptr<b2World>& p_world) : IteratingSystem(ECS::Family::all<LifetimeComponent>().get()), m_engine(p_engine), m_world(p_world)
{
}

void LifetimeSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cActive = p_entity->get<ActiveComponent>();

  if (!cActive->isActive)
  {
    return;
  }

  auto cLifetime = p_entity->get<LifetimeComponent>();

  cLifetime->timeAlive += p_dt;
  if (cLifetime->timeAlive >= cLifetime->maxLifetime)
  {
    m_engine->removeEntity(p_entity);
  }
}
