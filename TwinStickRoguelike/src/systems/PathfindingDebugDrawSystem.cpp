#ifdef _DEBUG

#include <systems/PathfindingDebugDrawSystem.hpp>
#include <components/MicroPatherComponent.hpp>

PathfindingDebugDrawSystem::PathfindingDebugDrawSystem(sf::RenderTarget& p_renderTarget) : IteratingSystem(ECS::Family::all<MicroPatherComponent>().get()), m_renderTarget(p_renderTarget)
{
}

void PathfindingDebugDrawSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cMicroPather = p_entity->get<MicroPatherComponent>();
  m_renderTarget.draw(cMicroPather->debugDraw);
}

#endif