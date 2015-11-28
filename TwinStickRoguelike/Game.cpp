#include "Game.hpp"

#include <iostream>
#include <algorithm>
#include <SDL_image.h>

#include "EntityFactory.hpp"
#include "RenderSystem.hpp"
#include "BrowserApp.hpp"
#include "UIUpdateSystem.hpp"

Game::Game()
{
}

bool Game::start()
{
  CefMainArgs args(GetModuleHandle(nullptr));

  CefRefPtr<BrowserApp> app(new BrowserApp());

  int exit_code = CefExecuteProcess(args, app.get(), nullptr);
  if (exit_code >= 0)
  {
    return false;
  }

  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
    return false;
  }

  m_window = SDL_CreateWindow("SDL + CEF3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
  if (m_window == nullptr)
  {
    std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;

    SDL_Quit();

    return false;
  }
  else
  {
    //Create renderer for window
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == NULL)
    {
      printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
    }
    else
    {
      //Initialize renderer color
      SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);

      //Initialize PNG loading
      int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
      if (!(IMG_Init(imgFlags) & imgFlags))
      {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
      }
    }
  }

  m_resources = new ResourceManager(m_renderer);
  //loadMedia();

  CefSettings settings;
  bool result = CefInitialize(args, settings, app.get(), nullptr);

  if (!result)
  {
    // handle error
    return false;
  }

  std::string path = "file://" + GetApplicationDir() + "/../html/index.html";
  //std::string               path = "http://deanm.github.io/pre3d/monster.html";
  //std::string               path = "http://www.google.com";
  CefRefPtr<CefCommandLine> command_line = CefCommandLine::GetGlobalCommandLine();

  if (command_line->HasSwitch("url"))
  {
    path = command_line->GetSwitchValue("url");
  }

  m_uiTexture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);
  SDL_SetTextureBlendMode(m_uiTexture, SDL_BLENDMODE_BLEND);

  m_uiRenderHandler = new RenderHandler(m_uiTexture);

  CefWindowInfo window_info;
  CefBrowserSettings browserSettings;

  window_info.windowless_rendering_enabled = true;
  window_info.transparent_painting_enabled = true;

  m_uiValues = new UIValues();

  m_uiBrowserClient = new BrowserClient(m_uiRenderHandler, m_uiValues);
  m_uiBrowser = CefBrowserHost::CreateBrowserSync(window_info, m_uiBrowserClient.get(), path, browserSettings, nullptr);

  // Engine parameters: entityPoolInitialSize, entityPoolMaxSize, componentPoolInitialSize
  m_engine = new ECS::Engine(10, 100, 100);

  RenderSystem* renderSystem = new RenderSystem(this);
  m_engine->addSystem(renderSystem);

  UIUpdateSystem* uiUpdateSystem = new UIUpdateSystem();
  m_engine->addSystem(uiUpdateSystem);

  ECS::Entity* uiContainer = EntityFactory::makeUIContainer(m_engine, m_uiTexture, m_uiBrowser, m_uiValues);
  m_engine->addEntity(uiContainer);

  m_player = EntityFactory::makePlayer(m_engine, m_resources);
  m_engine->addEntity(m_player);

  return true;
}

