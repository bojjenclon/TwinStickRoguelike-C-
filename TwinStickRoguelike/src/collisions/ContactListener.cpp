#include <collisions/ContactListener.hpp>
#include <collisions/CollisionData.hpp>
#include <components/HealthComponent.hpp>
#include <collisions/BulletCollisionData.hpp>
#include <Game.hpp>
#include <components/HitOnceComponent.hpp>

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

  if ((collisionDataA->type == Entity::Player || collisionDataB->type == Entity::Player) && (collisionDataA->type == Entity::Enemy || collisionDataB->type == Entity::Enemy))
  {
    PlayerEnemyContactBegin(collisionDataA, collisionDataB);
  }
  else if ((collisionDataA->type == Entity::Bullet || collisionDataB->type == Entity::Bullet) && (collisionDataA->type == Entity::Enemy || collisionDataB->type == Entity::Enemy))
  {
    BulletEnemyContactBegin(collisionDataA, collisionDataB);
  }
}

void ContactListener::EndContact(b2Contact* p_contact)
{
  
}

void ContactListener::PlayerEnemyContactBegin(CollisionData* p_dataA, CollisionData* p_dataB)
{
  auto playerCollisionData = p_dataA->type == Entity::Type::Player ? p_dataA : p_dataB;
  auto enemyCollisionData = p_dataA->type == Entity::Type::Enemy ? p_dataA : p_dataB;;

  auto cHealth = playerCollisionData->entity->get<HealthComponent>();
  cHealth->currentHealth--;
}

void ContactListener::BulletEnemyContactBegin(CollisionData* p_dataA, CollisionData* p_dataB)
{
  auto bulletCollisionData = p_dataA->type == Entity::Type::Bullet ? static_cast<BulletCollisionData*>(p_dataA) : static_cast<BulletCollisionData*>(p_dataB);
  auto enemyCollisionData = p_dataA->type == Entity::Type::Enemy ? p_dataA : p_dataB;

  if (bulletCollisionData->owner == Entity::Player)
  {
    auto cHitOnce = bulletCollisionData->entity->get<HitOnceComponent>();

    if (find(cHitOnce->alreadyHit.begin(), cHitOnce->alreadyHit.end(), enemyCollisionData->entity->getId()) == cHitOnce->alreadyHit.end())
    {
      auto& game = Game::Get();

      game.setTarget(enemyCollisionData->entity);

      auto cHealth = enemyCollisionData->entity->get<HealthComponent>();
      cHealth->currentHealth--;

      if (cHealth->currentHealth <= 0)
      {
        game.clearTarget();
      }
      
      cHitOnce->alreadyHit.push_back(enemyCollisionData->entity->getId());
    }
  }
}