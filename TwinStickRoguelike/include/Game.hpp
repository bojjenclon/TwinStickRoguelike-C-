#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <ecstasy/core/Engine.h>
#include <Box2D/Box2d.h>
#include <ResourceManager.hpp>
#include <UIValues.hpp>
#include <js/JSHandler.hpp>
#include <tiled/TiledTileLayerDrawable.hpp>
#include <GameMap.hpp>

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

  void addMap(GameMap* p_map, bool p_active = false);
  void changeMap(GameMap* p_gameMap, const sf::Vector2f& p_playerPosition);

  static Game& Get();
  static std::string GetApplicationDir();

private:
  Game();

  sf::RenderWindow m_window;
  sf::Sprite m_uiSprite;

  sf::RenderTexture m_mapTexture;
  sf::Sprite m_mapSprite;

  std::vector<GameMap*> m_maps;
  GameMap* m_previousMap = nullptr;
  GameMap* m_currentMap = nullptr;
  bool m_mapChanged = false;
  sf::Vector2f m_newPlayerPosition;

  WebCore* m_webCore = nullptr;
  WebSession* m_webSession = nullptr;
  WebView* m_webView = nullptr;
  BitmapSurface* m_uiSurface = nullptr;
  unsigned char* m_uiRGBABuffer = nullptr;
  sf::Texture* m_uiTexture = nullptr;

  JSValue m_jsApp;
  JSHandler* m_jsHandler = nullptr;

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

