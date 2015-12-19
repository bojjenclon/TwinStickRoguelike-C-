#include "Game.hpp"

#include <iostream>
#include <algorithm>

#include <EntityFactory.hpp>
#include <systems/RenderSystem.hpp>
#include <systems/UIUpdateSystem.hpp>
#include <cef/BrowserApp.hpp>
#include <systems/PlayerInputSystem.hpp>
#include <systems/AnimationSystem.hpp>

Game::Game()
{
}

bool Game::start()
{
  CefMainArgs args(GetModuleHandle(nullptr));

  CefRefPtr<BrowserApp> app(new BrowserApp());

  auto exit_code = CefExecuteProcess(args, app.get(), nullptr);
  if (exit_code >= 0)
  {
    return false;
  }

  m_window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "SFML + CEF3");
  m_window.setFramerateLimit(60);

  CefSettings settings;
  settings.multi_threaded_message_loop = false;
  settings.single_process = false;

  auto result = CefInitialize(args, settings, app.get(), nullptr);

  if (!result)
  {
    // handle error
    return false;
  }

  auto path = "file://" + GetApplicationDir() + "/../html/Blank.html";
  //std::string               path = "http://deanm.github.io/pre3d/monster.html";
  //std::string               path = "http://www.google.com";
  //std::string               path = "http://www.bojjenclon.com";

  auto command_line = CefCommandLine::GetGlobalCommandLine();
#ifndef _DEBUG
  command_line->AppendSwitch("off-screen-rendering-enabled");
  //command_line->AppendSwitchWithValue("off-screen-frame-rate", "60");
  command_line->AppendSwitch("disable-gpu");
  //command_line->AppendSwitch("enable-media-stream");
  command_line->AppendSwitch("disable-gpu-compositing");
  command_line->AppendSwitch("enable-begin-frame-scheduling");
#endif

  if (command_line->HasSwitch("url"))
  {
    path = command_line->GetSwitchValue("url");
  }

  auto uiTexture = new sf::Texture();
  uiTexture->create(SCREEN_WIDTH, SCREEN_HEIGHT);
  m_uiSprite.setTexture(*uiTexture);

  loadMedia();

  m_uiRenderHandler = new RenderHandler(uiTexture);

  CefWindowInfo window_info;
  CefBrowserSettings browserSettings;

  window_info.windowless_rendering_enabled = true;
  window_info.transparent_painting_enabled = true;

  m_uiBrowserClient = new BrowserClient(m_uiRenderHandler, m_uiValues);
  m_uiBrowser = CefBrowserHost::CreateBrowserSync(window_info, m_uiBrowserClient.get(), path, browserSettings, nullptr);

  // Engine parameters: entityPoolInitialSize, entityPoolMaxSize, componentPoolInitialSize
  m_engine = new ECS::Engine(10, 100, 100);

  auto playerInputSystem = new PlayerInputSystem();
  m_engine->addSystem(playerInputSystem);

  auto animationSystem = new AnimationSystem();
  m_engine->addSystem(animationSystem);

  auto uiUpdateSystem = new UIUpdateSystem();
  m_engine->addSystem(uiUpdateSystem);

  auto renderSystem = new RenderSystem(m_window);
  m_engine->addSystem(renderSystem);

  auto uiContainer = EntityFactory::makeUIContainer(m_engine, m_uiSprite, m_uiBrowser, m_uiValues);
  m_engine->addEntity(uiContainer);

  m_player = EntityFactory::makePlayer(m_engine, m_resources);
  m_engine->addEntity(m_player);

  return true;
}

