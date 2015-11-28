#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/SortedIteratingSystem.h>

#include "Game.hpp"
#include "RenderComponent.hpp"

struct DepthTest
{
  bool operator()(ECS::Entity* a, ECS::Entity* b)
  {
    auto ac = a->get<RenderComponent>();
    auto bc = b->get<RenderComponent>();

    return ac->depth > bc->depth;
  }
};

class RenderSystem : public ECS::SortedIteratingSystem<RenderSystem, DepthTest>
{
public:
  RenderSystem(Game* p_game);

  void update(float deltaTime) override;
  void processEntity(ECS::Entity* entity, float deltaTime) override;

private:
  Game* m_game = nullptr;
};


#endif

