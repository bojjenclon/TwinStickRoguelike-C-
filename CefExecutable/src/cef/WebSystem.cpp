#include "cef/WebSystem.hpp"
#include <fstream>
#include <utility>

/* Static Variables */
std::string WebSystem::sSubprocess = "";
bool WebSystem::sSingleProcess = false;
CefMainArgs WebSystem::sArgs;
CefRefPtr<WebApp> WebSystem::sApp = nullptr;
CefRefPtr<WebSystem> WebSystem::sInstance = nullptr;
std::queue<WebInterface*> WebSystem::sMakeWebInterfaceQueue;
std::map<int, WebInterface*> WebSystem::sWebInterfaces;
WebSystem::BindingMap WebSystem::sBindings;

void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target);
void SetList(CefRefPtr<CefListValue> source, CefRefPtr<CefV8Value> target);

// Transfer a V8 value to a List index.
void SetListValue(CefRefPtr<CefListValue> list, int index, CefRefPtr<CefV8Value> value) {
  if (value->IsArray()) 
  {
    CefRefPtr<CefListValue> new_list = CefListValue::Create();
    SetList(value, new_list);
    list->SetList(index, new_list);
  }
  else if (value->IsString()) 
  {
    list->SetString(index, value->GetStringValue());
  }
  else if (value->IsBool()) 
  {
    list->SetBool(index, value->GetBoolValue());
  }
  else if (value->IsInt()) 
  {
    list->SetInt(index, value->GetIntValue());
  }
  else if (value->IsDouble()) 
  {
    list->SetDouble(index, value->GetDoubleValue());
  }
}

// Transfer a V8 array to a List.
void SetList(CefRefPtr<CefV8Value> source, CefRefPtr<CefListValue> target) {
  //ASSERT(source->IsArray());

  int arg_length = source->GetArrayLength();
  if (arg_length == 0)
  {
    return;
  }

  // Start with null types in all spaces.
  target->SetSize(arg_length);

  for (int i = 0; i < arg_length; ++i)
  {
    SetListValue(target, i, source->GetValue(i));
  }
}


CefRefPtr<WebSystem> WebSystem::GetInstance()
{
  if (!sInstance)
  {
    sInstance = new WebSystem();
  }

  return sInstance;
}

void WebSystem::AddBrowserToInterface(WebInterface* pWeb)
{
  //base::AutoLock lock_scope(mLock);

  CefWindowInfo window_info;
  window_info.windowless_rendering_enabled = true;
  window_info.transparent_painting_enabled = true;
  //window_info.SetAsWindowless(pWeb->mHandle, true);
  
  CefBrowserSettings browserSettings;
  //browserSettings.javascript_access_clipboard = STATE_ENABLED;

  CefRefPtr<CefBrowser> browser = CefBrowserHost::CreateBrowserSync(window_info, this, pWeb->GetCurrentURL(), browserSettings, nullptr);

  pWeb->mBrowser = browser;

  sWebInterfaces[browser->GetIdentifier()] = pWeb;
}

int WebSystem::Main()
{
#if(PLATFORM == WINDOWS)
  sArgs = CefMainArgs(GetModuleHandle(nullptr));
#else
  sArgs = CefMainArgs();
#endif
  sApp = new WebApp();
  
  int exit_code = CefExecuteProcess(sArgs, sApp.get(), nullptr);

  if (exit_code >= 0)
  {
    return exit_code;
  }

  return -1;
}

void WebSystem::SetSubprocess(const std::string path)
{
  sSubprocess = path;
}

void WebSystem::SetSingleProcess(bool single)
{
  sSingleProcess = single;
}

void WebSystem::StartWeb()
{
  if (!sApp)
  {
    Main();
  }

  //Standard fare CEF initialization.
  CefSettings settings;
  settings.multi_threaded_message_loop = false;
  settings.single_process = sSingleProcess;

  //If we have a specified subrocess path, use that subprocess.
  if (sSubprocess != "")
  {
    //Check if file exists.
    std::ifstream ifile(sSubprocess.c_str());
    if (ifile)
    {
      CefString(&settings.browser_subprocess_path).FromASCII(sSubprocess.c_str());
    }
  }

  CefInitialize(sArgs, settings, sApp.get(), nullptr);
}

