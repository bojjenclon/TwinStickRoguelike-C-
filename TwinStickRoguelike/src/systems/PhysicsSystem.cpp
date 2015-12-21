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
  world.Step(1 / 60.0f, 6, 2);

  IteratingSystem::update(p_dt);
}

void PhysicsSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cPhysics = p_entity->get<PhysicsComponent>();
  auto cRender = p_entity->get<RenderComponent>();
  
  auto body = cPhysics->body;
  auto sprite = dynamic_cast<sf::Sprite*>(cRender->drawable);

  sprite->setPosition(body->GetPosition().x * Game::PIXELS_PER_METER, body->GetPosition().y * Game::PIXELS_PER_METER);
  sprite->setRotation(body->GetAngle());

  if (cPhysics->hasMaxSpeed)
  {
    if (body->GetLinearVelocity().x < -cPhysics->maxSpeed.x)
    {
      body->SetLinearVelocity(b2Vec2(-cPhysics->maxSpeed.x, body->GetLinearVelocity().y));
    }
    else if (body->GetLinearVelocity().x > cPhysics->maxSpeed.x)
    {
      body->SetLinearVelocity(b2Vec2(cPhysics->maxSpeed.x, body->GetLinearVelocity().y));
    }

    if (body->GetLinearVelocity().y < -cPhysics->maxSpeed.y)
    {
      body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, -cPhysics->maxSpeed.y));
    }
    else if (body->GetLinearVelocity().y > cPhysics->maxSpeed.y)
    {
      body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, cPhysics->maxSpeed.y));
    }
  }
}