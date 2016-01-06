#ifndef MICRO_PATHER_COMPONENT_HPP
#define MICRO_PATHER_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <ecstasy/core/Entity.h>
#include <pathfinding/micropather.h>
#ifdef _DEBUG
#include <SFML/Graphics.hpp>
#endif

using namespace micropather;

struct MicroPatherComponent : public ECS::Component<MicroPatherComponent>
{
  ECS::Entity* following = nullptr;
  MPVector<void*> path;
  std::vector<sf::Vector2f> smoothedPath;
  unsigned int positionInPath = 0;

#ifdef _DEBUG
  sf::VertexArray debugDraw;
#endif

  void reset() override
  {
    following = nullptr;
    path.clear();
    smoothedPath.clear();
    positionInPath = 0;

#ifdef _DEBUG
    debugDraw.clear();
#endif
  }
};

#endif

