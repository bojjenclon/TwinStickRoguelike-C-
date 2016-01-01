#include <systems/PhysicsSystem.hpp>
#include <components/PhysicsComponent.hpp>
#include <components/RenderComponent.hpp>
#include <Constants.hpp>

// based on: http://saltares.com/blog/games/fixing-your-timestep-in-libgdx-and-box2d/

PhysicsSystem::PhysicsSystem(std::unique_ptr<b2World>& p_world) : IteratingSystem(ECS::Family::all<PhysicsComponent, RenderComponent>().get()), m_world(p_world)
{
  m_fixedStepLimit = 1 / 60.0f;
  m_accumulator = 0;
}

void PhysicsSystem::update(float p_dt)
{
  if (p_dt > 0.25f)
  {
    p_dt = 0.25f;	  // note: max frame time to avoid spiral of death
  }

  m_accumulator += p_dt;

  while (m_accumulator >= m_fixedStepLimit) {
    copyCurrentPosition();

    m_world->Step(m_fixedStepLimit, 10, 8);
    m_accumulator -= m_fixedStepLimit;

    IteratingSystem::update(p_dt);
  }
}

void PhysicsSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cPhysics = p_entity->get<PhysicsComponent>();
  auto body = cPhysics->body;

  if (body->GetType() != b2_dynamicBody || !body->IsActive())
  {
    return;
  }

  auto cRender = p_entity->get<RenderComponent>();
  auto sprite = dynamic_cast<sf::Sprite*>(cRender->drawable);

  auto alpha = m_accumulator / m_fixedStepLimit;

  auto interpolatedX = body->GetPosition().x * alpha + cPhysics->previousPosition.x * (1.0f - alpha);
  auto interpolatedY = body->GetPosition().y * alpha + cPhysics->previousPosition.y * (1.0f - alpha);
  auto interpolatedAngle = body->GetAngle() * alpha + cPhysics->previousAngle * (1.0f - alpha);

  sprite->setPosition(interpolatedX * Constants::PIXELS_PER_METER, interpolatedY * Constants::PIXELS_PER_METER);
  sprite->setRotation(interpolatedAngle);

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

void PhysicsSystem::copyCurrentPosition() const
{
  auto entities = this->getEntities();
  
  for (auto it = entities->begin(); it != entities->end(); ++it)
  {
    auto entity = *it;
    auto cPhysics = entity->get<PhysicsComponent>();

    auto body = cPhysics->body;

    cPhysics->previousPosition.x = body->GetPosition().x;
    cPhysics->previousPosition.y = body->GetPosition().y;
    cPhysics->previousAngle = body->GetAngle();
  }
}
