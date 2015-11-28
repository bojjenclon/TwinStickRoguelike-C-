#ifndef ENTITY_FACTORY_HPP
#define ENTITY_FACTORY_HPP

#include <ecstasy/core/Engine.h>
#include <include/cef_browser.h>

#include "ResourceManager.hpp"
#include "UIValues.hpp"

namespace EntityFactory
{
  ECS::Entity* makeUIContainer(ECS::Engine* p_engine, SDL_Texture* p_texture, CefRefPtr<CefBrowser> p_browser, UIValues* p_uiValues);
  ECS::Entity* makePlayer(ECS::Engine* p_engine, ResourceManager* p_resources);
};

#endif