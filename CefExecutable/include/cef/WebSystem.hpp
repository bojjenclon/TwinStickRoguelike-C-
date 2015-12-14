#ifndef WEB_SYSTEM_HPP
#define WEB_SYSTEM_HPP

#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/base/cef_lock.h>
#include <SFML/Graphics.hpp>
#include <queue>

#ifndef PLATFORM
#define PLATFORM WINDOWS
#endif

#ifndef BYTES_PER_PIXEL
#define BYTES_PER_PIXEL 4
#endif

struct JsBinding
{
public:
  typedef bool(*JsCallback) (CefRefPtr<CefListValue> arguments);

  JsBinding(std::string name, JsCallback callback) : mFunctionName(name), mfpJSCallback(callback)
  {
  }

  std::string mFunctionName;
  JsCallback mfpJSCallback;
};

class WebApp;
class WebInterface;

class WebSystem : public CefBrowserProcessHandler,
  public CefRenderProcessHandler,
  public CefClient,
  public CefLifeSpanHandler,
  public CefLoadHandler,
  public CefRequestHandler,
  public CefDisplayHandler,
  public CefRenderHandler,
  public CefKeyboardHandler
{
  friend class WebInterface;
  friend class WebV8Handler;

public:
  
  static int Main();

  static void SetSubprocess(const std::string path);
  static void SetSingleProcess(bool single);

  static void StartWeb();
  static void RunWeb();
  static void EndWeb();

  static WebInterface* CreateWebInterfaceSync(int width, int height, const std::string& url, bool transparent);

  static void UpdateInterfaceTextures();

  static CefRefPtr<WebSystem> GetInstance();

  // Cef Handler methods
  virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; }
  virtual CefRefPtr<CefLoadHandler> GetLoadHandler() override { return this; }
  virtual CefRefPtr<CefRequestHandler> GetRequestHandler() override { return this; }
  virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  virtual CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
  virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() override { return this; }

  // CefLoadHandler methods
  virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) override;
  virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) override;

  // CefRenderHandler methods
  virtual bool GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect) override;
  virtual void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;

  // CefKeyboardHandler methods
  virtual bool OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& e, CefEventHandle os_event) override;

  // CefLifespanHandler methods
  virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefBrowserProcessHandler methods
  virtual void OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info) override;

  // CefRenderProcessHandler methods
  virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) override;
  virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) override;
  virtual void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override;
  virtual void OnContextReleased(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, CefRefPtr< CefV8Context > context) override;
  virtual void OnUncaughtException(CefRefPtr< CefBrowser > browser, CefRefPtr< CefFrame > frame, CefRefPtr< CefV8Context > context, CefRefPtr< CefV8Exception > exception, CefRefPtr< CefV8StackTrace > stackTrace) override;

  // CefClient / CefRenderProcessHandler shared method
  virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override;

private:
  static std::string sSubprocess;
  static bool sSingleProcess;
  static CefMainArgs sArgs;

  static CefRefPtr<WebApp> sApp;
  static CefRefPtr<WebSystem> sInstance;

  // Thread

  //base::Lock mLock;

  static std::queue<WebInterface*> sMakeWebInterfaceQueue;
  static std::map<int, WebInterface*> sWebInterfaces;

  typedef std::map<std::pair<std::string, int>, JsBinding::JsCallback> BindingMap;

  static BindingMap sBindings;

  void AddBrowserToInterface(WebInterface* pWeb);

  WebSystem() {}
  ~WebSystem() {}

public:
  IMPLEMENT_REFCOUNTING(WebSystem);
};

class WebApp : public CefApp
{
public:
  WebApp();

  virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
  virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override;

  IMPLEMENT_REFCOUNTING(WebApp);
};

class WebInterface : public CefBase
{
  friend class WebSystem;

public:
  WebInterface(int width, int height, const std::string& url, bool transparent);
  virtual ~WebInterface();

  void SendFocusEvent(bool setFocus);
  void SendMouseClickEvent(int x, int y, sf::Mouse::Button button, bool mouseUp, int clickCount);
  void SendMouseMoveEvent(int x, int y, bool mouseLeave = false);
  void SendMouseWheelEvent(int x, int y, int deltaX, int deltaY);
  void SendKeyEvent(WPARAM key, bool keyUp, bool isSystem = false, int modifiers = -1);
  void SendKeyEvent(char key, int modifiers = -1);

  void AddJSBinding(const std::string name, JsBinding::JsCallback callback);
  void AddJSBindings(const std::vector<JsBinding> bindings);
  void ExecuteJS(const CefString& code);
  void ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame);
  void ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame, int startLine);

  bool JSCallback(const CefString& name, CefRefPtr<CefListValue> arguments);

  void Close(bool force);

  CefRefPtr<CefBrowser> GetBrowser();
  int GetWidth();
  int GetHeight();

  void UpdateTexture();

  sf::Texture* GetTexture();
  std::string GetCurrentURL();

  bool IsTransparent();

  void SetSize(int width, int height);

private:
  static int GetMouseModifiers();
  static int GetKeyboardModifiers();

  struct UpdateRect
  {
  public:
    char* buffer;
    CefRect rect;
  };

  CefRefPtr<CefBrowser> mBrowser;
  void ClearBrowser();

  sf::Texture* mpTexture;
  int mTextureWidth;
  int mTextureHeight;

  sf::Mutex mMutex;

  std::queue<UpdateRect> mUpdateRects;

  std::string mCurrentURL;

  bool mTransparent;

public:
  IMPLEMENT_REFCOUNTING(WebInterface);
};

class WebV8Handler : public CefV8Handler
{
  friend class WebSystem;

public:
  WebV8Handler();

  virtual bool Execute(const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception) override;

private:
  IMPLEMENT_REFCOUNTING(WebV8Handler);
};

#endif
