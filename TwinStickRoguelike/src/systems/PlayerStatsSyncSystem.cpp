#include <systems/PlayerStatsSyncSystem.hpp>
#include <components/PlayerComponent.hpp>
#include <components/HealthComponent.hpp>

PlayerStatsSyncSystem::PlayerStatsSyncSystem(UIValues& p_uiValues) : IteratingSystem(ECS::Family::all<PlayerComponent, HealthComponent>().get()), m_uiValues(p_uiValues)
{
  
}

void PlayerStatsSyncSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cHealth = p_entity->get<HealthComponent>();
  
  if (m_uiValues.player.currentHealth != cHealth->currentHealth)
  {
    m_uiValues.player.currentHealth = cHealth->currentHealth;
    m_uiValues.player.healthChanged = true;
  }
  else if (m_uiValues.player.maxHealth != cHealth->maxHealth)
  {
    m_uiValues.player.maxHealth = cHealth->maxHealth;
    m_uiValues.player.healthChanged = true;
  }
}

