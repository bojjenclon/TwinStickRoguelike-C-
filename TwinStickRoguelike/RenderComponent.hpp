#ifndef RENDER_COMPONENT_HPP
#define RENDER_COMPONENT_HPP

#include <ecstasy/core/Component.h>

#include <SDL.h>

struct RenderComponent : public ECS::Component<RenderComponent>
{
  SDL_Texture* texture = nullptr;
  int depth = 1;

  void reset() override
  {
    texture = nullptr;
    depth = 1;
  }
};

#endif

