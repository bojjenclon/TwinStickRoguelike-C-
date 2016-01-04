#include <systems/PhysicsDebugDrawSystem.hpp>
#include <components/PhysicsComponent.hpp>
#include <Constants.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include <Box2D/Box2D.h>

PhysicsDebugDrawSystem::PhysicsDebugDrawSystem(sf::RenderTarget& p_renderTarget) : IteratingSystem(ECS::Family::all<PhysicsComponent>().get()), m_renderTarget(p_renderTarget)
{
}

void PhysicsDebugDrawSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cPhysics = p_entity->get<PhysicsComponent>();

  auto body = cPhysics->body;
  
  for (auto fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
  {
    auto shape = fixture->GetShape();
    
    if (shape->GetType() == b2Shape::Type::e_circle)
    {
      auto boxCircle = dynamic_cast<b2CircleShape*>(shape);

      auto radius = boxCircle->m_radius;
      radius *= Constants::PIXELS_PER_METER;
      
      sf::CircleShape circle;
      circle.setRadius(radius);
      circle.setOrigin(radius, radius);
      circle.setPosition(
        body->GetPosition().x * Constants::PIXELS_PER_METER + boxCircle->m_p.x * Constants::PIXELS_PER_METER,
        body->GetPosition().y * Constants::PIXELS_PER_METER + boxCircle->m_p.y * Constants::PIXELS_PER_METER
      );
      circle.setFillColor(sf::Color::Transparent);
      circle.setOutlineColor(sf::Color::Red);
      circle.setOutlineThickness(1);
      m_renderTarget.draw(circle);
    }
    else if (shape->GetType() == b2Shape::Type::e_polygon)
    {
      auto boxPolygon = dynamic_cast<b2PolygonShape*>(shape);
      
      sf::ConvexShape sfPolygon;
      sfPolygon.setPosition(body->GetPosition().x * Constants::PIXELS_PER_METER, body->GetPosition().y * Constants::PIXELS_PER_METER);
      sfPolygon.setFillColor(sf::Color::Transparent);
      sfPolygon.setOutlineColor(sf::Color::Red);
      sfPolygon.setOutlineThickness(1);

      sfPolygon.setPointCount(boxPolygon->GetVertexCount());
      for (auto i = 0; i < boxPolygon->GetVertexCount(); i++)
      {
        auto vertex = boxPolygon->GetVertex(i);

        sfPolygon.setPoint(i, sf::Vector2f(vertex.x * Constants::PIXELS_PER_METER, vertex.y * Constants::PIXELS_PER_METER));
      }

      m_renderTarget.draw(sfPolygon);
    }
  }
}
