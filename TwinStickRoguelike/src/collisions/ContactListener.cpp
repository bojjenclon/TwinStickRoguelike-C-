#include <collisions/ContactListener.hpp>
#include <collisions/CollisionData.hpp>
#include <components/HealthComponent.hpp>
#include <components/BulletComponent.hpp>
#include <EntityInfo.hpp>

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
  auto exitCollisionData = p_dataA->type == EntityInfo::Exit ? p_dataA : p_dataB;;

  printf("You hit an exit!\n");
}
