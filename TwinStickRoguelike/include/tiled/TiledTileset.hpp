#ifndef TILED_TILESET_HPP
#define TILED_TILESET_HPP

#include <string>

class TiledTileset
{
public:
  struct TilesetOptions
  {
    int firstGid = 1;
    std::string imagePath;
    std::string name;
    int tileWidth;
    int tileHeight;
    int tileCount;
    int imageWidth;
    int imageHeight;
    int spacing;
    int margin;

    TilesetOptions(int p_firstGid, std::string p_imagePath, std::string p_name, int p_tileWidth, int p_tileHeight, int p_tileCount, int p_imageWidth, int p_imageHeight, int p_spacing, int p_margin)
      : firstGid(p_firstGid), imagePath(p_imagePath), name(p_name), 
      tileWidth(p_tileWidth), tileHeight(p_tileHeight), tileCount(p_tileCount), imageWidth(p_imageWidth), imageHeight(p_imageHeight), 
      spacing(p_spacing), margin(p_margin)
    {
      
    }
  };

public:
  TiledTileset();
  TiledTileset(TilesetOptions p_options);

  int getFirstGid() const;
  std::string getImagePath() const;
  std::string getName() const;
  int getTileWidth() const;
  int getTileHeight() const;
  int getTileCount() const;
  int getImageWidth() const;
  int getImageHeight() const;
  int getSpacing() const;
  int getMargin() const;

  int getTileIndex(int p_gid) const;

private:
  int m_firstGid;

  std::string m_imagePath;
  std::string m_name;

  int m_tileWidth;
  int m_tileHeight;
  int m_tileCount;

  int m_imageWidth;
  int m_imageHeight;

  int m_spacing;
  int m_margin;
};

#endif
