#include "EntityFactory.hpp"

#include "RenderComponent.hpp"
#include "HealthComponent.hpp"
#include "UIComponent.hpp"

ECS::Entity* EntityFactory::makeUIContainer(ECS::Engine* p_engine, SDL_Texture* p_texture, CefRefPtr<CefBrowser> p_browser, UIValues* p_uiValues)
{
  auto entity = p_engine->createEntity();

  auto cRender = p_engine->createComponent<RenderComponent>();
  entity->add(cRender);
  cRender->texture = p_texture;
  cRender->depth = -999;

  auto cUI = p_engine->createComponent<UIComponent>();
  entity->add(cUI);
  cUI->uiBrowser = p_browser;
  cUI->uiValues = p_uiValues;

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
  cRender->texture = p_resources->getTexture("test.png");

  auto cHealth = p_engine->createComponent<HealthComponent>();
  entity->add(cHealth);
  cHealth->currentHealth = cHealth->maxHealth = 10;

  return entity;
}

