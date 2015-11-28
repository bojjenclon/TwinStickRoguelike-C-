#include "BrowserApp.hpp"
#include "UIV8Handler.hpp"
#include <iostream>

BrowserApp::BrowserApp()
{
}

CefRefPtr<CefRenderProcessHandler> BrowserApp::GetRenderProcessHandler()
{
  return this;
}

// Setup native functions that can be called from the embedded webpage (the UI)
void BrowserApp::OnContextCreated(CefRefPtr<CefBrowser> p_browser, CefRefPtr<CefFrame> p_frame, CefRefPtr<CefV8Context> p_context)
{
  auto window = p_context->GetGlobal();
  m_handler = new UIV8Handler(p_browser);
  
  auto funcGetHP = CefV8Value::CreateFunction("getHP", m_handler);
  auto funcSetHP = CefV8Value::CreateFunction("setHP", m_handler);
  
  auto cpp = CefV8Value::CreateObject(nullptr);

  window->SetValue("cpp", cpp, V8_PROPERTY_ATTRIBUTE_NONE);

  cpp->SetValue("getHP", funcGetHP, V8_PROPERTY_ATTRIBUTE_NONE);
  cpp->SetValue("setHP", funcSetHP, V8_PROPERTY_ATTRIBUTE_NONE);
}

void BrowserApp::OnContextReleased(CefRefPtr<CefBrowser> p_browser, CefRefPtr<CefFrame> p_frame, CefRefPtr<CefV8Context> p_context)
{
  static_cast<UIV8Handler*>(m_handler.get())->releaseBrowser();
  m_handler = nullptr;
}
