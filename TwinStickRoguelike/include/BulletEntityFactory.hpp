#ifndef BULLET_ENTITY_FACTORY_HPP
#define BULLET_ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <ResourceManager.hpp>
#include <EntityInfo.hpp>

struct Bullet
{
  struct Options
  {
    EntityInfo::Type owner;
    sf::Vector2f position;
    sf::Vector2f velocity;
  };
};

namespace BulletEntityFactory
{
  ECS::Entity* makeBasicBullet(ResourceManager& p_resources, Bullet::Options p_options);
};

#endif