void WebSystem::RunWeb()
{
  while (sMakeWebInterfaceQueue.size() > 0)
  {
    GetInstance()->AddBrowserToInterface(sMakeWebInterfaceQueue.front());
    sMakeWebInterfaceQueue.pop();
  }

  CefDoMessageLoopWork();
}

void WebSystem::EndWeb()
{
  //WE SHOULD PROBABLY CHECK IF THERE ARE STILL LIVE BROWSERS HERE

  CefShutdown();
}

WebInterface* WebSystem::CreateWebInterfaceSync(int width, int height, const std::string& url, bool transparent)
{
  WebInterface* pWeb = new WebInterface(width, height, url, transparent);

  sMakeWebInterfaceQueue.push(pWeb);

  while (!pWeb->mBrowser)
  {
    RunWeb();

    //Sleep for one millisecond to prevent 100% CPU usage.
    //There is probably a more elegant solution for this.
    Sleep(1);
  }

  return pWeb;
}

void WebSystem::UpdateInterfaceTextures()
{
  std::map<int, WebInterface*>::iterator i;
  for (i = sWebInterfaces.begin(); i != sWebInterfaces.end(); i++)
  {
    i->second->UpdateTexture();
  }
}

void WebSystem::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
  //Send all JsBindings
  BindingMap::iterator i = sBindings.begin();
  for (; i != sBindings.end(); i++)
  {
    if (i->first.second == browser->GetIdentifier())
    {
      CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
      message->GetArgumentList()->SetString(0, i->first.first);
      message->GetArgumentList()->SetInt(1, i->first.second);

      browser->SendProcessMessage(PID_RENDERER, message);
    }
  }
}

void WebSystem::OnBrowserDestroyed(CefRefPtr<CefBrowser> browser)
{
  BindingMap::iterator i = sBindings.begin();
  for (; i != sBindings.end();)
  {
    if (i->first.second == browser->GetIdentifier())
    {
      sBindings.erase(i++);
    }
    else
    {
      ++i;
    }
  }
}

void WebSystem::OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
  //Retrieve the context's window object.
  CefRefPtr<CefV8Value> object = context->GetGlobal();

  //Create a new callback handler.
  CefRefPtr<CefV8Handler> handler = new WebV8Handler();

  //Setup bindings for that web interface.	
  BindingMap::iterator i = sBindings.begin();
  for (; i != sBindings.end(); i++)
  {
    if (i->first.second == browser->GetIdentifier())
    {
      CefRefPtr<CefV8Value> func = CefV8Value::CreateFunction(i->first.first, handler);

      object->SetValue(i->first.first, func, V8_PROPERTY_ATTRIBUTE_NONE);
    }
  }
}

void WebSystem::OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context)
{
}

void WebSystem::OnUncaughtException(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace)
{
}

void WebSystem::OnRenderProcessThreadCreated(CefRefPtr<CefListValue> extra_info)
{
}

bool WebSystem::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
  CefString message_name = message->GetName();

  //Identify whether we are in the browser process or the render process.
  if (CefCurrentlyOn(TID_RENDERER))
  {
    //Renderer
    if (message_name == "jsbinding_create")
    {
      std::string name = message->GetArgumentList()->GetString(0);

      sBindings.insert(std::make_pair(std::make_pair(name, message->GetArgumentList()->GetInt(1)), static_cast<JsBinding::JsCallback>(nullptr)));

      browser->Reload();

      return true;
    }
  }
  else
  {
    //Browser
    if (message_name == "jsbinding_call")
    {
      if (sWebInterfaces.count(browser->GetIdentifier()))
      {
        WebInterface* pWeb = sWebInterfaces[browser->GetIdentifier()];

        std::string name;
        CefRefPtr<CefListValue> arguments = CefListValue::Create();
        CefRefPtr<CefV8Value> retval;
        CefString exception;

        CefRefPtr<CefListValue> margs = message->GetArgumentList();
        name = margs->GetString(0);
        int argCount = margs->GetInt(1);
        std::string test = margs->GetString(2);
        for (int i = 0; i < argCount; i++)
        {
          CefValueType type = margs->GetType(i + 2);
          switch (type)
          {
            case VTYPE_LIST:
            {
              CefRefPtr<CefListValue> list = margs->GetList(i + 2);
              arguments->SetList(i, list);
              break;
            }
            case VTYPE_BOOL:
            {
              bool b = margs->GetBool(i + 2);
              arguments->SetBool(i, b);
              break;
            }
            case VTYPE_DOUBLE:
            {
              double d = margs->GetDouble(i + 2);
              arguments->SetDouble(i, d);
              break;
            }
            case VTYPE_INT:
            {
              int integer = margs->GetInt(i + 2);
              arguments->SetInt(i, integer);
              break;
            }
            case VTYPE_STRING:
            {
              std::string s = margs->GetString(i + 2);
              arguments->SetString(i, s);
              break;
            }
            default:
              break;
          }
        }

        pWeb->JSCallback(name, arguments);

        return true;
      }
    }
  }

  return false;
}

