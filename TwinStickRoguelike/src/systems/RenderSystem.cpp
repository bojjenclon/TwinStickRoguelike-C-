#include "systems/RenderSystem.hpp"

#include <ecstasy/core/Family.h>
#include <SDL.h>

RenderSystem::RenderSystem(Game* p_game) : SortedIteratingSystem(ECS::Family::all<RenderComponent>().get(), DepthTest())
{
  m_game = p_game;
}

void RenderSystem::update(float deltaTime)
{
  auto renderer = m_game->getRenderer();

  SDL_RenderClear(renderer);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  SortedIteratingSystem::update(deltaTime);

  SDL_RenderPresent(renderer);
}

void RenderSystem::processEntity(ECS::Entity* entity, float deltaTime)
{
  auto renderer = m_game->getRenderer();
  auto scaleX = m_game->getScaleX(), scaleY = m_game->getScaleY();

  auto cRender = entity->get<RenderComponent>();

  int w, h;
  SDL_QueryTexture(cRender->texture, nullptr, nullptr, &w, &h);
  SDL_Rect renderQuad = { 0, 0, static_cast<int>(w * scaleX), static_cast<int>(h * scaleY) };

  SDL_RenderCopy(renderer, cRender->texture, nullptr, &renderQuad);
}