WPARAM sfkeyToWparam(sf::Keyboard::Key key)
{
  switch (key)
  {
    case sf::Keyboard::LControl: return VK_LCONTROL;
    case sf::Keyboard::RControl: return VK_RCONTROL;
    case sf::Keyboard::LShift: return VK_LSHIFT;
    case sf::Keyboard::RShift: return VK_RSHIFT;
    case sf::Keyboard::LAlt: return VK_LMENU;
    case sf::Keyboard::RAlt: return VK_RMENU;
    case sf::Keyboard::LSystem: return VK_LWIN;
    case sf::Keyboard::RSystem: return VK_RWIN;
    case sf::Keyboard::Menu: return VK_APPS;
    case sf::Keyboard::SemiColon: return VK_OEM_1 ;
    case sf::Keyboard::Slash: return VK_OEM_2 ;
    case sf::Keyboard::Equal: return VK_OEM_PLUS ;
    case sf::Keyboard::Dash: return VK_OEM_MINUS ;
    case sf::Keyboard::LBracket: return VK_OEM_4 ;
    case sf::Keyboard::RBracket: return VK_OEM_6 ;
    case sf::Keyboard::Comma: return VK_OEM_COMMA ;
    case sf::Keyboard::Period: return VK_OEM_PERIOD ;
    case sf::Keyboard::Quote: return VK_OEM_7 ;
    case sf::Keyboard::BackSlash: return VK_OEM_5 ;
    case sf::Keyboard::Tilde: return VK_OEM_3 ;
    case sf::Keyboard::Escape: return VK_ESCAPE;
    case sf::Keyboard::Space: return VK_SPACE;
    case sf::Keyboard::Return: return VK_RETURN;
    case sf::Keyboard::BackSpace: return VK_BACK;
    case sf::Keyboard::Tab: return VK_TAB;
    case sf::Keyboard::PageUp: return VK_PRIOR;
    case sf::Keyboard::PageDown: return VK_NEXT;
    case sf::Keyboard::End: return VK_END;
    case sf::Keyboard::Home: return VK_HOME;
    case sf::Keyboard::Insert: return VK_INSERT;
    case sf::Keyboard::Delete: return VK_DELETE;
    case sf::Keyboard::Add: return VK_ADD;
    case sf::Keyboard::Subtract: return VK_SUBTRACT;
    case sf::Keyboard::Multiply: return VK_MULTIPLY;
    case sf::Keyboard::Divide: return VK_DIVIDE;
    case sf::Keyboard::Pause: return VK_PAUSE;
    case sf::Keyboard::F1: return VK_F1;
    case sf::Keyboard::F2: return VK_F2;
    case sf::Keyboard::F3: return VK_F3;
    case sf::Keyboard::F4: return VK_F4;
    case sf::Keyboard::F5: return VK_F5;
    case sf::Keyboard::F6: return VK_F6;
    case sf::Keyboard::F7: return VK_F7;
    case sf::Keyboard::F8: return VK_F8;
    case sf::Keyboard::F9: return VK_F9;
    case sf::Keyboard::F10: return VK_F10;
    case sf::Keyboard::F11: return VK_F11;
    case sf::Keyboard::F12: return VK_F12;
    case sf::Keyboard::F13: return VK_F13;
    case sf::Keyboard::F14: return VK_F14;
    case sf::Keyboard::F15: return VK_F15;
    case sf::Keyboard::Left: return VK_LEFT;
    case sf::Keyboard::Right: return VK_RIGHT;
    case sf::Keyboard::Up: return VK_UP;
    case sf::Keyboard::Down: return VK_DOWN;
    case sf::Keyboard::Numpad0: return VK_NUMPAD0;
    case sf::Keyboard::Numpad1: return VK_NUMPAD1;
    case sf::Keyboard::Numpad2: return VK_NUMPAD2;
    case sf::Keyboard::Numpad3: return VK_NUMPAD3;
    case sf::Keyboard::Numpad4: return VK_NUMPAD4;
    case sf::Keyboard::Numpad5: return VK_NUMPAD5;
    case sf::Keyboard::Numpad6: return VK_NUMPAD6;
    case sf::Keyboard::Numpad7: return VK_NUMPAD7;
    case sf::Keyboard::Numpad8: return VK_NUMPAD8;
    case sf::Keyboard::Numpad9: return VK_NUMPAD9;
    case sf::Keyboard::A: return 'A';
    case sf::Keyboard::B: return 'B';
    case sf::Keyboard::C: return 'C';
    case sf::Keyboard::D: return 'D';
    case sf::Keyboard::E: return 'E';
    case sf::Keyboard::F: return 'F';
    case sf::Keyboard::G: return 'G';
    case sf::Keyboard::H: return 'H';
    case sf::Keyboard::I: return 'I';
    case sf::Keyboard::J: return 'J';
    case sf::Keyboard::K: return 'K';
    case sf::Keyboard::L: return 'L';
    case sf::Keyboard::M: return 'M';
    case sf::Keyboard::N: return 'N';
    case sf::Keyboard::O: return 'O';
    case sf::Keyboard::P: return 'P';
    case sf::Keyboard::Q: return 'Q';
    case sf::Keyboard::R: return 'R';
    case sf::Keyboard::S: return 'S';
    case sf::Keyboard::T: return 'T';
    case sf::Keyboard::U: return 'U';
    case sf::Keyboard::V: return 'V';
    case sf::Keyboard::W: return 'W';
    case sf::Keyboard::X: return 'X';
    case sf::Keyboard::Y: return 'Y';
    case sf::Keyboard::Z: return 'Z';
    case sf::Keyboard::Num0: return '0';
    case sf::Keyboard::Num1: return '1';
    case sf::Keyboard::Num2: return '2';
    case sf::Keyboard::Num3: return '3';
    case sf::Keyboard::Num4: return '4';
    case sf::Keyboard::Num5: return '5';
    case sf::Keyboard::Num6: return '6';
    case sf::Keyboard::Num7: return '7';
    case sf::Keyboard::Num8: return '8';
    case sf::Keyboard::Num9: return '9';
  }

  return VK_NONAME;
}

