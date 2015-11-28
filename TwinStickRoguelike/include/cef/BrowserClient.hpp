#ifndef BROWSER_CLIENT_HPP
#define BROWSER_CLIENT_HPP

#include <include/cef_client.h>

#include "RenderHandler.hpp"
#include "UIValues.hpp"

class BrowserClient : public CefClient, public CefLifeSpanHandler, public CefRenderProcessHandler
{
public:
  BrowserClient(CefRefPtr<RenderHandler> renderHandler, UIValues* p_uiValues);
  ~BrowserClient();

  // CefClient interface
public:
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override;
  bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;

  // CefLifeSpanHandler interface
public:
  virtual bool DoClose(CefRefPtr<CefBrowser> browser) override;
  virtual bool OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access) override;
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

private:
  CefRefPtr<CefRenderHandler> m_renderHandler;
  CefRefPtr<CefBrowser> m_browser;
  CefWindowHandle m_browserHwnd;

  UIValues* m_uiValues = nullptr;

  IMPLEMENT_REFCOUNTING(BrowserClient);
};

#endif

