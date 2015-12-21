#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <include/cef_browser.h>

#include <SFML/Graphics.hpp>

#include <ResourceManager.hpp>
#include <UIValues.hpp>

namespace EntityFactory
{
  enum CollisionType
  {
    Obstacle        = 1 << 0,
    Player          = 1 << 1,
    Enemy           = 1 << 2,
    PlayerBullet    = 1 << 3,
    EnemyBullet     = 1 << 4
  };

  ECS::Entity* makeDrawable(sf::Drawable& p_drawable, int p_depth = 1);
  ECS::Entity* makeUIContainer(sf::Sprite& p_sprite, CefRefPtr<CefBrowser> p_browser, UIValues& p_uiValues);
  ECS::Entity* makePlayer(ResourceManager& p_resources, sf::Vector2f p_position = sf::Vector2f());
  ECS::Entity* makeBullet(ResourceManager& p_resources, sf::Vector2f p_position = sf::Vector2f(), sf::Vector2f p_velocity = sf::Vector2f());
};

#endif