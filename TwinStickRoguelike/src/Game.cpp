#include <Game.hpp>

#include <iostream>
#include <algorithm>
#include <codecvt>
#include <Awesomium/STLHelpers.h>
#include <Awesomium/DataPak.h>
#include <Constants.hpp>
#include <factories/BasicEntityFactory.hpp>
#include <systems/RenderSystem.hpp>
#include <systems/UIUpdateSystem.hpp>
#include <systems/PlayerInputSystem.hpp>
#include <systems/AnimationSystem.hpp>
#include <systems/LifetimeSystem.hpp>
#include <systems/PhysicsSystem.hpp>
#include <systems/PhysicsDebugDrawSystem.hpp>
#include <systems/BehaviorTreeSystem.hpp>
#include <collisions/ContactListener.hpp>
#include <systems/PlayerStatsSyncSystem.hpp>
#include <systems/DeathCheckSystem.hpp>
#include <components/TargetedComponent.hpp>
#include <systems/TargetStatsSyncSystem.hpp>
#include <systems/BulletSystem.hpp>
#include <factories/BulletEntityFactory.hpp>
#include <factories/EnemyEntityFactory.hpp>
#include <tiled/TiledMap.hpp>
#include <tiled/TiledTileLayerDrawable.hpp>
#include <chrono>
#include <systems/NodeWatchSystem.hpp>
#include <systems/PathfindingDebugDrawSystem.hpp>
#include <js/ShopJSHandler.hpp>
#include <components/ActiveComponent.hpp>
#include <components/PhysicsComponent.hpp>
#include <components/BulletComponent.hpp>
#include <factories/ItemEntityFactory.hpp>
#include <ItemBehavior.hpp>

