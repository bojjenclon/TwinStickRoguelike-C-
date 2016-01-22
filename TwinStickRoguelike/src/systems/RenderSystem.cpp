#include <systems/RenderSystem.hpp>

#include <ecstasy/core/Family.h>
#include <SFML/Graphics.hpp>
#include <components/ActiveComponent.hpp>

RenderSystem::RenderSystem(sf::RenderTarget& p_renderTarget) : SortedIteratingSystem(ECS::Family::all<RenderComponent>().get(), DepthTest()), m_renderTarget(p_renderTarget)
{
}

void RenderSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cActive = p_entity->get<ActiveComponent>();

  if (!cActive->isActive)
  {
    return;
  }

  auto cRender = p_entity->get<RenderComponent>();

  m_renderTarget.draw(*cRender->drawable);
}
