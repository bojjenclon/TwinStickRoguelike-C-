#include <collisions/ContactListener.hpp>
#include <collisions/CollisionData.hpp>
#include <components/HealthComponent.hpp>
#include <components/BulletComponent.hpp>
#include <EntityInfo.hpp>
#include <collisions/ExitCollisionData.hpp>
#include <tiled/TiledMap.hpp>
#include <Game.hpp>
#include <components/RenderComponent.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <components/LifetimeComponent.hpp>
#include <components/ItemComponent.hpp>

void ContactListener::BeginContact(b2Contact* p_contact)
{
  auto userDataA = p_contact->GetFixtureA()->GetBody()->GetUserData();
  auto userDataB = p_contact->GetFixtureB()->GetBody()->GetUserData();
  
  if (userDataA == nullptr || userDataB == nullptr)
  {
    return;
  }

  auto collisionDataA = static_cast<CollisionData*>(userDataA);
  auto collisionDataB = static_cast<CollisionData*>(userDataB);

  if ((collisionDataA->type == EntityInfo::Player || collisionDataB->type == EntityInfo::Player) && (collisionDataA->type == EntityInfo::Enemy || collisionDataB->type == EntityInfo::Enemy))
  {
    PlayerEnemyContactBegin(collisionDataA, collisionDataB);
  }
  else if (collisionDataA->type == EntityInfo::Bullet || collisionDataB->type == EntityInfo::Bullet)
  {
    BulletContactBegin(collisionDataA, collisionDataB);
  }
  else if ((collisionDataA->type == EntityInfo::Player || collisionDataB->type == EntityInfo::Player) && (collisionDataA->type == EntityInfo::Exit || collisionDataB->type == EntityInfo::Exit))
  {
    PlayerExitContactBegin(collisionDataA, collisionDataB);
  }
  else if ((collisionDataA->type == EntityInfo::Player || collisionDataB->type == EntityInfo::Player) && (collisionDataA->type == EntityInfo::Item || collisionDataB->type == EntityInfo::Item))
  {
    PlayerItemContactBegin(collisionDataA, collisionDataB);
  }
}

void ContactListener::EndContact(b2Contact* p_contact)
{
  
}

void ContactListener::PlayerEnemyContactBegin(CollisionData* p_dataA, CollisionData* p_dataB)
{
  auto playerCollisionData = p_dataA->type == EntityInfo::Player ? p_dataA : p_dataB;
  //auto enemyCollisionData = p_dataA->type == Entity::Type::Enemy ? p_dataA : p_dataB;;

  auto cHealth = playerCollisionData->entity->get<HealthComponent>();
  cHealth->currentHealth--;
}

void ContactListener::BulletContactBegin(CollisionData* p_dataA, CollisionData* p_dataB)
{
  auto bulletCollisionData = p_dataA->type == EntityInfo::Bullet ? p_dataA : p_dataB;

  if (!bulletCollisionData->entity->has<BulletComponent>())
  {
    return;
  }

  auto cBullet = bulletCollisionData->entity->get<BulletComponent>();

  if (cBullet->collisionCallback != nullptr)
  {
    cBullet->collisionCallback(p_dataA, p_dataB);
  }
}

void ContactListener::PlayerExitContactBegin(CollisionData* p_dataA, CollisionData* p_dataB)
{
  auto playerCollisionData = p_dataA->type == EntityInfo::Player ? p_dataA : p_dataB;
  auto exitCollisionData = static_cast<ExitCollisionData*>(p_dataA->type == EntityInfo::Exit ? p_dataA : p_dataB);
  
  if (exitCollisionData->exit->getDestination() != nullptr)
  {
    auto& game = Game::Get();

    auto destination = exitCollisionData->exit->getDestination();

    auto cRender = playerCollisionData->entity->get<RenderComponent>();
    auto sprite = static_cast<sf::Sprite*>(cRender->drawable);
    auto playerSize = sprite->getLocalBounds();

    sf::Vector2f playerPosition;

    switch (exitCollisionData->exit->getDirection())
    {
      case (North) :
      {
        auto exit = destination->tiledMap->getExit(South);

        auto exitPosition = exit.getPosition();

        playerPosition.x = exitPosition.x;
        playerPosition.y = exitPosition.y - playerSize.height;

        break;
      }
      case (South) :
      {
        auto exit = destination->tiledMap->getExit(North);

        auto exitPosition = exit.getPosition();
        auto exitSize = exit.getSize();

        playerPosition.x = exitPosition.x;
        playerPosition.y = exitPosition.y + exitSize.y + playerSize.height;

        break;
      }
      case (West) :
      {
        auto exit = destination->tiledMap->getExit(East);

        auto exitPosition = exit.getPosition();

        playerPosition.x = exitPosition.x - playerSize.width;
        playerPosition.y = exitPosition.y;

        break;
      }
      case (East) :
      {
        auto exit = destination->tiledMap->getExit(West);

        auto exitPosition = exit.getPosition();
        auto exitSize = exit.getSize();

        playerPosition.x = exitPosition.x + exitSize.x + playerSize.width;
        playerPosition.y = exitPosition.y;

        break;
      }
    }

    game.changeMap(destination, playerPosition);
  }
}

void ContactListener::PlayerItemContactBegin(CollisionData* p_dataA, CollisionData* p_dataB)
{
  auto playerCollisionData = p_dataA->type == EntityInfo::Player ? p_dataA : p_dataB;
  auto itemCollisionData = p_dataA->type == EntityInfo::Item ? p_dataA : p_dataB;

  auto itemEntity = itemCollisionData->entity;

  auto cItem = itemEntity->get<ItemComponent>();

  auto destroyItem = true;
  if (cItem->callback != nullptr)
  {
    destroyItem = cItem->callback(playerCollisionData->entity);
  }

  printf("Collided with item: %s\n", cItem->name.c_str());

  if (destroyItem)
  {
    // cheap hack to delete the item

    auto& game = Game::Get();
    auto& engine = game.getEngine();

    auto cLifetime = engine.createComponent<LifetimeComponent>();
    cLifetime->maxLifetime = 0;

    itemEntity->add(cLifetime);
  }
}