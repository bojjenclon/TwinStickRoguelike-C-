#include <systems/RenderSystem.hpp>

#include <ecstasy/core/Family.h>
#include <SFML/Graphics.hpp>
#include <components/PhysicsComponent.hpp>

RenderSystem::RenderSystem(sf::RenderTarget& p_renderTarget) : SortedIteratingSystem(ECS::Family::all<RenderComponent>().get(), DepthTest()), m_renderTarget(p_renderTarget)
{
}

void RenderSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cRender = p_entity->get<RenderComponent>();

  m_renderTarget.draw(*cRender->drawable);

  /*if (p_entity->has<PhysicsComponent>())
  {
    auto cPhysics = p_entity->get<PhysicsComponent>();
    auto body = cPhysics->body;
    //auto sprite = dynamic_cast<sf::Sprite*>(cRender->drawable);

    auto radius = body->GetFixtureList()->GetShape()->m_radius;
    radius *= Game::PIXELS_PER_METER;

    sf::CircleShape circle;
    circle.setRadius(radius);
    circle.setOrigin(radius / 2, radius / 2);
    //circle.setOrigin(sprite->getOrigin());
    circle.setPosition(body->GetPosition().x * Game::PIXELS_PER_METER, body->GetPosition().y * Game::PIXELS_PER_METER);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Red);
    circle.setOutlineThickness(1);
    m_renderTarget.draw(circle);
  }*/
}
