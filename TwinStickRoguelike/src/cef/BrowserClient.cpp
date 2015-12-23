#include <cef/BrowserClient.hpp>

BrowserClient::BrowserClient(CefRefPtr<RenderHandler> renderHandler, UIValues& p_uiValues) : m_renderHandler(renderHandler), m_browser(nullptr), m_browserHwnd(nullptr), m_uiValues(p_uiValues)
{
}

BrowserClient::~BrowserClient()
{
  m_renderHandler = nullptr;
  m_browser = nullptr;
  m_browserHwnd = nullptr;
}

CefRefPtr<CefLifeSpanHandler> BrowserClient::GetLifeSpanHandler()
{
  return this;
}

CefRefPtr<CefRenderHandler> BrowserClient::GetRenderHandler()
{
  return m_renderHandler;
}

// Receive messages sent from the render thread, which will typically be commands sent from the UI
bool BrowserClient::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  auto messageType = message->GetName();

  if (messageType == "healthChanged")
  {
    auto currentHealth = message->GetArgumentList()->GetInt(0);

    m_uiValues.player.currentHealth = currentHealth;
    m_uiValues.player.healthChanged = true;
  }

  return false;
}

bool BrowserClient::DoClose(CefRefPtr<CefBrowser> browser)
{
  browser->GetHost()->CloseBrowser(false);

  return false;
}

bool BrowserClient::OnBeforePopup(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString& target_url, const CefString& target_frame_name, CefLifeSpanHandler::WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures& popupFeatures, CefWindowInfo& windowInfo, CefRefPtr<CefClient>& client, CefBrowserSettings& settings, bool* no_javascript_access)
{
  return false;
}

void BrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  if (!m_browser.get())
  {
    m_browser = browser;
    m_browserHwnd = browser->GetHost()->GetWindowHandle();
  }
}

void BrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  if (m_browserHwnd == browser->GetHost()->GetWindowHandle())
  {
    m_browser = nullptr;
  }
}


