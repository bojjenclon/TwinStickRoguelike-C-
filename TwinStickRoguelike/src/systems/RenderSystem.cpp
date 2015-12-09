#include "systems/RenderSystem.hpp"

#include <ecstasy/core/Family.h>
#include <SFML/Graphics.hpp>

RenderSystem::RenderSystem(sf::RenderTarget& p_renderTarget) : SortedIteratingSystem(ECS::Family::all<RenderComponent>().get(), DepthTest()), m_renderTarget(p_renderTarget)
{
}

void RenderSystem::processEntity(ECS::Entity* entity, float deltaTime)
{
  auto cRender = entity->get<RenderComponent>();

  m_renderTarget.draw(*cRender->drawable);
}
