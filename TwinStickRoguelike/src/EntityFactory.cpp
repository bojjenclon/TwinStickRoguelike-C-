#include "EntityFactory.hpp"

#include "components/RenderComponent.hpp"
#include "components/HealthComponent.hpp"
#include "components/UIComponent.hpp"

ECS::Entity* EntityFactory::makeDrawable(ECS::Engine* p_engine, sf::Drawable& p_drawable, int p_depth = 1)
{
  auto entity = p_engine->createEntity();

  auto cRender = p_engine->createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->drawable = &p_drawable;
  cRender->depth = p_depth;

  return entity;
}

ECS::Entity* EntityFactory::makeUIContainer(ECS::Engine* p_engine, sf::Sprite& p_sprite, CefRefPtr<CefBrowser> p_browser, UIValues& p_uiValues)
{
  auto entity = p_engine->createEntity();

  auto cRender = p_engine->createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->drawable = &p_sprite;
  cRender->depth = -999;

  auto cUI = p_engine->createComponent<UIComponent>();
  entity->add(cUI);
  cUI->uiBrowser = p_browser;
  cUI->uiValues = &p_uiValues;

  return entity;
}

ECS::Entity* EntityFactory::makePlayer(ECS::Engine* p_engine, ResourceManager* p_resources)
{
  auto entity = p_engine->createEntity();

  auto cRender = p_engine->createComponent<RenderComponent>();
  entity->add(cRender);
  if (!p_resources->isTextureLoaded("test.png"))
  {
    p_resources->loadTexture("test.png");
  }
  cRender->drawable = new sf::Sprite(*p_resources->getTexture("test.png"));

  auto cHealth = p_engine->createComponent<HealthComponent>();
  entity->add(cHealth);
  cHealth->currentHealth = cHealth->maxHealth = 10;

  return entity;
}

