#ifdef _DEBUG

#ifndef PATHFINDING_DEBUG_DRAW_SYSTEM_HPP
#define PATHFINDING_DEBUG_DRAW_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/IteratingSystem.h>
#include <SFML/Graphics/RenderTarget.hpp>

class PathfindingDebugDrawSystem : public ECS::IteratingSystem<PathfindingDebugDrawSystem>
{
public:
  PathfindingDebugDrawSystem(sf::RenderTarget& p_renderTarget);

  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  sf::RenderTarget& m_renderTarget;
};

#endif

#endif