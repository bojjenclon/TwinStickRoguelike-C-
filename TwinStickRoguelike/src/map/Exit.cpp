#include <map/Exit.hpp>

Exit::Exit()
{
  
}


Exit::Exit(const sf::Vector2f& p_position, const sf::Vector2f& p_size, GameMap* p_destination)
  : m_position(p_position), m_size(p_size), m_destination(p_destination)
{
  
}

const sf::Vector2f& Exit::getPosition() const
{
  return m_position;
}

void Exit::setPosition(const sf::Vector2f& p_position)
{
  m_position = p_position;
}

const sf::Vector2f& Exit::getSize() const
{
  return m_size;
}

void Exit::setSize(const sf::Vector2f& p_size)
{
  m_size = p_size;
}

GameMap* Exit::getDestination() const
{
  return m_destination;
}

void Exit::setDesintation(GameMap* p_destination)
{
  m_destination = p_destination;
}