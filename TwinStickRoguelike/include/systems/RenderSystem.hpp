#ifndef RENDER_SYSTEM_HPP
#define RENDER_SYSTEM_HPP

#include <ecstasy/core/Engine.h>
#include <ecstasy/systems/SortedIteratingSystem.h>

#include <Game.hpp>
#include <components/RenderComponent.hpp>

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
  RenderSystem(sf::RenderTarget& p_renderTarget);

  void processEntity(ECS::Entity* p_entity, float p_dt) override;

private:
  sf::RenderTarget& m_renderTarget;
};


#endif

