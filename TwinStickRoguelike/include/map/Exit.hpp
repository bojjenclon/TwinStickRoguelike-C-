#ifndef EXIT_HPP
#define EXIT_HPP

#include <SFML/System/Vector2.hpp>

// forward declaration
class TiledMap;

class Exit
{
public:
  Exit();
  Exit(const sf::Vector2f& p_position, const sf::Vector2f& p_size, TiledMap* p_destination = nullptr);

  const sf::Vector2f& getPosition() const;
  void setPosition(const sf::Vector2f& p_position);

  const sf::Vector2f& getSize() const;
  void setSize(const sf::Vector2f& p_size);

  TiledMap* getDestination() const;
  void setDesintation(TiledMap* p_destination);

private:
  sf::Vector2f m_position;
  sf::Vector2f m_size;
  TiledMap* m_destination = nullptr;
};

#endif
