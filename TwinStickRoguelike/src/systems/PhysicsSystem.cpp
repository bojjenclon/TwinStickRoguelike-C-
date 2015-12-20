#include <systems/PhysicsSystem.hpp>
#include <components/PhysicsComponent.hpp>
#include <components/RenderComponent.hpp>
#include <Game.hpp>

PhysicsSystem::PhysicsSystem() : IteratingSystem(ECS::Family::all<PhysicsComponent, RenderComponent>().get())
{
}

void PhysicsSystem::update(float p_dt)
{
  auto& world = Game::Get().getWorld();
  world.Step(p_dt, 6, 2);

  IteratingSystem::update(p_dt);
}

void PhysicsSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cPhysics = p_entity->get<PhysicsComponent>();
  auto cRender = p_entity->get<RenderComponent>();
  
  auto body = cPhysics->body;
  auto sprite = dynamic_cast<sf::Sprite*>(cRender->drawable);

  sprite->setPosition(body->GetPosition().x, body->GetPosition().y);
  sprite->setRotation(body->GetAngle());
}