#include <systems/DeathCheckSystem.hpp>
#include <components/HealthComponent.hpp>
#include <components/PhysicsComponent.hpp>

DeathCheckSystem::DeathCheckSystem(std::unique_ptr<ECS::Engine>& p_engine, std::unique_ptr<b2World>& p_world) : IteratingSystem(ECS::Family::all<HealthComponent>().get()), m_engine(p_engine), m_world(p_world)
{
  
}

void DeathCheckSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cHealth = p_entity->get<HealthComponent>();

  if (cHealth->currentHealth <= 0)
  {
    if (p_entity->has<PhysicsComponent>())
    {
      auto cPhysics = p_entity->get<PhysicsComponent>();
      m_world->DestroyBody(cPhysics->body);
      cPhysics = nullptr;
    }

    m_engine->removeEntity(p_entity);
  }
}