int GetKeyboardModifiers()
{
  int mod = 0;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
  {
    mod |= EVENTFLAG_CONTROL_DOWN;// mod |= EVENTFLAG_IS_LEFT;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
  {
    mod |= EVENTFLAG_CONTROL_DOWN;// mod |= EVENTFLAG_IS_RIGHT;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
  {
    mod |= EVENTFLAG_SHIFT_DOWN;// mod |= EVENTFLAG_IS_LEFT;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
  {
    mod |= EVENTFLAG_SHIFT_DOWN;// mod |= EVENTFLAG_IS_RIGHT;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
  {
    mod |= EVENTFLAG_ALT_DOWN;// mod |= EVENTFLAG_IS_LEFT;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
  {
    mod |= EVENTFLAG_ALT_DOWN;// mod |= EVENTFLAG_IS_RIGHT;
  }

  return mod;
}

void Game::mainLoop()
{
  sf::Clock deltaClock;

  sf::Clock clickClock;
  clickClock.restart();
  auto clickTime = 0.25f;
  auto lastClickType = sf::Mouse::Left;
  auto clickCount = 1;

  sf::Font font;
  font.loadFromFile("Adventure Subtitles.ttf");

  sf::Text fpsText("FPS: 60", font);
  fpsText.setCharacterSize(16);
  fpsText.setStyle(sf::Text::Bold);
  fpsText.setColor(sf::Color::Red);
  fpsText.setPosition(5, SCREEN_HEIGHT - 21);

  auto fpsEntity = EntityFactory::makeDrawable(m_engine, fpsText, -10);
  m_engine->addEntity(fpsEntity);

  while (m_window.isOpen())
  {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        m_window.close();
      }

      m_uiBrowser->GetHost()->SendFocusEvent(true);

      if (event.type == sf::Event::MouseButtonPressed)
      {
        if (clickClock.getElapsedTime().asSeconds() < clickTime && event.mouseButton.button == lastClickType)
        {
          clickCount++;
        }
        else
        {
          clickCount = 1;
        }

        lastClickType = event.mouseButton.button;

        auto mousePosition = sf::Mouse::getPosition(m_window);
        uint32 modifiers = GetKeyboardModifiers();

        auto point = m_window.mapPixelToCoords(mousePosition);

        CefMouseEvent cefEvent({ static_cast<int>(point.x), static_cast<int>(point.y), modifiers });
        auto type = event.mouseButton.button == sf::Mouse::Left ? MBT_LEFT : event.mouseButton.button == sf::Mouse::Right ? MBT_RIGHT : MBT_MIDDLE;

        m_uiBrowser->GetHost()->SendMouseMoveEvent(cefEvent, false);
        m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, type, false, clickCount);

        clickClock.restart();
      }
      else if (event.type == sf::Event::MouseButtonReleased)
      {
        auto mousePosition = sf::Mouse::getPosition(m_window);
        uint32 modifiers = GetKeyboardModifiers();

        auto point = m_window.mapPixelToCoords(mousePosition);

        CefMouseEvent cefEvent({ static_cast<int>(point.x), static_cast<int>(point.y), modifiers });
        auto type = event.mouseButton.button == sf::Mouse::Left ? MBT_LEFT : MBT_RIGHT;

        m_uiBrowser->GetHost()->SendMouseMoveEvent(cefEvent, false);
        m_uiBrowser->GetHost()->SendMouseClickEvent(cefEvent, type, true, clickCount);
      }
      else if (event.type == sf::Event::MouseMoved)
      {
        auto mousePosition = sf::Mouse::getPosition(m_window);
        uint32 modifiers = GetKeyboardModifiers();

        auto point = m_window.mapPixelToCoords(mousePosition);

        CefMouseEvent cefEvent({ static_cast<int>(point.x), static_cast<int>(point.y), modifiers });

        m_uiBrowser->GetHost()->SendMouseMoveEvent(cefEvent, false);
      }
      else if (event.type == sf::Event::MouseWheelScrolled)
      {
        auto mousePosition = sf::Mouse::getPosition(m_window);
        uint32 modifiers = GetKeyboardModifiers();

        auto point = m_window.mapPixelToCoords(mousePosition);

        CefMouseEvent cefEvent({ static_cast<int>(point.x), static_cast<int>(point.y), modifiers });

        m_uiBrowser->GetHost()->SendMouseWheelEvent(cefEvent, 0, event.mouseWheelScroll.delta * 10);
      }
      else if (event.type == sf::Event::KeyPressed)
      {
        auto key = sfkeyToWparam(event.key.code);

        if (key != VK_NONAME)
        {
          uint32 modifiers = GetKeyboardModifiers();

          CefKeyEvent e;
          e.windows_key_code = key;
          e.modifiers = modifiers == -1 ? GetKeyboardModifiers() : modifiers;
          e.type = KEYEVENT_KEYDOWN;
          e.is_system_key = false;
          e.character = key;
          e.unmodified_character = key;
          //e.native_key_code = 0;

          m_uiBrowser->GetHost()->SendKeyEvent(e);
        }
      }
      else if (event.type == sf::Event::KeyReleased)
      {
        if (event.key.code == sf::Keyboard::A)
        {
          m_uiValues.currentHealth--;
          m_uiValues.healthChanged = true;

          if (m_uiValues.currentHealth < 0)
          {
            m_uiValues.currentHealth = 0;
          }
        }

        auto key = sfkeyToWparam(event.key.code);

        if (key != VK_NONAME)
        {
          uint32 modifiers = GetKeyboardModifiers();

          CefKeyEvent e;
          e.windows_key_code = key;
          e.modifiers = modifiers == -1 ? GetKeyboardModifiers() : modifiers;
          e.type = KEYEVENT_KEYUP;
          e.is_system_key = false;
          e.character = key;
          e.unmodified_character = key;
          //e.native_key_code = 0;

          m_uiBrowser->GetHost()->SendKeyEvent(e);
        }
      }
      else if (event.type == sf::Event::TextEntered)
      {
        auto key = static_cast<WPARAM>(static_cast<char>(event.text.unicode));
        uint32 modifiers = GetKeyboardModifiers();

        CefKeyEvent e;
        e.windows_key_code = key;
        e.modifiers = modifiers == -1 ? GetKeyboardModifiers() : modifiers;
        e.type = KEYEVENT_CHAR;
        e.character = key;
        e.unmodified_character = key;

        m_uiBrowser->GetHost()->SendKeyEvent(e);
      }
    }

    m_uiRenderHandler->update();
    m_uiRenderHandler->updateTexture();

    auto dt = deltaClock.restart();

    fpsText.setString("FPS: " + std::to_string(1 / dt.asSeconds()));

    m_window.clear(sf::Color::White);

    m_engine->update(dt.asMilliseconds() / 1000.f);
    
    m_window.display();
  }
}

void Game::quit()
{
  /*auto uiEntities = m_engine->getEntitiesFor(ECS::Family::all<UIComponent>().get());
  for (auto i = 0; i < uiEntities->size(); i++)
  {
    uiEntities->at(i)->get<UIComponent>()->uiBrowser = nullptr;
  }*/

  m_uiBrowser->GetHost()->CloseBrowser(true);

  m_uiRenderHandler = nullptr;
  m_uiBrowser = nullptr;
  m_uiBrowserClient = nullptr;

  CefShutdown();

  m_engine = nullptr;
}

void Game::loadMedia()
{
  m_resources.loadTexture("deer", "deer.png");
}

const sf::RenderWindow& Game::getWindow() const
{
  return m_window;
}

ECS::Entity* Game::getPlayer() const
{
  return m_player;
}

std::string Game::GetApplicationDir()
{
  auto hModule = GetModuleHandleW(nullptr);
  WCHAR wpath[MAX_PATH];

  GetModuleFileNameW(hModule, wpath, MAX_PATH);
  std::wstring wide(wpath);

  std::string path = CefString(wide);
  std::replace(path.begin(), path.end(), '\\', '/');
  path = path.substr(0, path.find_last_of("\\/"));

  return path;
}

