#ifndef BASIC_ENTITY_FACTORY_HPP
#define BASIC_ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <Awesomium/WebCore.h>
#include <SFML/Graphics.hpp>
#include <ResourceManager.hpp>
#include <UIValues.hpp>
#include <factories/FactoryOptions.hpp>

using namespace Awesomium;

namespace BasicEntityFactory
{
  struct DrawableOptions : public FactoryOptions
  {
    int depth = 1;

    DrawableOptions(bool p_isActive = true, int p_depth = 1)
      : FactoryOptions(p_isActive), depth(p_depth)
    {
      
    }
  };

  ECS::Entity* makeDrawable(sf::Drawable& p_drawable, const DrawableOptions& p_options);
  ECS::Entity* makeUIContainer(WebView* p_webView, UIValues& p_uiValues);

  ECS::Entity* makePlayer(ResourceManager& p_resources, sf::Vector2f p_position = sf::Vector2f());
};

#endif