int GetKeyboardModifiers()
{
  int mod = 0;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
  {
    mod |= WebKeyboardEvent::kModControlKey;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
  {
    mod |= WebKeyboardEvent::kModShiftKey;
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
  {
    mod |= WebKeyboardEvent::kModAltKey;
  }

  return mod;
}

int getWebKeyFromSFMLKey(sf::Keyboard::Key key)
{
  switch (key)
  {
    case sf::Keyboard::LControl: return KeyCodes::AK_CONTROL;
    case sf::Keyboard::RControl: return KeyCodes::AK_CONTROL;
    case sf::Keyboard::LShift: return KeyCodes::AK_SHIFT;
    case sf::Keyboard::RShift: return KeyCodes::AK_SHIFT;
    case sf::Keyboard::LAlt: return KeyCodes::AK_LMENU;
    case sf::Keyboard::RAlt: return KeyCodes::AK_RMENU;
    case sf::Keyboard::LSystem: return KeyCodes::AK_LWIN;
    case sf::Keyboard::RSystem: return KeyCodes::AK_RWIN;
    case sf::Keyboard::Menu: return KeyCodes::AK_MENU;
    case sf::Keyboard::SemiColon: return KeyCodes::AK_OEM_1;
    case sf::Keyboard::Slash: return KeyCodes::AK_OEM_1;
    case sf::Keyboard::Equal: return KeyCodes::AK_OEM_PLUS;
    case sf::Keyboard::Dash: return KeyCodes::AK_OEM_MINUS;
    case sf::Keyboard::LBracket: return KeyCodes::AK_OEM_4;
    case sf::Keyboard::RBracket: return KeyCodes::AK_OEM_6;
    case sf::Keyboard::Comma: return KeyCodes::AK_OEM_COMMA;
    case sf::Keyboard::Period: return KeyCodes::AK_OEM_PERIOD;
    case sf::Keyboard::Quote: return KeyCodes::AK_OEM_7;
    case sf::Keyboard::BackSlash: return KeyCodes::AK_OEM_5;
    case sf::Keyboard::Tilde: return KeyCodes::AK_OEM_3;
    case sf::Keyboard::Escape: return KeyCodes::AK_ESCAPE;
    case sf::Keyboard::Space: return KeyCodes::AK_SPACE;
    case sf::Keyboard::Return: return KeyCodes::AK_RETURN;
    case sf::Keyboard::BackSpace: return KeyCodes::AK_BACK;
    case sf::Keyboard::Tab: return KeyCodes::AK_TAB;
    case sf::Keyboard::PageUp: return KeyCodes::AK_PRIOR;
    case sf::Keyboard::PageDown: return KeyCodes::AK_NEXT;
    case sf::Keyboard::End: return KeyCodes::AK_END;
    case sf::Keyboard::Home: return KeyCodes::AK_HOME;
    case sf::Keyboard::Insert: return KeyCodes::AK_INSERT;
    case sf::Keyboard::Delete: return KeyCodes::AK_DELETE;
    case sf::Keyboard::Add: return KeyCodes::AK_ADD;
    case sf::Keyboard::Subtract: return KeyCodes::AK_SUBTRACT;
    case sf::Keyboard::Multiply: return KeyCodes::AK_MULTIPLY;
    case sf::Keyboard::Divide: return KeyCodes::AK_DIVIDE;
    case sf::Keyboard::Pause: return KeyCodes::AK_PAUSE;
    case sf::Keyboard::F1: return KeyCodes::AK_F1;
    case sf::Keyboard::F2: return KeyCodes::AK_F2;
    case sf::Keyboard::F3: return KeyCodes::AK_F3;
    case sf::Keyboard::F4: return KeyCodes::AK_F4;
    case sf::Keyboard::F5: return KeyCodes::AK_F5;
    case sf::Keyboard::F6: return KeyCodes::AK_F6;
    case sf::Keyboard::F7: return KeyCodes::AK_F7;
    case sf::Keyboard::F8: return KeyCodes::AK_F8;
    case sf::Keyboard::F9: return KeyCodes::AK_F9;
    case sf::Keyboard::F10: return KeyCodes::AK_F10;
    case sf::Keyboard::F11: return KeyCodes::AK_F11;
    case sf::Keyboard::F12: return KeyCodes::AK_F12;
    case sf::Keyboard::F13: return KeyCodes::AK_F13;
    case sf::Keyboard::F14: return KeyCodes::AK_F14;
    case sf::Keyboard::F15: return KeyCodes::AK_F15;
    case sf::Keyboard::Left: return KeyCodes::AK_LEFT;
    case sf::Keyboard::Right: return KeyCodes::AK_RIGHT;
    case sf::Keyboard::Up: return KeyCodes::AK_UP;
    case sf::Keyboard::Down: return KeyCodes::AK_DOWN;
    case sf::Keyboard::Numpad0: return KeyCodes::AK_NUMPAD0;
    case sf::Keyboard::Numpad1: return KeyCodes::AK_NUMPAD1;
    case sf::Keyboard::Numpad2: return KeyCodes::AK_NUMPAD2;
    case sf::Keyboard::Numpad3: return KeyCodes::AK_NUMPAD3;
    case sf::Keyboard::Numpad4: return KeyCodes::AK_NUMPAD4;
    case sf::Keyboard::Numpad5: return KeyCodes::AK_NUMPAD5;
    case sf::Keyboard::Numpad6: return KeyCodes::AK_NUMPAD6;
    case sf::Keyboard::Numpad7: return KeyCodes::AK_NUMPAD7;
    case sf::Keyboard::Numpad8: return KeyCodes::AK_NUMPAD8;
    case sf::Keyboard::Numpad9: return KeyCodes::AK_NUMPAD9;
    case sf::Keyboard::A: return KeyCodes::AK_A;
    case sf::Keyboard::B: return KeyCodes::AK_B;
    case sf::Keyboard::C: return KeyCodes::AK_C;
    case sf::Keyboard::D: return KeyCodes::AK_D;
    case sf::Keyboard::E: return KeyCodes::AK_E;
    case sf::Keyboard::F: return KeyCodes::AK_F;
    case sf::Keyboard::G: return KeyCodes::AK_G;
    case sf::Keyboard::H: return KeyCodes::AK_H;
    case sf::Keyboard::I: return KeyCodes::AK_I;
    case sf::Keyboard::J: return KeyCodes::AK_J;
    case sf::Keyboard::K: return KeyCodes::AK_K;
    case sf::Keyboard::L: return KeyCodes::AK_L;
    case sf::Keyboard::M: return KeyCodes::AK_M;
    case sf::Keyboard::N: return KeyCodes::AK_N;
    case sf::Keyboard::O: return KeyCodes::AK_O;
    case sf::Keyboard::P: return KeyCodes::AK_P;
    case sf::Keyboard::Q: return KeyCodes::AK_Q;
    case sf::Keyboard::R: return KeyCodes::AK_R;
    case sf::Keyboard::S: return KeyCodes::AK_S;
    case sf::Keyboard::T: return KeyCodes::AK_T;
    case sf::Keyboard::U: return KeyCodes::AK_U;
    case sf::Keyboard::V: return KeyCodes::AK_V;
    case sf::Keyboard::W: return KeyCodes::AK_W;
    case sf::Keyboard::X: return KeyCodes::AK_X;
    case sf::Keyboard::Y: return KeyCodes::AK_Y;
    case sf::Keyboard::Z: return KeyCodes::AK_Z;
    case sf::Keyboard::Num0: return KeyCodes::AK_0;
    case sf::Keyboard::Num1: return KeyCodes::AK_1;
    case sf::Keyboard::Num2: return KeyCodes::AK_2;
    case sf::Keyboard::Num3: return KeyCodes::AK_3;
    case sf::Keyboard::Num4: return KeyCodes::AK_4;
    case sf::Keyboard::Num5: return KeyCodes::AK_5;
    case sf::Keyboard::Num6: return KeyCodes::AK_6;
    case sf::Keyboard::Num7: return KeyCodes::AK_7;
    case sf::Keyboard::Num8: return KeyCodes::AK_8;
    case sf::Keyboard::Num9: return KeyCodes::AK_9;
    default: return KeyCodes::AK_UNKNOWN;
  }
}

Game::Game()
{
}

bool Game::start()
{
  m_window.create(sf::VideoMode(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT), "SFML + Awesomium");
  m_window.setPosition(sf::Vector2i(m_window.getPosition().x, 0));
  m_window.setFramerateLimit(60);
  
  m_mapTexture.create(Constants::VIEW_WIDTH, Constants::MAP_HEIGHT, false);

  m_mapSprite.setTexture(m_mapTexture.getTexture());
  m_mapSprite.setPosition(0, static_cast<float>(Constants::MAP_OFFSET));

  m_window.setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(Constants::VIEW_WIDTH), static_cast<float>(Constants::VIEW_HEIGHT))));

  loadMedia();

  /* Awesomium Setup Begin */

  m_webCore = WebCore::Initialize(WebConfig());

  m_webSession = m_webCore->CreateWebSession(WSLit(""), WebPreferences());
  DataSource* data_source = new DataPakSource(WSLit((GetApplicationDir() + "/../assets/resources.pak").c_str()));
  m_webSession->AddDataSource(WSLit("app"), data_source);

  m_webView = m_webCore->CreateWebView(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT, m_webSession, kWebViewType_Offscreen);
  m_webView->SetTransparent(true);

  WebURL url(WSLit("asset://app/InGameHud.html"));
  m_webView->LoadURL(url);

  while (m_webView->IsLoading())
  {
    m_webCore->Update();
  }

  Sleep(300);
  m_webCore->Update();

  // ensure nothing on the page can be selected
  m_webView->ExecuteJavascript(WSLit("document.body.onselectstart = function() { return false; }"), WSLit(""));
  m_jsApp = m_webView->CreateGlobalJavascriptObject(WSLit("app"));

  m_uiSurface = static_cast<BitmapSurface*>(m_webView->surface());

  m_uiTexture = new sf::Texture();
  m_uiTexture->create(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT);
  m_uiSprite.setTexture(*m_uiTexture);

  m_uiRGBABuffer = new unsigned char[Constants::VIEW_WIDTH * Constants::VIEW_HEIGHT * 4];

  m_uiSurface->CopyTo(m_uiRGBABuffer, Constants::VIEW_WIDTH * 4, 4, true, false);
  m_uiTexture->update(m_uiRGBABuffer, Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT, 0, 0);

  /*m_jsHandler = new ShopJSHandler(m_jsApp);
  m_webView->set_js_method_handler(m_jsHandler);*/

  /* Awesoimium Setup End */

  // Engine parameters: entityPoolInitialSize, entityPoolMaxSize, componentPoolInitialSize
  m_engine = std::make_unique<ECS::Engine>(25, 250, 150);

  /* Systems Setup Begin */

  auto behaviorTreeSystem = new BehaviorTreeSystem();
  m_engine->addSystem(behaviorTreeSystem);

  auto playerInputSystem = new PlayerInputSystem(m_window);
  m_engine->addSystem(playerInputSystem);

  auto bulletSystem = new BulletSystem();
  m_engine->addSystem(bulletSystem);

  auto animationSystem = new AnimationSystem();
  m_engine->addSystem(animationSystem);

  auto physicsSystem = new PhysicsSystem(m_world);
  m_engine->addSystem(physicsSystem);

  auto nodeWatchSystem = new NodeWatchSystem();
  m_engine->addSystem(nodeWatchSystem);

  auto lifetimeSystem = new LifetimeSystem(m_engine, m_world);
  m_engine->addSystem(lifetimeSystem);

  auto playerStatsSyncSystem = new PlayerStatsSyncSystem(m_uiValues);
  m_engine->addSystem(playerStatsSyncSystem);

  auto targetStatsSyncSystem = new TargetStatsSyncSystem(m_uiValues);
  m_engine->addSystem(targetStatsSyncSystem);

  auto deathCheckSystem = new DeathCheckSystem(m_engine, m_world);
  m_engine->addSystem(deathCheckSystem);

  auto uiUpdateSystem = new UIUpdateSystem();
  m_engine->addSystem(uiUpdateSystem);

  auto renderSystem = new RenderSystem(m_mapTexture);
  m_engine->addSystem(renderSystem);

