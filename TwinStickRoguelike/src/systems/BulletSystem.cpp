#include <systems/BulletSystem.hpp>
#include <components/BulletComponent.hpp>

BulletSystem::BulletSystem() : IteratingSystem(ECS::Family::all<BulletComponent>().get())
{
}

void BulletSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cBullet = p_entity->get<BulletComponent>();

  if (cBullet->updateCallback != nullptr)
  {
    cBullet->updateCallback(p_entity, p_dt);
  }
}