#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <ecstasy/core/Engine.h>
#include <Box2D/Box2d.h>
#include <ResourceManager.hpp>
#include <UIValues.hpp>

using namespace Awesomium;

class Game
{
public:
  bool start();
  void mainLoop();
  void quit();

  void loadMedia();

  void handleBrowserEvents(sf::Event& p_event) const;

  const sf::RenderWindow& getWindow() const;
  ECS::Entity* getPlayer() const;
  ECS::Engine& getEngine() const;
  b2World& getWorld() const;

  ECS::Entity* getTarget() const;
  void setTarget(ECS::Entity* p_target);
  void clearTarget();

  static Game& Get();
  static std::string GetApplicationDir();

private:
  Game();

  sf::RenderWindow m_window;
  sf::Sprite m_uiSprite;

  WebCore* m_webCore = nullptr;
  WebSession* m_webSession = nullptr;
  WebView* m_webView = nullptr;
  BitmapSurface* m_uiSurface = nullptr;
  unsigned char* m_uiRGBABuffer = nullptr;
  sf::Texture* m_uiTexture = nullptr;

  ResourceManager m_resources;

  UIValues m_uiValues;

  std::unique_ptr<ECS::Engine> m_engine = nullptr;
  ECS::Entity* m_player = nullptr;

  std::unique_ptr<b2World> m_world = nullptr;

  ECS::Entity* m_target = nullptr;

public:
  Game(Game const&) = delete;
  void operator=(Game const&) = delete;
};

#endif

