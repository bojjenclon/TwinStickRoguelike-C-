#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <SFML/Graphics.hpp>
#include <include/cef_app.h>
#include <ecstasy/core/Engine.h>

#include "ResourceManager.hpp"
#include "UIValues.hpp"
#include "cef/WebSystem.hpp"

class Game
{
public:
  Game();

  bool start();
  void mainLoop();
  void quit();

  const sf::RenderWindow& getWindow() const;

  ECS::Entity* getPlayer() const;

  static std::string GetApplicationDir();

public:
  static const int SCREEN_WIDTH = 1024;
  static const int SCREEN_HEIGHT = 768;
  static const int SCREEN_FPS = 60;

private:
  sf::RenderWindow m_window;
  sf::Sprite m_uiSprite;

  CefRefPtr<WebInterface> m_uiBrowser;

  ResourceManager* m_resources = nullptr;

  UIValues m_uiValues;

  ECS::Engine* m_engine = nullptr;
  ECS::Entity* m_player = nullptr;
};

#endif