void WebSystem::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame)
{
}

void WebSystem::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{
}

void WebSystem::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
  //base::AutoLock lock_scope(mLock);
}

void WebSystem::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
  //base::AutoLock lock_scope(mLock);

  //Free the browser pointer so that the browser can be destroyed.
  if (sWebInterfaces.count(browser->GetIdentifier()))
  {
    sWebInterfaces[browser->GetIdentifier()]->ClearBrowser();
  }
}

bool WebSystem::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect)
{
  if (!sWebInterfaces.count(browser->GetIdentifier()))
  {
    rect = CefRect();
    return true;
  }

  WebInterface* pWeb = sWebInterfaces[browser->GetIdentifier()];
  if (!pWeb)
  {
    rect = CefRect();
    return true;
  }

  rect = CefRect(0, 0, pWeb->GetWidth(), pWeb->GetHeight());

  return true;
}

void WebSystem::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height)
{
  printf("On Paint\n");
  ////Get the web interface we will be working with. 
  if (!sWebInterfaces.count(browser->GetIdentifier()))
  {
    return;
  }

  WebInterface* pWeb = sWebInterfaces[browser->GetIdentifier()];
  if (!pWeb)
  {
    return;
  }

  if (!buffer)
  {
    return;
  }

  if (type == PET_VIEW)
  {
    int old_width = pWeb->mTextureWidth;
    int old_height = pWeb->mTextureHeight;

    //Retrieve current size of browser view.
    pWeb->mTextureWidth = width;
    pWeb->mTextureHeight = height;

    //Check if we need to resize the texture before drawing to it.
    if (old_width != pWeb->mTextureWidth || old_height != pWeb->mTextureHeight)
    {
      //This literally has never been called since the creation of this project, thus 
      //it was never updated, and the outdated code has now been removed.
      //printf("Called resize code in onpaint.\n");
    }
    else
    {
      //We want to work on the buffer byte by byte so get a pointer with a new type.
      char* bitmap = (char*)(buffer);

      sf::Lock lock(pWeb->mMutex);
      //Update the dirty rectangles.
      CefRenderHandler::RectList::const_iterator it = dirtyRects.begin();
      for (; it != dirtyRects.end(); ++it)
      {
        const CefRect& rect = *it;
        //Create a rect sized buffer for the new rectangle data.
        char* rectBuffer = new char[rect.width * (rect.height + 1) * BYTES_PER_PIXEL];

        for (int jj = 0; jj < rect.height; jj++)
        {
          //Copy the new rectangle data out of the full size buffer into our rect sized one.  
          memcpy(
            rectBuffer + jj * rect.width * BYTES_PER_PIXEL,
            bitmap + ((rect.x + ((rect.y + jj) * pWeb->mTextureWidth)) * BYTES_PER_PIXEL),
            rect.width * BYTES_PER_PIXEL
          );
        }

        //Convert BGRA to RGBA
        unsigned int* pTmpBuf = reinterpret_cast<unsigned int*>(rectBuffer);
        const int numPixels = rect.width * rect.height;
        for (int i = 0; i < numPixels; i++)
        {
          pTmpBuf[i] = (pTmpBuf[i] & 0xFF00FF00) | ((pTmpBuf[i] & 0x00FF0000) >> 16) | ((pTmpBuf[i] & 0x000000FF) << 16);
        }

        if (!rectBuffer)
          continue;
        //Update the texture with the new data.  
        //This can be interrupted if the main thread calls a draw on a sprite which uses this texture
        // as the texture is bound by openGL calls.  
        //To rectify this we have the redundancy updating system.  
        pWeb->mpTexture->update(reinterpret_cast<sf::Uint8*>(rectBuffer), rect.width, rect.height, rect.x, rect.y);

        //Here we need to add the data required for the update to the queue for redundancy updates.  
        pWeb->mUpdateRects.push(WebInterface::UpdateRect());
        pWeb->mUpdateRects.back().buffer = rectBuffer;
        pWeb->mUpdateRects.back().rect = rect;
      }
    }
  }
}

