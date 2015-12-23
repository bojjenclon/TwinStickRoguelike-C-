#include <systems/PlayerStatsSyncSystem.hpp>
#include <components/PlayerComponent.hpp>
#include <components/HealthComponent.hpp>

PlayerStatsSyncSystem::PlayerStatsSyncSystem(UIValues& p_uiValues) : IteratingSystem(ECS::Family::all<PlayerComponent, HealthComponent>().get()), m_uiValues(p_uiValues)
{
  
}

void PlayerStatsSyncSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cHealth = p_entity->get<HealthComponent>();
  
  if (m_uiValues.currentHealth != cHealth->currentHealth)
  {
    m_uiValues.currentHealth = cHealth->currentHealth;
    m_uiValues.healthChanged = true;
  }
  else if (m_uiValues.maxHealth != cHealth->maxHealth)
  {
    m_uiValues.maxHealth = cHealth->maxHealth;
    m_uiValues.healthChanged = true;
  }
}

