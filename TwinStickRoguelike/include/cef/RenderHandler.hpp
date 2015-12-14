#ifndef RENDER_HANDLER_HPP
#define RENDER_HANDLER_HPP

#include <SFML/Graphics.hpp>

#include <include/cef_app.h>
#include <include/cef_render_handler.h>

#ifndef BYTES_PER_PIXEL
#define BYTES_PER_PIXEL 4
#endif

class RenderHandler : public CefRenderHandler
{
public:
  sf::Texture* m_renderTexture;

  RenderHandler(sf::Texture* p_texture);

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