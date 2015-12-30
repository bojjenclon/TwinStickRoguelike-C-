#include <tiled/TiledTileLayer.hpp>
#include <tiled/TiledMap.hpp>
#include <base64/base64.hpp>
#include <zlib.h>

TiledTileLayer::TiledTileLayer()
{
  m_depth = 1;
}

TiledTileLayer::TiledTileLayer(TileCollection p_tiles) : m_tiles(p_tiles)
{
  m_width = m_tiles.size();
  m_height = m_tiles[0].size();

  m_depth = 1;
}

TiledTileLayer::TiledTileLayer(int p_width, int p_height) : m_tiles(p_width, std::vector<TiledTile>(p_height)), m_width(p_width), m_height(p_height)
{
  m_depth = 1;
}

TiledTile TiledTileLayer::getTile(int p_x, int p_y) const
{
  return m_tiles[p_x][p_y];
}

int TiledTileLayer::getTileGid(int p_x, int p_y) const
{
  return m_tiles[p_x][p_y].getGid();
}

int TiledTileLayer::getTileId(int p_x, int p_y) const
{
  return m_tiles[p_x][p_y].getId();
}

int TiledTileLayer::getWidth() const
{
  return m_width;
}

int TiledTileLayer::getHeight() const
{
  return m_height;
}

int TiledTileLayer::getDepth() const
{
  return m_depth;
}

void TiledTileLayer::setDepth(int p_depth)
{
  m_depth = p_depth;
}

TiledTileLayer TiledTileLayer::createFromData(const TiledMap& p_map, std::string p_data, int p_width, int p_height)
{
  auto decoded = base64_decode(p_data);
  std::vector<char> compressed(decoded.begin(), decoded.end());

  uLongf outlen = p_width * p_height * 4;
  auto decompressed = static_cast<unsigned *>(malloc(outlen));
  uncompress(
    reinterpret_cast<Bytef*>(decompressed), &outlen,
    reinterpret_cast<const Bytef*>(decoded.c_str()), decoded.size());

  TileCollection tiles(p_width, std::vector<TiledTile>(p_height));

  static const auto FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
  static const auto FLIPPED_VERTICALLY_FLAG = 0x40000000;
  static const auto FLIPPED_DIAGONALLY_FLAG = 0x20000000;

  for (auto x = 0; x < p_width; ++x)
  {
    for (auto y = 0; y < p_height; ++y)
    {
      auto gid = decompressed[y * p_width + x];

      auto isFlippedHorizontally = (gid & FLIPPED_HORIZONTALLY_FLAG) != 0;
      auto isFlippedVertically = (gid & FLIPPED_VERTICALLY_FLAG) != 0;
      auto isFlippedDiagonally = (gid & FLIPPED_DIAGONALLY_FLAG) != 0;

      gid &= ~(FLIPPED_HORIZONTALLY_FLAG | FLIPPED_VERTICALLY_FLAG | FLIPPED_DIAGONALLY_FLAG);

      tiles[x][y] = TiledTile(p_map.findTilesetFromGid(gid), gid);
    }
  }

  free(decompressed);

  TiledTileLayer layer(tiles);
  return layer;
}
