#include <systems/PhysicsDebugDrawSystem.hpp>

#include <components/PhysicsComponent.hpp>
#include <Game.hpp>

PhysicsDebugDrawSystem::PhysicsDebugDrawSystem(sf::RenderTarget& p_renderTarget) : IteratingSystem(ECS::Family::all<PhysicsComponent>().get()), m_renderTarget(p_renderTarget)
{
}

void PhysicsDebugDrawSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cPhysics = p_entity->get<PhysicsComponent>();

  auto body = cPhysics->body;
  auto shape = body->GetFixtureList()->GetShape();

  if (shape->GetType() == b2Shape::Type::e_circle)
  {
    auto radius = shape->m_radius;
    radius *= Game::PIXELS_PER_METER;

    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setOrigin(radius, radius);
    circle.setPosition(body->GetPosition().x * Game::PIXELS_PER_METER, body->GetPosition().y * Game::PIXELS_PER_METER);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(1);
    m_renderTarget.draw(circle);
  }
}
