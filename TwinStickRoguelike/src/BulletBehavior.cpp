#include <BulletBehavior.hpp>
#include <collisions/BulletCollisionData.hpp>
#include <components/HitOnceComponent.hpp>
#include <Game.hpp>
#include <components/HealthComponent.hpp>
#include <EntityInfo.hpp>

void BulletBehavior::basicBulletCollision(CollisionData* p_dataA, CollisionData* p_dataB)
{
  auto bulletCollisionData = p_dataA->type == EntityInfo::Type::Bullet ? static_cast<BulletCollisionData*>(p_dataA) : static_cast<BulletCollisionData*>(p_dataB);
  auto enemyCollisionData = p_dataA->type == EntityInfo::Type::Enemy ? p_dataA : p_dataB;

  if (bulletCollisionData->owner == EntityInfo::Player)
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
