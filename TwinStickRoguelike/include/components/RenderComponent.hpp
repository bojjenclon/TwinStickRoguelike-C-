#ifndef RENDER_COMPONENT_HPP
#define RENDER_COMPONENT_HPP

#include <ecstasy/core/Component.h>
#include <SFML/Graphics.hpp>

struct RenderComponent : public ECS::Component<RenderComponent>
{
  sf::Drawable* drawable;
  int depth = 1;

  void reset() override
  {
    if (drawable != nullptr)
    {
      delete drawable;

      drawable = nullptr;
    }

    depth = 1;
  }
};

#endif