////////////////////////////////////////////////////////////
bool WebSystem::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent& e, CefEventHandle os_event)
{
  if (e.is_system_key && e.modifiers == 4)
  {
    if (e.character == 'V')
    {
      browser->GetFocusedFrame()->Paste();
    }
    else if (e.character == 'X')
    {
      browser->GetFocusedFrame()->Cut();
    }
    else if (e.character == 'C')
    {
      browser->GetFocusedFrame()->Copy();
    }
    else if (e.character == 'Z')
    {
      browser->GetFocusedFrame()->Undo();
    }
    else if (e.character == 'Y')
    {
      browser->GetFocusedFrame()->Redo();
    }
    else if (e.character == 'A')
    {
      browser->GetFocusedFrame()->SelectAll();
    }
  }

  return true;
}

/* WebApp Definitions */

WebApp::WebApp()
{
}

CefRefPtr<CefBrowserProcessHandler> WebApp::GetBrowserProcessHandler()
{
  return WebSystem::GetInstance().get();
}

CefRefPtr<CefRenderProcessHandler> WebApp::GetRenderProcessHandler()
{
  return WebSystem::GetInstance().get();
}

/* WebInterface Definitions */

WebInterface::WebInterface(int width, int height, const std::string& url, bool transparent)
  : mTextureWidth(width)
  , mTextureHeight(height)
  , mCurrentURL(url)
  , mTransparent(transparent)
  , mBrowser(nullptr)
  , mpTexture(nullptr)
{
  mpTexture = new sf::Texture();
  sf::Image img;
  img.create(mTextureWidth, mTextureHeight, sf::Color(0, 0, 0, 0));
  mpTexture->loadFromImage(img);
  mpTexture->setSmooth(true);
}

WebInterface::~WebInterface()
{
  sf::Lock lock(mMutex);

  delete mpTexture;

  while (mUpdateRects.size() > 0)
  {
    delete[] mUpdateRects.front().buffer;
    mUpdateRects.pop();
  }

  std::map<int, WebInterface*>::iterator i;
  for (i = WebSystem::sWebInterfaces.begin(); i != WebSystem::sWebInterfaces.end(); i++)
  {
    if (i->second == this)
    {
      WebSystem::sWebInterfaces.erase(i);
      break;
    }
  }

  if (mBrowser)
  {
    mBrowser->GetHost()->CloseBrowser(true);
    mBrowser = nullptr;
  }
}

void WebInterface::UpdateTexture()
{
  if (mUpdateRects.size() <= 0)
  {
    return;
  }

  sf::Lock lock(mMutex);

  while (mUpdateRects.size() > 0)
  {
    int size = mUpdateRects.size();
    const CefRect& rect = mUpdateRects.front().rect;
    mpTexture->update(reinterpret_cast<sf::Uint8*>(mUpdateRects.front().buffer), rect.width, rect.height, rect.x, rect.y);
    delete[] mUpdateRects.front().buffer;
    mUpdateRects.pop();
  }
}

void WebInterface::SendFocusEvent(bool setFocus)
{
  mBrowser->GetHost()->SendFocusEvent(setFocus);
}

