#ifndef BASIC_ENTITY_FACTORY_HPP
#define BASIC_ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <Awesomium/WebCore.h>
#include <SFML/Graphics.hpp>
#include <ResourceManager.hpp>
#include <UIValues.hpp>

using namespace Awesomium;

namespace BasicEntityFactory
{
  ECS::Entity* makeDrawable(sf::Drawable& p_drawable, int p_depth = 1);
  ECS::Entity* makeUIContainer(sf::Sprite& p_sprite, WebView* p_webView, UIValues& p_uiValues);

  ECS::Entity* makePlayer(ResourceManager& p_resources, sf::Vector2f p_position = sf::Vector2f());
};

#endif