#include "RenderHandler.hpp"

RenderHandler::RenderHandler(SDL_Texture* p_texture) : m_renderTexture(p_texture)
{
}

void RenderHandler::update()
{
  CefDoMessageLoopWork();
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> p_browser, CefRect& p_rect)
{
  int w, h;
  SDL_QueryTexture(m_renderTexture, nullptr, nullptr, &w, &h);
  p_rect = CefRect(0, 0, w, h);

  return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> p_browser, PaintElementType p_type, const RectList& p_dirtyRects, const void* p_buffer, int p_width, int p_height)
{
  int w, h;
  SDL_QueryTexture(m_renderTexture, nullptr, nullptr, &w, &h);

  int pitch;
  void* pixels = nullptr;

  SDL_LockTexture(m_renderTexture, nullptr, &pixels, &pitch);

  memcpy(pixels, p_buffer, pitch * h);

  SDL_UnlockTexture(m_renderTexture);
}

