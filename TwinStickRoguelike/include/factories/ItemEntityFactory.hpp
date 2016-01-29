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
    std::string name;
    std::function<bool(ECS::Entity* p_entity)> callback = nullptr;
    sf::Sprite& sprite;
    sf::Vector2f position;
    int lifetime = -1;

    ItemOptions(std::string p_name, std::function<bool(ECS::Entity* p_entity)> p_callback, sf::Sprite& p_sprite, const sf::Vector2f& p_position, int p_lifetime = -1, bool p_isActive = true)
      : FactoryOptions(p_isActive), name(p_name), callback(p_callback), sprite(p_sprite), position(p_position), lifetime(p_lifetime)
    {

    }
  };

  ECS::Entity* makeItem(const ItemOptions& p_options);
};

#endif