#ifdef _DEBUG
  auto physicsDebugDrawSystem = new PhysicsDebugDrawSystem(m_mapTexture);
  physicsDebugDrawSystem->setProcessing(false);
  m_engine->addSystem(physicsDebugDrawSystem);

  auto pathfindingDebugDrawSystem = new PathfindingDebugDrawSystem(m_mapTexture);
  pathfindingDebugDrawSystem->setProcessing(false);
  m_engine->addSystem(pathfindingDebugDrawSystem);
#endif

  /* Systems Setup End */

  /* Box2D Setup Begin */

  m_world = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));

  m_world->SetContactListener(new ContactListener());

  /* Box2D Setup End */

  /* Entity Setup Begin */

  auto uiContainer = BasicEntityFactory::makeUIContainer(m_webView, m_uiValues);
  m_engine->addEntity(uiContainer);

  m_player = BasicEntityFactory::makePlayer(m_resources, sf::Vector2f(300, 200));
  m_engine->addEntity(m_player);

  /* Entity Setup End */

  return true;
}

void Game::mainLoop()
{
#ifdef _DEBUG
  sf::Font font;
  font.loadFromFile("../assets/Adventure Subtitles.ttf");

  sf::Text fpsText("FPS: 60", font);
  fpsText.setCharacterSize(16);
  fpsText.setStyle(sf::Text::Bold);
  fpsText.setColor(sf::Color::Red);
  fpsText.setPosition(5, Constants::VIEW_HEIGHT - 21);
#endif

  srand(static_cast<unsigned int>(time(nullptr)));

  sf::Clock deltaClock;

  /* Maps Begin */

  auto offscreenMap = TiledMap::loadFromJson("../assets/levels/NW_02.json");
  auto offscreenGameMap = new GameMap(offscreenMap);
  addMap(offscreenGameMap);

  auto map = TiledMap::loadFromJson("../assets/levels/S_01.json"); // NW_02
  auto gameMap = new GameMap(map);
  addMap(gameMap, true);
  
  auto& southExit = map->getExit(South);
  southExit.setDesintation(offscreenGameMap);

  auto& northExit = offscreenMap->getExit(North);
  northExit.setDesintation(gameMap);

  /* Maps End */

  auto enemy = EnemyEntityFactory::makeBasicEnemy(m_resources, map, sf::Vector2f(600, 200));
  m_engine->addEntity(enemy);

  m_currentMap->tiledMap->addEntity(enemy);
  
  m_window.requestFocus();
  while (m_window.isOpen())
  {
    sf::Event event;
    while (m_window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        m_window.close();
      }

      handleBrowserEvents(event);

      if (m_jsHandler == nullptr || !m_jsHandler->doesBlockInput())
      {
        if (event.type == sf::Event::MouseButtonPressed)
        {
          if (event.mouseButton.button == sf::Mouse::Button::Left)
          {
            if (m_player->getId() != 0)
            {
              static const auto BULLET_SPEED = 0.07f;

              auto mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
              auto playerTransform = dynamic_cast<sf::Transformable*>(m_player->get<RenderComponent>()->drawable);
              
              auto dx = mousePos.x - playerTransform->getPosition().x;
              auto dy = mousePos.y - playerTransform->getPosition().y - Constants::MAP_OFFSET;

              auto angle = atan2(dy, dx);

              auto bullet = BulletEntityFactory::makeBasicBullet(
                m_resources,
                // Options
                {
                  EntityInfo::Player,
                  sf::Vector2f(
                    playerTransform->getPosition().x,
                    playerTransform->getPosition().y
                  ),
                  sf::Vector2f(
                    BULLET_SPEED * cos(angle),
                    BULLET_SPEED * sin(angle)
                  )
                }
              );
              m_engine->addEntity(bullet);
            }
          }
#ifdef _DEBUG
          else if (event.mouseButton.button == sf::Mouse::Right)
          {
            auto mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
            mousePos.y -= Constants::MAP_OFFSET;

            auto mx = static_cast<int>(mousePos.x / Constants::COLLISION_TILE_WIDTH);
            auto my = static_cast<int>(mousePos.y / Constants::COLLISION_TILE_HEIGHT);

            if (mx >= 0 && my >= 0)
            {
              auto tile = map->getCollisionTile(mx, my);

              printf("x, y: (%d, %d)\n", mx, my);
              printf("horizontal clearance: (%d, %d)\n", tile.horizontalClearance.x, tile.horizontalClearance.y);
              printf("vertical clearance: (%d, %d)\n\n", tile.verticalClearance.x, tile.verticalClearance.y);
            }
          }
#endif
        }
        else if (event.type == sf::Event::KeyPressed)
        {
#ifdef _DEBUG
          if (event.key.code == sf::Keyboard::Numpad0)
          {
            auto physicsDebugDrawSystem = m_engine->getSystem<PhysicsDebugDrawSystem>();
            physicsDebugDrawSystem->setProcessing(!physicsDebugDrawSystem->checkProcessing());

            auto pathfindingDebugDrawSystem = m_engine->getSystem<PathfindingDebugDrawSystem>();
            pathfindingDebugDrawSystem->setProcessing(!pathfindingDebugDrawSystem->checkProcessing());
          }
          else if (event.key.code == sf::Keyboard::Numpad5)
          {
            if (map->isCollisionAdded())
            {
              map->removeCollision(m_world, m_engine);
            }
            else
            {
              map->addCollision(m_world, m_engine);
            }
          }
          else if (event.key.code == sf::Keyboard::I)
          {
            auto mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
            mousePos.y -= Constants::MAP_OFFSET;

            auto sprite = new sf::Sprite(m_resources.getTexture("items"));

            auto rectX = rand() % 224;
            rectX = static_cast<int>(rectX / 32.f) * 32;

            auto rectY = rand() % 544;
            rectY = static_cast<int>(rectY / 32.f) * 32;

            sprite->setTextureRect(sf::IntRect(rectX, rectY, 32, 32));
            sprite->setOrigin(sprite->getTextureRect().width / 2.0f, sprite->getTextureRect().height / 2.0f);
            sprite->setPosition(mousePos);
            
            auto item = ItemEntityFactory::makeItem({ ("Test " + std::to_string(rand() % 255)), ItemBehavior::testItem, *sprite, mousePos });
            m_engine->addEntity(item);

            m_currentMap->tiledMap->addEntity(item);
          }
          else if (event.key.code == sf::Keyboard::Numpad8)
          {
            m_window.setSize(sf::Vector2u(Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT));
          }
          else if (event.key.code == sf::Keyboard::Numpad9)
          {
            m_window.setSize(sf::Vector2u(Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT));
          }
#endif
        }
      }
    }
    
    m_webCore->Update();

    if (m_uiSurface->is_dirty())
    {
      m_uiSurface->CopyTo(m_uiRGBABuffer, m_uiSurface->row_span(), 4, true, false);
      m_uiTexture->update(m_uiRGBABuffer, Constants::VIEW_WIDTH, Constants::VIEW_HEIGHT, 0, 0);
    }

    auto dt = deltaClock.restart();
    auto dtMillis = dt.asMilliseconds() / 1000.f;

#ifdef _DEBUG
    fpsText.setString("FPS: " + std::to_string(1 / dt.asSeconds()));
#endif

    m_window.clear(sf::Color::Black);
    m_mapTexture.clear(sf::Color::Black);

    m_engine->update(dtMillis);

    // has to be done here as Box2D bodies cannot be changed during a simulation,
    // which would occur if this were called in changeMap as that function is called
    // in the contact listener upon touching an exit (during the Box2D simulation)
    if (m_mapChanged)
    {
      m_previousMap->tiledMap->disableCollision();
      m_currentMap->tiledMap->enableCollision();

      m_previousMap->tiledMap->disableEntities();
      m_currentMap->tiledMap->enableEntities();

      auto cPhysics = m_player->get<PhysicsComponent>();
      cPhysics->body->SetTransform(
        b2Vec2(
          m_newPlayerPosition.x / Constants::PIXELS_PER_METER,
          m_newPlayerPosition.y / Constants::PIXELS_PER_METER),
        cPhysics->body->GetAngle());

      m_mapChanged = false;
    }

#ifdef _DEBUG
    //map->drawCollisionMap(m_window);
#endif

    m_mapTexture.display();
    m_window.draw(m_mapSprite);

    m_window.draw(fpsText);

    m_window.draw(m_uiSprite);
    
    m_window.display();
  }
}

