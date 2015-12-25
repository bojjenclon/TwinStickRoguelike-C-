#ifndef ENEMY_ENTITY_FACTORY_HPP
#define ENEMY_ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <ResourceManager.hpp>

namespace EnemyEntityFactory
{
  ECS::Entity* makeBasicEnemy(ResourceManager& p_resources, sf::Vector2f p_position = sf::Vector2f());
};

#endif