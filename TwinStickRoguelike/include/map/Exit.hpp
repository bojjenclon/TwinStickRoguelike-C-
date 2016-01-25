#ifndef EXIT_HPP
#define EXIT_HPP

#include <SFML/System/Vector2.hpp>
#include <map/ExitDirection.hpp>

// forward declaration
struct GameMap;

class Exit
{
public:
  Exit();
  Exit(const sf::Vector2f& p_position, const sf::Vector2f& p_size, const ExitDirection& p_direction, GameMap* p_destination = nullptr);

  const sf::Vector2f& getPosition() const;
  void setPosition(const sf::Vector2f& p_position);

  const sf::Vector2f& getSize() const;
  void setSize(const sf::Vector2f& p_size);

  const ExitDirection& getDirection() const;
  void setDirection(const ExitDirection& p_direction);

  GameMap* getDestination() const;
  void setDesintation(GameMap* p_destination);

private:
  sf::Vector2f m_position;
  sf::Vector2f m_size;
  ExitDirection m_direction;
  GameMap* m_destination = nullptr;
};

#endif