void Game::quit()
{
  m_engine->removeAllEntities();

  m_webView->Destroy();
  m_webSession->Release();
  m_webCore->Shutdown();

  m_webCore = nullptr;
  m_webSession = nullptr;
  m_webView = nullptr;
  m_uiSurface = nullptr;
  m_uiRGBABuffer = nullptr;
  m_uiTexture = nullptr;

  m_player = nullptr;
}

void Game::loadMedia()
{
  m_resources.loadTexture("duck", "../assets/duck.png");
  m_resources.loadTexture("iffrit", "../assets/iffrit.png");

  m_resources.loadTexture("deer", "../assets/deer.png");
  m_resources.loadTexture("pinkBullet", "../assets/pinkBullet.png");

  m_resources.loadTexture("items", "../assets/items.png");

  m_resources.loadTexture("terrain_atlas", "F:/Images/Game Graphics/Tilesets/Atlas_0/terrain_atlas.png");
  m_resources.loadTexture("build_atlas", "F:/Images/Game Graphics/Tilesets/Atlas2/build_atlas.png");
}

void Game::handleBrowserEvents(sf::Event& p_event) const
{
  if (p_event.type == sf::Event::MouseButtonPressed)
  {
    auto button = p_event.mouseButton.button;
    m_webView->InjectMouseDown(button == sf::Mouse::Left ? kMouseButton_Left : button == sf::Mouse::Right ? kMouseButton_Right : kMouseButton_Middle);
  }
  else if (p_event.type == sf::Event::MouseButtonReleased)
  {
    auto button = p_event.mouseButton.button;
    m_webView->InjectMouseUp(button == sf::Mouse::Left ? kMouseButton_Left : button == sf::Mouse::Right ? kMouseButton_Right : kMouseButton_Middle);
  }
  else if (p_event.type == sf::Event::MouseMoved)
  {
    auto mousePosition = sf::Mouse::getPosition(m_window);
    auto point = m_window.mapPixelToCoords(mousePosition);

    m_webView->InjectMouseMove(static_cast<int>(point.x), static_cast<int>(point.y));
  }
  else if (p_event.type == sf::Event::MouseWheelScrolled)
  {
    m_webView->InjectMouseWheel(static_cast<int>(p_event.mouseWheelScroll.delta * 10), 0);
  }
  else if (p_event.type == sf::Event::KeyPressed)
  {
    WebKeyboardEvent keyEvent;

    keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyDown;

    char* buf = new char[20];
    keyEvent.virtual_key_code = getWebKeyFromSFMLKey(p_event.key.code);
    Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
    strcpy_s(keyEvent.key_identifier, buf);
    delete[] buf;

    keyEvent.modifiers = GetKeyboardModifiers();
    keyEvent.native_key_code = p_event.key.system;

    unsigned int chr;
    if ((p_event.text.unicode & 0xFF80) == 0)
    {
      chr = p_event.text.unicode & 0x7F;
    }
    else
    {
      chr = p_event.text.unicode;
    }

    keyEvent.text[0] = chr;
    keyEvent.unmodified_text[0] = chr;

    m_webView->InjectKeyboardEvent(keyEvent);
  }
  else if (p_event.type == sf::Event::KeyReleased)
  {
    WebKeyboardEvent keyEvent;

    keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;

    char* buf = new char[20];
    keyEvent.virtual_key_code = getWebKeyFromSFMLKey(p_event.key.code);
    Awesomium::GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
    strcpy_s(keyEvent.key_identifier, buf);
    delete[] buf;

    keyEvent.modifiers = GetKeyboardModifiers();
    keyEvent.native_key_code = p_event.key.system;

    m_webView->InjectKeyboardEvent(keyEvent);
  }
  else if (p_event.type == sf::Event::TextEntered)
  {
    WebKeyboardEvent keyEvent;

    keyEvent.type = Awesomium::WebKeyboardEvent::kTypeKeyUp;

    char* buf = new char[20];
    keyEvent.virtual_key_code = getWebKeyFromSFMLKey(p_event.key.code);
    GetKeyIdentifierFromVirtualKeyCode(keyEvent.virtual_key_code, &buf);
    strcpy_s(keyEvent.key_identifier, buf);
    delete[] buf;

    keyEvent.modifiers = GetKeyboardModifiers();
    keyEvent.native_key_code = p_event.key.system;

    unsigned int chr;
    if ((p_event.text.unicode & 0xFF80) == 0)
    {
      chr = p_event.text.unicode & 0x7F;
    }
    else
    {
      chr = p_event.text.unicode;
    }

    keyEvent.text[0] = chr;
    keyEvent.unmodified_text[0] = chr;

    if (chr)
    {
      keyEvent.type = WebKeyboardEvent::kTypeChar;
      keyEvent.virtual_key_code = chr;
      keyEvent.native_key_code = chr;
      m_webView->InjectKeyboardEvent(keyEvent);
    }
  }
}

