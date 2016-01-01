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

public:
  static const int SCREEN_WIDTH = 1024;
  static const int SCREEN_HEIGHT = 768;
  static const int SCREEN_FPS = 60;
  static const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

  static const float PIXELS_PER_METER;

private:
  Game();

  sf::RenderWindow m_window;
  sf::Sprite m_uiSprite;

  WebCore* m_webCore;
  WebSession* m_webSession;
  WebView* m_webView;
  BitmapSurface* m_uiSurface;
  unsigned char* m_uiRGBABuffer;
  sf::Texture* m_uiTexture;

  ResourceManager m_resources;

  UIValues m_uiValues;

  std::unique_ptr<ECS::Engine> m_engine = nullptr;
  ECS::Entity* m_player = nullptr;

  std::unique_ptr<b2World> m_world;

  ECS::Entity* m_target = nullptr;

public:
  Game(Game const&) = delete;
  void operator=(Game const&) = delete;
};

#endif

