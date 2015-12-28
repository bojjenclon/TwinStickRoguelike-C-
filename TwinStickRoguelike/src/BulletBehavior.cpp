#include <BulletBehavior.hpp>
#include <collisions/BulletCollisionData.hpp>
#include <components/HitOnceComponent.hpp>
#include <Game.hpp>
#include <components/HealthComponent.hpp>
#include <EntityInfo.hpp>

void BulletBehavior::basicBulletCollision(CollisionData* p_dataA, CollisionData* p_dataB)
{
  // make sure that any enemy or the player is being hit, as they're all that matter for the basic bullet
  if (p_dataA->type != EntityInfo::Type::Enemy && p_dataB->type != EntityInfo::Type::Enemy && p_dataA->type != EntityInfo::Type::Player && p_dataB->type != EntityInfo::Type::Player)
  {
    return;
  }

  auto bulletCollisionData = p_dataA->type == EntityInfo::Type::Bullet ? static_cast<BulletCollisionData*>(p_dataA) : static_cast<BulletCollisionData*>(p_dataB);
  auto entityCollisionData = p_dataA->type == EntityInfo::Type::Enemy ? p_dataA : p_dataB;

  // make sure the bullet doesn't affect its owner (based on type, so enemies can't hit each other)
  if (bulletCollisionData->owner == entityCollisionData->type)
  {
    return;
  }

  auto cHitOnce = bulletCollisionData->entity->get<HitOnceComponent>();
  if (find(cHitOnce->alreadyHit.begin(), cHitOnce->alreadyHit.end(), entityCollisionData->entity->getId()) == cHitOnce->alreadyHit.end())
  {
    auto cHealth = entityCollisionData->entity->get<HealthComponent>();
    cHealth->currentHealth--;

    cHitOnce->alreadyHit.push_back(entityCollisionData->entity->getId());

    if (bulletCollisionData->owner == EntityInfo::Player && entityCollisionData->type == EntityInfo::Enemy)
    {
      auto& game = Game::Get();
      game.setTarget(entityCollisionData->entity);

      if (cHealth->currentHealth <= 0)
      {
        game.clearTarget();
      }
    }
  }
}
