#include <cef/RenderHandler.hpp>

RenderHandler::RenderHandler(sf::Texture* p_texture) : m_renderTexture(p_texture)
{
}

RenderHandler::~RenderHandler()
{
  delete m_renderTexture;
}

void RenderHandler::update()
{
  CefDoMessageLoopWork();
}

void RenderHandler::updateTexture()
{
  if (m_updateRects.size() <= 0)
  {
    return;
  }

  sf::Lock lock(m_mutex);
  
  while (m_updateRects.size() > 0)
  {
    const CefRect& rect = m_updateRects.front().rect;
    m_renderTexture->update(reinterpret_cast<sf::Uint8*>(m_updateRects.front().buffer), rect.width, rect.height, rect.x, rect.y);
    delete[] m_updateRects.front().buffer;
    m_updateRects.pop();
  }
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> p_browser, CefRect& p_rect)
{
  p_rect = CefRect(0, 0, m_renderTexture->getSize().x, m_renderTexture->getSize().y);

  return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> p_browser, PaintElementType p_type, const RectList& p_dirtyRects, const void* p_buffer, int p_width, int p_height)
{
  char* bitmap = (char*)(p_buffer);

  CefRenderHandler::RectList::const_iterator i = p_dirtyRects.begin();
  for (; i != p_dirtyRects.end(); ++i)
  {
    const CefRect& rect = *i;
    //Create a rect sized buffer for the new rectangle data.
    char* rectBuffer = new char[rect.width * (rect.height + 1) * BYTES_PER_PIXEL];

    for (int jj = 0; jj < rect.height; jj++)
    {
      //Copy the new rectangle data out of the full size buffer into our rect sized one.  
      memcpy(
        rectBuffer + jj * rect.width * BYTES_PER_PIXEL,
        bitmap + ((rect.x + ((rect.y + jj) * m_renderTexture->getSize().x)) * BYTES_PER_PIXEL),
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
    {
      continue;
    }

    //Update the texture with the new data.  
    //This can be interrupted if the main thread calls a draw on a sprite which uses this texture
    // as the texture is bound by openGL calls.  
    //To rectify this we have the redundancy updating system.  
    m_renderTexture->update(reinterpret_cast<sf::Uint8*>(rectBuffer), rect.width, rect.height, rect.x, rect.y);

    //Here we need to add the data required for the update to the queue for redundancy updates.  
    m_updateRects.push(UpdateRect());
    m_updateRects.back().buffer = rectBuffer;
    m_updateRects.back().rect = rect;
  }
}

