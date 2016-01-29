#ifndef BULLET_ENTITY_FACTORY_HPP
#define BULLET_ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <ResourceManager.hpp>
#include <EntityInfo.hpp>
#include <factories/FactoryOptions.hpp>

namespace BulletEntityFactory
{
  struct BulletOptions : public FactoryOptions
  {
    EntityInfo::Type owner;
    sf::Vector2f position;
    sf::Vector2f velocity;

    BulletOptions(EntityInfo::Type p_owner, sf::Vector2f p_position, sf::Vector2f p_velocity, bool p_isActive = true)
      : FactoryOptions(p_isActive), owner(p_owner), position(p_position), velocity(p_velocity)
    {

    }
  };

  ECS::Entity* makeBasicBullet(ResourceManager& p_resources, BulletOptions p_options);
};

#endif