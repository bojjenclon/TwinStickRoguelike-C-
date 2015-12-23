#include <collisions/ContactListener.hpp>
#include <collisions/CollisionData.hpp>
#include <components/HealthComponent.hpp>

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
