#include <systems/PlayerInputSystem.hpp>
#include <components/PlayerComponent.hpp>
#include <components/RenderComponent.hpp>

PlayerInputSystem::PlayerInputSystem() : IteratingSystem(ECS::Family::all<PlayerComponent, RenderComponent>().get())
{
}

void PlayerInputSystem::processEntity(ECS::Entity* p_entity, float p_dt)
{
  auto cRender = p_entity->get<RenderComponent>();

  auto sprite = static_cast<sf::Sprite*>(cRender->drawable);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
  {
    sprite->move(-60 / p_dt, 0);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
  {
    sprite->move(60 / p_dt, 0);
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
  {
    sprite->move(0, -60 / p_dt);
  }
  else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
  {
    sprite->move(0, 60 / p_dt);
  }
}