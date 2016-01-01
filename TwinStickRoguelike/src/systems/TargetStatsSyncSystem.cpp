#include <systems/TargetStatsSyncSystem.hpp>
#include <components/HealthComponent.hpp>
#include <components/TargetedComponent.hpp>

TargetStatsSyncSystem::TargetStatsSyncSystem(UIValues& p_uiValues) : IteratingSystem(ECS::Family::all<TargetedComponent, HealthComponent>().get()), m_uiValues(p_uiValues)
{

}

void TargetStatsSyncSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cHealth = p_entity->get<HealthComponent>();

  if (m_uiValues.enemy.currentHealth != cHealth->currentHealth)
  {
    m_uiValues.enemy.currentHealth = cHealth->currentHealth;
    m_uiValues.enemy.healthChanged = true;
  }
  else if (m_uiValues.player.maxHealth != cHealth->maxHealth)
  {
    m_uiValues.enemy.maxHealth = cHealth->maxHealth;
    m_uiValues.enemy.healthChanged = true;
  }
}

