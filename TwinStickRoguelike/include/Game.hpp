#ifndef GAME_HPP
#define GAME_HPP

#include <chrono>
#include <SDL.h>
#include <include/cef_app.h>
#include <ecstasy/core/Engine.h>

#include "ResourceManager.hpp"
#include "UIValues.hpp"
#include "cef/BrowserClient.hpp"
#include "cef/RenderHandler.hpp"

class Game
{
public:
  Game();

  bool start();
  void mainLoop();
  void quit();

  SDL_Window* getWindow() const;
  SDL_Renderer* getRenderer() const;

  float getScaleX() const;
  float getScaleY() const;

  ECS::Entity* getPlayer() const;

  static std::string GetApplicationDir();

public:
  static const int SCREEN_WIDTH = 1024;
  static const int SCREEN_HEIGHT = 768;
  static const int SCREEN_FPS = 60;
  static const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

private:
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;

  SDL_Texture* m_uiTexture = nullptr;

  CefRefPtr<RenderHandler> m_uiRenderHandler;
  CefRefPtr<CefBrowser> m_uiBrowser;
  CefRefPtr<BrowserClient> m_uiBrowserClient;

  float m_scaleX = 1.0f, m_scaleY = 1.0f;

  std::chrono::high_resolution_clock::time_point m_timePrev;
  float m_delta = 0;

  ResourceManager* m_resources = nullptr;

  UIValues* m_uiValues = nullptr;

  ECS::Engine* m_engine = nullptr;
  ECS::Entity* m_player = nullptr;
};

#endif

