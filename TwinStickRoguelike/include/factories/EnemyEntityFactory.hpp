#ifndef ENEMY_ENTITY_FACTORY_HPP
#define ENEMY_ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <ResourceManager.hpp>
#include <tiled/TiledMap.hpp>

using namespace micropather;

namespace EnemyEntityFactory
{
  ECS::Entity* makeBasicEnemy(ResourceManager& p_resources, TiledMap* p_map, sf::Vector2f p_position = sf::Vector2f());
};

#endif