void Game::mainLoop()
{
  SDL_Event e;
  bool quit = false;

  int hpBarWidth = 100;

  bool result = m_resources->loadTexture("test.png");
  SDL_Texture* tex = m_resources->getTexture("test.png");

  m_timePrev = std::chrono::high_resolution_clock::now();

  while (!quit)
  {
    while (SDL_PollEvent(&e) != 0)
    {
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
      else if (e.type == SDL_MOUSEBUTTONDOWN)
      {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        uint32 modifiers = 0;
        
        if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL])
        {
          modifiers = modifiers | EVENTFLAG_CONTROL_DOWN;
        }
        if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
        {
          modifiers = modifiers | EVENTFLAG_SHIFT_DOWN;
        }

        CefMouseEvent cefEvent({ static_cast<int>(e.button.x / m_scaleX), static_cast<int>(e.button.y / m_scaleY), modifiers });
        
        if (e.button.button == SDL_BUTTON_LEFT)
        {
          m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, MBT_LEFT, false, 1);
        }
        else if (e.button.button == SDL_BUTTON_RIGHT)
        {
          m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, MBT_RIGHT, false, 1);
        }
        else if (e.button.button == SDL_BUTTON_MIDDLE)
        {
          m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, MBT_MIDDLE, false, 1);
        }
      }
      else if (e.type == SDL_MOUSEBUTTONUP)
      {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        uint32 modifiers = 0;

        if (state[SDL_SCANCODE_LCTRL] || state[SDL_SCANCODE_RCTRL])
        {
          modifiers = modifiers | EVENTFLAG_CONTROL_DOWN;
        }
        if (state[SDL_SCANCODE_LSHIFT] || state[SDL_SCANCODE_RSHIFT])
        {
          modifiers = modifiers | EVENTFLAG_SHIFT_DOWN;
        }

        CefMouseEvent cefEvent({ static_cast<int>(e.button.x / m_scaleX), static_cast<int>(e.button.y / m_scaleY), modifiers });

        if (e.button.button == SDL_BUTTON_LEFT)
        {
          m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, MBT_LEFT, true, 1);
        }
        else if (e.button.button == SDL_BUTTON_RIGHT)
        {
          m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, MBT_RIGHT, true, 1);
        }
        else if (e.button.button == SDL_BUTTON_MIDDLE)
        {
          m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, MBT_MIDDLE, true, 1);
        }
      }
      else if (e.type == SDL_MOUSEMOTION)
      {
        CefMouseEvent cefEvent({ static_cast<int>(e.motion.x / m_scaleX), static_cast<int>(e.motion.y / m_scaleY), 0 });

        m_uiBrowser->GetHost()->SendMouseMoveEvent(cefEvent, false);
      }
      else if (e.type == SDL_MOUSEWHEEL)
      {
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        CefMouseEvent cefEvent({ mx, my, 0 });

        m_uiBrowser->GetHost()->SendMouseWheelEvent(cefEvent, e.wheel.x, e.wheel.y);
      }
      else if (e.type == SDL_KEYDOWN)
      {
        
      }
      else if (e.type == SDL_KEYUP)
      {        
        if (e.key.keysym.sym == SDLK_a)
        {
          m_uiValues->currentHealth--;
          m_uiValues->healthChanged = true;

          if (m_uiValues->currentHealth < 0)
          {
            m_uiValues->currentHealth = 0;
          }
        }
      }
      else if (e.type == SDL_WINDOWEVENT)
      {
        if (e.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          m_scaleX = static_cast<float>(e.window.data1) / SCREEN_WIDTH;
          m_scaleY = static_cast<float>(e.window.data2) / SCREEN_HEIGHT;

          //m_uiBrowser->GetHost()->WasResized();
        }
      }
    }

    auto timeCurrent = std::chrono::high_resolution_clock::now();
    auto timeDiff = std::chrono::duration_cast<std::chrono::nanoseconds>(timeCurrent - m_timePrev);
    m_delta = static_cast<float>(timeDiff.count());
    m_delta /= 1000000000;

    m_uiRenderHandler->update();

    m_engine->update(m_delta);

    m_timePrev = timeCurrent;
  }
}

void Game::quit()
{
  //SDL_DestroyTexture(gTexture);
  SDL_DestroyTexture(m_uiTexture);

  //m_resources->freeAllTextures();
  
  m_uiRenderHandler = nullptr;
  m_uiBrowser = nullptr;
  m_uiBrowserClient = nullptr;
  
  /*auto uiEntities = m_engine->getEntitiesFor(ECS::Family::all<UIComponent>().get());
  for (auto i = 0; i < uiEntities->size(); i++)
  {
    uiEntities->at(i)->get<UIComponent>()->uiBrowser = nullptr;
  }*/

  SetErrorMode(SEM_NOGPFAULTERRORBOX); // used to prevent DebugObjCt error, should really figure out why the error is being thrown instead of suppressing it
  CefShutdown();

  SDL_DestroyWindow(m_window);
  SDL_Quit();

  m_resources = nullptr;
  m_window = nullptr;
  m_engine = nullptr;
}

SDL_Window* Game::getWindow() const
{
  return m_window;
}

SDL_Renderer* Game::getRenderer() const
{
  return m_renderer;
}

float Game::getScaleX() const
{
  return m_scaleX;
}

float Game::getScaleY() const
{
  return m_scaleY;
}

ECS::Entity* Game::getPlayer() const
{
  return m_player;
}

std::string Game::GetApplicationDir()
{
  HMODULE hModule = GetModuleHandleW(nullptr);
  WCHAR wpath[MAX_PATH];

  GetModuleFileNameW(hModule, wpath, MAX_PATH);
  std::wstring wide(wpath);

  std::string path = CefString(wide);
  std::replace(path.begin(), path.end(), '\\', '/');
  path = path.substr(0, path.find_last_of("\\/"));

  return path;
}

