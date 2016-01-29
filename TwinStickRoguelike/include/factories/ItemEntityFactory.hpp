#ifndef ITEM_ENTITY_FACTORY_HPP
#define ITEM_ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <Awesomium/WebCore.h>
#include <SFML/Graphics.hpp>
#include <factories/FactoryOptions.hpp>

using namespace Awesomium;

namespace ItemEntityFactory
{
  struct ItemOptions : public FactoryOptions
  {
    std::string name = "Item";
    sf::Sprite& sprite;
    sf::Vector2f position;
    int lifetime = -1;

    ItemOptions(const std::string p_name, sf::Sprite& p_sprite, const sf::Vector2f& p_position, int p_lifetime = -1, bool p_isActive = true)
      : FactoryOptions(p_isActive), name(p_name), sprite(p_sprite), position(p_position), lifetime(p_lifetime)
    {

    }
  };

  ECS::Entity* makeItem(const ItemOptions& p_options);
};

#endif