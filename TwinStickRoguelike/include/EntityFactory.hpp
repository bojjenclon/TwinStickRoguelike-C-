#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <include/cef_browser.h>

#include <SFML/Graphics.hpp>

#include <ResourceManager.hpp>
#include <UIValues.hpp>

namespace EntityFactory
{
  ECS::Entity* makeDrawable(ECS::Engine* p_engine, sf::Drawable& p_drawable, int p_depth);
  ECS::Entity* makeUIContainer(ECS::Engine* p_engine, sf::Sprite& p_sprite, CefRefPtr<CefBrowser> p_browser, UIValues& p_uiValues);
  ECS::Entity* makePlayer(ECS::Engine* p_engine, ResourceManager& p_resources, sf::Vector2f p_position = sf::Vector2f());
  ECS::Entity* makeBullet(ECS::Engine* p_engine, ResourceManager& p_resources, sf::Vector2f p_position = sf::Vector2f(), sf::Vector2f p_velocity = sf::Vector2f());
};

#endif