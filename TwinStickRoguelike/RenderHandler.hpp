#ifndef RENDER_HANDLER_HPP
#define RENDER_HANDLER_HPP

#include <SDL.h>

#include <include/cef_app.h>
#include <include/cef_render_handler.h>

class RenderHandler : public CefRenderHandler
{
public:
  SDL_Texture* m_renderTexture;

  RenderHandler(SDL_Texture* p_texture);

  // SDL Update
public:
  void update();

  // CefRenderHandler interface
public:
  bool GetViewRect(CefRefPtr<CefBrowser> p_browser, CefRect& p_rect) override;
  void OnPaint(CefRefPtr<CefBrowser> p_browser, PaintElementType p_type, const RectList& p_dirtyRects, const void* p_buffer, int p_width, int p_height) override;

  // CefBase interface
public:
  IMPLEMENT_REFCOUNTING(RenderHandler);
};

#endif
