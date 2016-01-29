#include <systems/DeathCheckSystem.hpp>
#include <components/HealthComponent.hpp>
#include <components/PhysicsComponent.hpp>
#include <components/ActiveComponent.hpp>

DeathCheckSystem::DeathCheckSystem(std::unique_ptr<ECS::Engine>& p_engine, std::unique_ptr<b2World>& p_world) : IteratingSystem(ECS::Family::all<HealthComponent>().get()), m_engine(p_engine), m_world(p_world)
{
  
}

void DeathCheckSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cActive = p_entity->get<ActiveComponent>();

  if (!cActive->isActive)
  {
    return;
  }

  auto cHealth = p_entity->get<HealthComponent>();

  if (cHealth->currentHealth <= 0)
  {
    m_engine->removeEntity(p_entity);
  }
}