const sf::RenderWindow& Game::getWindow() const
{
  return m_window;
}

ECS::Entity* Game::getPlayer() const
{
  return m_player;
}

ECS::Engine& Game::getEngine() const
{
  return *m_engine;
}

b2World& Game::getWorld() const
{
  return *m_world;
}

ECS::Entity* Game::getTarget() const
{
  return m_target;
}

void Game::setTarget(ECS::Entity* p_target)
{
  if (m_target == p_target)
  {
    return;
  }

  if (m_target == nullptr)
  {
    m_uiValues.enemy.display = true;
    m_uiValues.enemy.displayChanged = true;
  }
  else
  {
    m_target->remove<TargetedComponent>();
  }

  m_target = p_target;

  auto cTarget = m_engine->createComponent<TargetedComponent>();
  m_target->add(cTarget);
}

void Game::clearTarget()
{
  if (m_target == nullptr)
  {
    return;
  }

  m_target->remove<TargetedComponent>();
  m_target = nullptr;

  m_uiValues.enemy.display = false;
  m_uiValues.enemy.displayChanged = true;
}

void Game::addMap(GameMap* p_map, bool p_active)
{
  m_maps.push_back(p_map);

  auto tiledMap = p_map->tiledMap;

  for (unsigned int i = 0; i < tiledMap->getTileLayerCount(); ++i)
  {
    auto mapLayer = tiledMap->getTileLayer(i);
    auto tileset = tiledMap->getTileset(mapLayer.getTilesetIndex());

    auto tiledLayer = new TiledTileLayerDrawable(m_resources.getTexture(tileset.getName()), mapLayer, tileset);
    auto layerEntity = BasicEntityFactory::makeDrawable(*tiledLayer, { p_active, mapLayer.getDepth() });

    m_engine->addEntity(layerEntity);

    p_map->mapLayerDrawables.push_back(tiledLayer);
    p_map->mapLayerEntities.push_back(layerEntity);
  }

  tiledMap->addCollision(m_world, m_engine, true, p_active);

  if (p_active)
  {
    m_currentMap = p_map;
  }
}

