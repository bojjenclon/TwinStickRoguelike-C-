#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <include/cef_app.h>
#include <ecstasy/core/Engine.h>

#include <ResourceManager.hpp>
#include <UIValues.hpp>
#include <cef/BrowserClient.hpp>
#include <cef/RenderHandler.hpp>

class Game
{
public:
  Game();

  bool start();
  void mainLoop();
  void quit();

  void loadMedia();

  void handleBrowserEvents(sf::Event& p_event);

  const sf::RenderWindow& getWindow() const;
  ECS::Entity* getPlayer() const;

  static std::string GetApplicationDir();

public:
  static const int SCREEN_WIDTH = 1024;
  static const int SCREEN_HEIGHT = 768;
  static const int SCREEN_FPS = 60;
  static const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

private:
  sf::RenderWindow m_window;
  sf::Sprite m_uiSprite;

  CefRefPtr<RenderHandler> m_uiRenderHandler;
  CefRefPtr<CefBrowser> m_uiBrowser;
  CefRefPtr<BrowserClient> m_uiBrowserClient;

  ResourceManager m_resources;

  UIValues m_uiValues;

  std::unique_ptr<ECS::Engine> m_engine = nullptr;
  ECS::Entity* m_player = nullptr;

  sf::Clock m_clickClock;
  float m_clickTime;
  sf::Mouse::Button m_lastClickType;
  int m_clickCount;
};

#endif

