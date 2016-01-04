#include <systems/NodeWatchSystem.hpp>
#include <components/NodeWatchComponent.hpp>
#include <components/PhysicsComponent.hpp>
#include <Constants.hpp>

NodeWatchSystem::NodeWatchSystem() : IteratingSystem(ECS::Family::all<NodeWatchComponent, PhysicsComponent>().get())
{
}

void NodeWatchSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cNodeWatch = p_entity->get<NodeWatchComponent>();
  auto cPhysics = p_entity->get<PhysicsComponent>();

  auto body = cPhysics->body;

  auto x = static_cast<int>(body->GetPosition().x * Constants::PIXELS_PER_METER / Constants::COLLISION_TILE_WIDTH);
  auto y = static_cast<int>(body->GetPosition().y * Constants::PIXELS_PER_METER / Constants::COLLISION_TILE_HEIGHT);

  cNodeWatch->nodeChanged = (x != cNodeWatch->previousX || y != cNodeWatch->previousY);

  if (cNodeWatch->nodeChanged)
  {
    cNodeWatch->previousX = x;
    cNodeWatch->previousY = y;
  }
}