void Game::changeMap(GameMap* p_gameMap, const sf::Vector2f& p_playerPosition)
{
  // remove all bullets, as they are neither carried over to the new map nor maintained on the old map
  auto allBullets = m_engine->getEntitiesFor(ECS::Family::all<BulletComponent>().get());
  for (auto it = allBullets->begin(); it != allBullets->end(); ++it)
  {
    auto bullet = *it;
    m_engine->removeEntity(bullet);
  }

  if (m_currentMap != nullptr)
  {
    for (auto it = m_currentMap->mapLayerEntities.begin(); it != m_currentMap->mapLayerEntities.end(); ++it)
    {
      auto layerEntity = *it;

      auto cActive = layerEntity->get<ActiveComponent>();
      cActive->isActive = false;
    }

    m_previousMap = m_currentMap;
  }

  m_currentMap = p_gameMap;

  for (auto it = m_currentMap->mapLayerEntities.begin(); it != m_currentMap->mapLayerEntities.end(); ++it)
  {
    auto layerEntity = *it;

    auto cActive = layerEntity->get<ActiveComponent>();
    cActive->isActive = true;
  }

  clearTarget();

  m_newPlayerPosition = p_playerPosition;
  m_mapChanged = true;
}

Game& Game::Get()
{
  static Game s_singleton;

  return s_singleton;
}

std::string Game::GetApplicationDir()
{
  auto hModule = GetModuleHandleW(nullptr);
  WCHAR wpath[MAX_PATH];

  GetModuleFileNameW(hModule, wpath, MAX_PATH);
  std::wstring wide(wpath);

  typedef std::codecvt_utf8<wchar_t> convert_type;
  std::wstring_convert<convert_type, wchar_t> converter;

  std::string path = converter.to_bytes(wide);
  std::replace(path.begin(), path.end(), '\\', '/');
  path = path.substr(0, path.find_last_of("\\/"));

  return path;
}