int WebInterface::GetMouseModifiers()
{
  int mod = 0;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    mod |= EVENTFLAG_CONTROL_DOWN;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
    mod |= EVENTFLAG_SHIFT_DOWN;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) ||
    sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
    mod |= EVENTFLAG_ALT_DOWN;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    mod |= EVENTFLAG_LEFT_MOUSE_BUTTON;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
    mod |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
  if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
    mod |= EVENTFLAG_RIGHT_MOUSE_BUTTON;

  // Low bit set from GetKeyState indicates "toggled".
  //if (::GetKeyState(VK_NUMLOCK) & 1)
  //	mod |= EVENTFLAG_NUM_LOCK_ON;
  //if (::GetKeyState(VK_CAPITAL) & 1)
  //	mod |= EVENTFLAG_CAPS_LOCK_ON;

  return mod;
}

int WebInterface::GetKeyboardModifiers()
{
  int mod = 0;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
    mod |= EVENTFLAG_CONTROL_DOWN;// mod |= EVENTFLAG_IS_LEFT;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    mod |= EVENTFLAG_CONTROL_DOWN;// mod |= EVENTFLAG_IS_RIGHT;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
    mod |= EVENTFLAG_SHIFT_DOWN;// mod |= EVENTFLAG_IS_LEFT;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
    mod |= EVENTFLAG_SHIFT_DOWN;// mod |= EVENTFLAG_IS_RIGHT;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
    mod |= EVENTFLAG_ALT_DOWN;// mod |= EVENTFLAG_IS_LEFT;
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
    mod |= EVENTFLAG_ALT_DOWN;// mod |= EVENTFLAG_IS_RIGHT;

  return mod;
}

void WebInterface::SendMouseClickEvent(int x, int y, sf::Mouse::Button button, bool mouseUp, int clickCount)
{
  if (!mBrowser)
  {
    return;
  }

  CefBrowserHost::MouseButtonType type = button == sf::Mouse::Left ? MBT_LEFT :
    button == sf::Mouse::Right ? MBT_RIGHT : MBT_MIDDLE;

  CefMouseEvent e; e.x = x; e.y = y; e.modifiers = GetMouseModifiers();
  mBrowser->GetHost()->SendMouseMoveEvent(e, false);
  mBrowser->GetHost()->SendMouseClickEvent(e, type, mouseUp, clickCount);
}

void WebInterface::SendMouseMoveEvent(int x, int y, bool mouseLeave)
{
  if (!mBrowser)
  {
    return;
  }

  CefMouseEvent e; e.x = x; e.y = y; e.modifiers = GetMouseModifiers();
  mBrowser->GetHost()->SendMouseMoveEvent(e, mouseLeave);
}

void WebInterface::SendMouseWheelEvent(int x, int y, int deltaX, int deltaY)
{
  if (!mBrowser)
  {
    return;
  }

  CefMouseEvent e; e.x = x; e.y = y; e.modifiers = GetMouseModifiers();
  mBrowser->GetHost()->SendMouseWheelEvent(e, deltaX, deltaY);
}

void WebInterface::SendKeyEvent(WPARAM key, bool keyUp, bool isSystem, int modifiers)
{
  if (!mBrowser)
  {
    return;
  }

  CefKeyEvent e; e.windows_key_code = key; e.modifiers = modifiers == -1 ? GetKeyboardModifiers() : modifiers;
  e.type = keyUp ? KEYEVENT_KEYUP : KEYEVENT_KEYDOWN;
  e.is_system_key = isSystem; e.character = key; e.unmodified_character = key; //e.native_key_code = 0;
  mBrowser->GetHost()->SendKeyEvent(e);
}

void WebInterface::SendKeyEvent(char key, int modifiers)
{
  if (!mBrowser)
  {
    return;
  }

  CefKeyEvent e; e.windows_key_code = key; e.modifiers = modifiers == -1 ? GetKeyboardModifiers() : modifiers;
  e.type = KEYEVENT_CHAR; e.character = key; e.unmodified_character = key;
  mBrowser->GetHost()->SendKeyEvent(e);
}

void WebInterface::AddJSBinding(const std::string name, JsBinding::JsCallback callback)
{
  WebSystem::sBindings.insert(std::make_pair(std::make_pair(name, mBrowser->GetIdentifier()), callback));

  CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
  message->GetArgumentList()->SetString(0, name.c_str());
  message->GetArgumentList()->SetInt(1, mBrowser->GetIdentifier());

  mBrowser->SendProcessMessage(PID_RENDERER, message);

  //We have to reload the page otherwise the bindings wont be added to the context.
  mBrowser->Reload();
}

