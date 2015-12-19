#include <systems/MovementSystem.hpp>
#include <components/VelocityComponent.hpp>
#include <components/RenderComponent.hpp>

MovementSystem::MovementSystem() : IteratingSystem(ECS::Family::all<RenderComponent, VelocityComponent>().get())
{
}

void MovementSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cRender = p_entity->get<RenderComponent>();
  auto cVelocity = p_entity->get<VelocityComponent>();

  auto transform = dynamic_cast<sf::Transformable*>(cRender->drawable);

  if (transform == nullptr)
  {
    return;
  }

  transform->move(cVelocity->vx * p_dt, cVelocity->vy * p_dt);
}
