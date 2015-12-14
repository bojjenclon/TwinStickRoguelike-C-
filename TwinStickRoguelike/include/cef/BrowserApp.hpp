#ifndef BROWSER_APP_HPP
#define BROWSER_APP_HPP

#include <include/cef_app.h>

class BrowserApp : public CefApp, public CefRenderProcessHandler {
public:
  BrowserApp();

  CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

  // CefRenderProcessHandler interface
public:
  void OnContextCreated(CefRefPtr<CefBrowser> p_browser, CefRefPtr<CefFrame> p_frame, CefRefPtr<CefV8Context> p_context) override;
  void OnContextReleased(CefRefPtr<CefBrowser> p_browser, CefRefPtr<CefFrame> p_frame, CefRefPtr<CefV8Context> p_context) override;

private:
  CefRefPtr<CefV8Handler> m_handler;

  IMPLEMENT_REFCOUNTING(BrowserApp);
};

#endif