void WebInterface::AddJSBindings(const std::vector<JsBinding> bindings)
{
  for (unsigned int i = 0; i < bindings.size(); i++)
  {
    WebSystem::sBindings.insert(std::make_pair(std::make_pair(bindings[i].mFunctionName, mBrowser->GetIdentifier()), bindings[i].mfpJSCallback));

    CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_create");
    message->GetArgumentList()->SetString(0, bindings[i].mFunctionName);
    message->GetArgumentList()->SetInt(1, mBrowser->GetIdentifier());

    mBrowser->SendProcessMessage(PID_RENDERER, message);
  }

  //We have to reload the page otherwise the bindings wont be added to the context.
  mBrowser->Reload();
}

void WebInterface::ExecuteJS(const CefString& code)
{
  if (!mBrowser)
  {
    return;
  }

  ExecuteJS(code, mBrowser->GetMainFrame());
}

void WebInterface::ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame)
{
  if (!mBrowser)
  {
    return;
  }

  //Should probably check to make sure the frame is from our browser here.

  ExecuteJS(code, frame, 0);
}

void WebInterface::ExecuteJS(const CefString& code, CefRefPtr<CefFrame> frame, int startLine)
{
  if (!mBrowser)
  {
    return;
  }

  //Should probably check to make sure the frame is from our browser here.

  frame->ExecuteJavaScript(code, frame->GetURL(), startLine);
}

bool WebInterface::JSCallback(const CefString& name, CefRefPtr<CefListValue> arguments)
{
  bool result = false;
  //Check if this is one of our bindings.

  if (WebSystem::sBindings.count(std::make_pair(name, mBrowser->GetIdentifier())))
  {
    result = WebSystem::sBindings[std::make_pair(name, mBrowser->GetIdentifier())](arguments);
  }

  //Otherwise fallthrough and return false.
  return result;
}

void WebInterface::SetSize(int width, int height)
{
  mTextureWidth = width;
  mTextureHeight = height;

  if (mBrowser)
  {
    mBrowser->GetHost()->WasResized();
  }

  sf::Texture* pOldTexture = mpTexture;
  mpTexture = new sf::Texture();
  mpTexture->create(mTextureWidth, mTextureHeight);
  mpTexture->setSmooth(true);
  if (pOldTexture)
  {
    delete pOldTexture;
  }

  sf::Lock lock(mMutex);

  //Clear the update rects
  while (mUpdateRects.size() > 0)
  {
    delete[] mUpdateRects.front().buffer;
    mUpdateRects.pop();
  }

  if (mBrowser)
  {
    mBrowser->GetHost()->Invalidate(PET_VIEW);
  }
}

void WebInterface::Close(bool force = true)
{
  sf::Lock lock(mMutex);
  mBrowser->GetHost()->CloseBrowser(force);
}

CefRefPtr<CefBrowser> WebInterface::GetBrowser()
{
  return mBrowser;
}

int WebInterface::GetWidth()
{
  return mTextureWidth;
}

int WebInterface::GetHeight()
{
  return mTextureHeight;
}

sf::Texture* WebInterface::GetTexture()
{
  return mpTexture;
}

std::string WebInterface::GetCurrentURL()
{
  return mCurrentURL;
}

bool WebInterface::IsTransparent()
{
  return mTransparent;
}

void WebInterface::ClearBrowser()
{
  mBrowser = NULL;
}

/* WebV8Handler Definitions */

WebV8Handler::WebV8Handler()
{
}

bool WebV8Handler::Execute(const CefString& name,
  CefRefPtr<CefV8Value> object,
  const CefV8ValueList& arguments,
  CefRefPtr<CefV8Value>& retval,
  CefString& exception)
{
  //Send message to browser process to call function
  CefRefPtr<CefBrowser> browser = CefV8Context::GetCurrentContext()->GetBrowser();
  CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create("jsbinding_call");
  message->GetArgumentList()->SetString(0, name);
  message->GetArgumentList()->SetInt(1, arguments.size());
  for (unsigned int i = 0; i < arguments.size(); i++)
  {
    SetListValue(message->GetArgumentList(), i + 2, arguments[i]);
  }

  browser->SendProcessMessage(PID_BROWSER, message);

  return false;
}